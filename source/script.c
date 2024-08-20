/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * MelonScript
 */

#include "log.h"
#include "string.h"

#include "script.h"

/**
 * Various types of tokens
 */
typedef enum DgScriptTokenType {
	DG_SCRIPT_TOKEN_NIL, // nil
	DG_SCRIPT_TOKEN_BOOLEAN, // true|false
	DG_SCRIPT_TOKEN_ID, // (<letter>|$|_)(<digit>|<letter>|$|_)*
	DG_SCRIPT_TOKEN_NUMBER, // <digits>(.<digits>)?([Ee][+-]?<digits>)?|0[Xx]<hexdigits>|0[Bb]<bits>|<digits>[Rr](<digit>|<letter>)
	DG_SCRIPT_TOKEN_STRING, // "([^"\\]|\\.)*"
	DG_SCRIPT_TOKEN_KEYWORD, // if, else, while, for, throw, function, etc.
	DG_SCRIPT_TOKEN_OP, // +|-|*|/|%|\\|\||^|&|~|#|<<|>>|+=|-=|*=|/=|%=|\\=|\|=|^=|&=|~=|<<=|>>=|=|:|,|?|@|!|==|!=|<|>|<>|<=|>=|(|)|[|]|{|}|->|<-|=>|<=
	DG_SCRIPT_TOKEN_ERROR, // Not a token - there was an error.
} DgScriptTokenType;

// Whitespace: <space>|\t|\r|\b|\f|\v
// Comments: //[^\n]*|/\*.*\*/

enum : uint8_t {
	DG_SCRIPT_EOF = 0xff, // End of file magic marker
};

/**
 * A single source token
 */
typedef struct DgScriptToken {
	DgScriptTokenType type;
	union {
		char *asText; // strings, symbols
		bool asBool; // booleans
		int64_t asInt; // integer numbers, op
		double asDouble; // decimal numbers
	};
	size_t start, end;
} DgScriptToken;

/**
 * State of the lexer
 */
typedef struct DgScriptLexer {
	const char *source; // The content to lex
	size_t start; // Start of the current lexeme e.g. after the last one ended
	size_t head; // Head pointer
} DgScriptLexer;

void DgScriptLexerInit(DgScriptLexer *this, const char *code) {
	/**
	 * Initialise the lexer given the source code string to lex
	 */
	
	this->source = code;
	this->start = 0;
	this->head = 0;
}

char DgScriptLexerReadChar(DgScriptLexer *this) {
	/**
	 * Read the next character and icrement the head
	 *
	 * @param this Script lexer instance
	 */
	
	if (this->head >= DgStringLength(this->source)) {
		return DG_SCRIPT_EOF;
	}
	
	return this->source[this->head++];
}

void DgScriptLexerUnread(DgScriptLexer *this) {
	this->head--;
}

DgScriptToken DgScriptLexerAccept(DgScriptLexer *this, DgScriptTokenType type) {
	/**
	 * Accepts a token of the given type. This is where most of the magic of
	 * getting from a matched string to a token happens. Also advances the start
	 * of token offset to match the current head.
	 *
	 * @param this Script lexer instance
	 * @param type The type of token to accept; for example, DG_SCRIPT_TOKEN_ID for an identifier
	 * @return Resultant token
	 */
	
	DgScriptToken token;
	
	token.type = type;
	
	switch (type) {
		case DG_SCRIPT_TOKEN_NIL: {
			break;
		}
		case DG_SCRIPT_TOKEN_BOOLEAN: {
			// HACK should really do some proper string compare
			token.asBool = this->source[this->start] == 't';
			break;
		}
		case DG_SCRIPT_TOKEN_ID: {
			token.asText = DgStringDuplicateUntil(&this->source[this->start], this->head - this->start);
			break;
		}
		case DG_SCRIPT_TOKEN_NUMBER: {
			// TODO: oh fuck it's another unimplemented thing
			DgLog(DG_LOG_WARNING, "lexer: can't parse numbers (heh)");
			break;
		}
		case DG_SCRIPT_TOKEN_STRING: {
			// TODO: This string should be escaped!
			token.asText = DgStringDuplicateUntil(&this->source[this->start + 1], this->head - this->start - 1);
			break;
		}
		case DG_SCRIPT_TOKEN_KEYWORD: {
			token.asText = DgStringDuplicateUntil(&this->source[this->start], this->head - this->start);
			break;
		}
		case DG_SCRIPT_TOKEN_OP: {
			// HACK spooky
			int64_t value = 0;
			
			for (size_t i = this->start; i < this->head; i++) {
				value |= this->source[i];
				value <<= 8;
			}
			
			value >>= 8;
			
			token.asInt = value;
			
			break;
		}
		case DG_SCRIPT_TOKEN_ERROR: {
			// Does not really do anything
			
			break;
		}
		default: {
			DgLog(DG_LOG_WARNING, "lexer: unknown token type %d", type);
			break;
		}
	}
	
	// Set token location
	token.start = this->start;
	token.end = this->head;
	
	// Advance start of token to current head
	this->start = this->head;
	
	// Return the new token
	return token;
}

