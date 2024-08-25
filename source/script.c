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

enum : uint16_t {
	DG_SCRIPT_EOF = 0x100, // End of file magic marker
};

typedef enum {
	DG_SCRIPT_LEX_OKAY = 1,
	DG_SCRIPT_LEX_NOT_MATCHED,
	DG_SCRIPT_LEX_EOF,
} DgScriptLexerStatus;

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

uint16_t DgScriptLexerReadChar(DgScriptLexer *this) {
	/**
	 * Read the next character and icrement the head
	 *
	 * @param this Script lexer instance
	 */
	
	if (this->head >= DgStringLength(this->source)) {
		return DG_SCRIPT_EOF;
	}
	
	DgLog(DG_LOG_VERBOSE, "read char: %c", this->source[this->head]);
	
	return this->source[this->head++];
}

void DgScriptLexerUnread(DgScriptLexer *this) {
	DgLog(DG_LOG_VERBOSE, "unread char");
	
	this->head--;
}

void DgScriptLexerSync(DgScriptLexer *this) {
	/**
	 * Sync the start with the head
	 */
	
	this->start = this->head;
}

const char *DgScriptLexerRemainingString(DgScriptLexer *this) {
	/**
	 * Get the remaining string to lex relative to the head.
	 */
	
	return &this->source[this->head];
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
			token.asText = DgStringDuplicateUntil(&this->source[this->start], this->head - this->start);
			break;
		}
		case DG_SCRIPT_TOKEN_STRING: {
			// TODO: This string should be escaped!
			token.asText = DgStringDuplicateUntil(&this->source[this->start + 1], this->head - this->start - 2);
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
#define sync() DgScriptLexerSync(this)
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
	"+=",
	"+",
	"->",
	"-=",
	"-",
	"*=",
	"*",
	"/=",
	"/",
	"%=",
	"%",
	"\\=",
	"\\",
	"|=",
	"|",
	"^=",
	"^",
	"&=",
	"&",
	"~=",
	"~",
	"#",
	"<<=",
	"<<",
	">>=",
	">>",
	"==",
	"=>",
	"=",
	":=",
	":",
	".",
	",",
	"?",
	";",
	"@",
	"!=",
	"!",
	"<-",
	"<=",
	"<>",
	"<",
	">=",
	">",
	"(",
	")",
	"[",
	"]",
	"{",
	"}",
	NULL,
};

DgScriptLexerStatus DgScriptLexerNextToken(DgScriptLexer *this, DgScriptToken *result) {
	/**
	 * Get the next token
	 * 
	 * @note This function only needs to recognise the tokens and doesn't need
	 * to worry about the actual contents of the token; that will be handled by
	 * accept().
	 */
	
	uint16_t c = readChar();
	
	// Read all whitespace
	do {
		if (!(c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v')) {
			break;
		}
		
		c = readChar();
	} while (true);
	
	unread();
	sync();
	readChar();
	
	if (c == DG_SCRIPT_EOF) {
		return DG_SCRIPT_LEX_EOF;
	}
	
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
	else if (c == '"') {
		while (true) {
			c = readChar();
			
			if (c == '"') {
				// Accept the string :D
				accept(DG_SCRIPT_TOKEN_STRING);
			}
			else if (c == '\\') {
				// skip char regardless of what it is, if preceeded by forward
				readChar();
			}
			else if (c == DG_SCRIPT_EOF) {
				// fallthrough to failure
				break;
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
			c = readChar();
			
			// Need to read at least one
			if (isNumber(c)) {
				while (true) {
					c = readChar();
					if (!isNumber(c)) { break; }
				}
			}
			else {
				unread();
			}
		}
		
		// This isn't very correct but whatever for now
		if (c == 'e' || c == 'E') {
			c = readChar();
			
			// Optional + or -
			if (c == '+' || c == '-') { c = readChar(); }
			
			if (isNumber(c)) {
				while (true) {
					c = readChar();
					if (!isNumber(c)) { break; }
				}
			}
			else {
				unread();
			}
		}
		
		unread(); // Revert last char which doesn't count as a number
		accept(DG_SCRIPT_TOKEN_NUMBER);
	}
	else {
		// An operator or an error of some kind
		unread(); // need to unread unknown char
		
		const char *remaining = DgScriptLexerRemainingString(this);
		
		DgLog(DG_LOG_VERBOSE, "remaining = '%s'", remaining);
		
		for (size_t i = 0; gScriptOps[i]; i++) {
			if (DgStringStartsWith(remaining, gScriptOps[i])) {
				DgLog(DG_LOG_VERBOSE, "matched %s", gScriptOps[i]);
				size_t size = DgStringLength(gScriptOps[i]);
				for (size_t j = 0; j < size; j++) { readChar(); }
				accept(DG_SCRIPT_TOKEN_OP);
			}
		}
	}
	
	// Cannot accept any token like that.
	return DG_SCRIPT_LEX_NOT_MATCHED;
	
	done: return DG_SCRIPT_LEX_OKAY;
}

static void DgScriptTokenPrint(DgScriptToken *this) {
	switch (this->type) {
		case DG_SCRIPT_TOKEN_BOOLEAN:
			DgLog(DG_LOG_VERBOSE, "boolean %d %d:%d", this->asBool, this->start, this->end);
			break;
		case DG_SCRIPT_TOKEN_ID:
			DgLog(DG_LOG_VERBOSE, "id      '%s' %d:%d", this->asText, this->start, this->end);
			break;
		case DG_SCRIPT_TOKEN_KEYWORD:
			DgLog(DG_LOG_VERBOSE, "keyword '%s' %d:%d", this->asText, this->start, this->end);
			break;
		case DG_SCRIPT_TOKEN_NIL:
			DgLog(DG_LOG_VERBOSE, "nil     %d:%d", this->start, this->end);
			break;
		case DG_SCRIPT_TOKEN_NUMBER:
			DgLog(DG_LOG_VERBOSE, "number  '%s' %d:%d", this->asText, this->start, this->end);
			break;
		case DG_SCRIPT_TOKEN_OP:
			DgLog(DG_LOG_VERBOSE, "op      %d %d:%d", this->asInt, this->start, this->end);
			break;
		case DG_SCRIPT_TOKEN_STRING:
			DgLog(DG_LOG_VERBOSE, "string  '%s' %d:%d", this->asText, this->start, this->end);
			break;
		default:
			DgLog(DG_LOG_VERBOSE, "error   %d:%d", this->start, this->end);
			break;
	}
}

void DgScript_LexString_Test(void) {
	DgScriptLexer lexer;
	const char *code = "1234 ( nil );\n\tx += y;\n\t\"string\".selectAll();";
	
	DgScriptLexerInit(&lexer, code);
	
	DgScriptToken curtoken;
	DgScriptLexerStatus status = DG_SCRIPT_LEX_OKAY;
	
	while (status == DG_SCRIPT_LEX_OKAY) {
		status = DgScriptLexerNextToken(&lexer, &curtoken);
		
		if (status == DG_SCRIPT_LEX_OKAY) {
			DgScriptTokenPrint(&curtoken);
		}
		else {
			DgLog(DG_LOG_VERBOSE, "lexing terminated with status %d", status);
		}
	}
}
