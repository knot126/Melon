/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * MelonScript
 */

#pragma once

#include "value.h"
#include "array.h"
#include "table.h"
#include "storage.h"

typedef enum DgScriptOpcode : uint8_t {
	// Variables and storage
	DG_SCRIPT_OPCODE_STORE = 1, // Set variable
	DG_SCRIPT_OPCODE_STOREGLOBAL, // Set global variable
	DG_SCRIPT_OPCODE_LOAD, // Get variable
	DG_SCRIPT_OPCODE_LOADGLOBAL, // Get global variable
	
	// Constants
	DG_SCRIPT_OPCODE_CONST, // Load a const value
	
	// Flow control
	DG_SCRIPT_OPCODE_RETURN, // Return
	DG_SCRIPT_OPCODE_CMP, // Compare values
	DG_SCRIPT_OPCODE_JUMP, // Unconditional jump
	DG_SCRIPT_OPCODE_JUMPEQ, // Jump if equal
	DG_SCRIPT_OPCODE_JUMPNEQ, // Jump if not equal
	DG_SCRIPT_OPCODE_JUMPLT, // Jump if less
	DG_SCRIPT_OPCODE_JUMPLE, // Jump if less or equal
	
	// Operations
	DG_SCRIPT_OPCODE_ADD,
	DG_SCRIPT_OPCODE_SUBTRACT,
	DG_SCRIPT_OPCODE_MULTIPLY,
	DG_SCRIPT_OPCODE_DIVIDE,
	DG_SCRIPT_OPCODE_MODULO,
	DG_SCRIPT_OPCODE_POW,
	DG_SCRIPT_OPCODE_AND,
	DG_SCRIPT_OPCODE_OR,
	DG_SCRIPT_OPCODE_XOR,
	DG_SCRIPT_OPCODE_NOT,
	DG_SCRIPT_OPCODE_BITAND,
	DG_SCRIPT_OPCODE_BITOR,
	DG_SCRIPT_OPCODE_BITXOR,
	DG_SCRIPT_OPCODE_BITNOT,
	DG_SCRIPT_OPCODE_SHLEFT,
	DG_SCRIPT_OPCODE_SHRIGHT,
	DG_SCRIPT_OPCODE_GET, // dict['test'], array[0]
	DG_SCRIPT_OPCODE_PUT, // dict['test'] = 'test', array[0] = 'test'
	DG_SCRIPT_OPCODE_DELETE, // dict.remove('test')
	DG_SCRIPT_OPCODE_HAS, // dict.has('test')
	DG_SCRIPT_OPCODE_CALL, // Call a function
} DgScriptOpcode;

typedef struct DgScriptClosure {
	uint8_t *bytecode;
	size_t bytecode_size;
} DgScriptClosure;