#define readChar() DgScriptLexerReadChar(this)
#define accept(TYPE) (*result = DgScriptLexerAccept(this, TYPE)); goto done
#define unread() DgScriptLexerUnread(this)
#define inRange(LOW, VAL, HIGH) ((VAL >= LOW) && (VAL <= HIGH))

#define isLetter(C) (inRange('a', C, 'z') || inRange('A', C, 'Z'))
#define isNumber(C) inRange('0', C, '9')
#define isHexdigit(C) (inRange('0', C, '9') || inRange('a', C, 'f') || inRange('A', C, 'F'))
#define isBit(C) (c == '0' || c == '1')

const char *gScriptKeywords[] = {
	"if",
	NULL,
};

// NOTE: Longer tokens with the same prefix must come BEFORE shorter ones!
// Bet you can't guess how I've implemented this :TailsHeh:
const char *gScriptOps[] = {
	"+",
	"-",
	"*",
	"/",
	"%",
	"\\",
	"|",
	"^",
	"&",
	"~",
	"#",
	"<<",
	">>",
	"+=",
	"-=",
	"*=",
	"/=",
	"%=",
	"\\=",
	"|=",
	"^=",
	"&=",
	"~=",
	"<<=",
	">>=",
	"=",
	":",
	",",
	"?",
	"@",
	"!",
	"==",
	"!=",
	"<",
	">",
	"<>",
	"<=",
	">=",
	"(",
	")",
	"[",
	"]",
	"{",
	"}",
	"->",
	"<-",
	"=>",
	"<=",
	NULL,
};

DgError DgScriptLexerNextToken(DgScriptLexer *this, DgScriptToken *result) {
	/**
	 * Get the next token
	 * 
	 * @note This function only needs to recognise the tokens and doesn't need
	 * to worry about the actual contents of the token; that will be handled by
	 * accept().
	 */
	
	char c = readChar();
	
	// Read all whitespace
	do {
		if (!(c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v')) {
			break;
		}
		
		c = readChar();
	} while (true);
	
	if (isLetter(c) || c == '_' || c == '$') {
		// Start of an identifier
		while (true) {
			c = readChar();
			
			if (!(isLetter(c) || isNumber(c) || c == '_' || c == '$')) {
				unread();
				accept(DG_SCRIPT_TOKEN_ID);
			}
		}
	}
	else if (isNumber(c)) {
		// Start of some type of numerical constant
		// TODO uses closure instead of positive closure everywhere and 
		while (true) {
			c = readChar();
			if (!isNumber(c)) { break; }
		}
		
		if (c == '.') {
			while (true) {
				c = readChar();
				if (!isNumber(c)) { break; }
			}
		}
		
		if (c == 'e' || c == 'E') {
			c = readChar();
			
			if (c == '+' || c == '-') { c = readChar(); }
			
			while (true) {
				c = readChar();
				if (!isNumber(c)) { break; }
			}
		}
	}
	else {
		// An operator or an error of some kind
	}
	
	// Cannot accept any token like that.
	return DG_ERROR_FAILED;
	
	done: return DG_ERROR_SUCCESS;
}
