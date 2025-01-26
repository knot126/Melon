/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
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

typedef enum DgVMOpcode : uint8_t {
	// Extended arguments for opcode, similar to CPython's interpreter
	DG_VMOP_EXTEND = 0,
	
	// Variables and storage
	// DG_VMOP_STORE = 0x01, // Set variable
	// DG_VMOP_STOREGLOBAL, // Set global variable
	// DG_VMOP_LOAD, // Get variable
	// DG_VMOP_LOADGLOBAL, // Get global variable
	DG_VMOP_GET, // Get var
	DG_VMOP_SET, // Set var
	DG_VMOP_LOAD, // Load a const value
	
	// Flow control
	DG_VMOP_CALL, // Call a function
	DG_VMOP_PREFORM, // Call a method
	DG_VMOP_RETURN, // Return
	DG_VMOP_CMP, // Compare values
	DG_VMOP_BRANCH, // Branch
	
	// Operations
	DG_VMOP_ADD,
	DG_VMOP_SUBTRACT,
	DG_VMOP_MULTIPLY,
	DG_VMOP_DIVIDE,
	DG_VMOP_MODULO,
	DG_VMOP_POW,
	DG_VMOP_AND,
	DG_VMOP_OR,
	DG_VMOP_XOR,
	DG_VMOP_NOT,
	DG_VMOP_BITAND,
	DG_VMOP_BITOR,
	DG_VMOP_BITXOR,
	DG_VMOP_BITNOT,
	DG_VMOP_SHLEFT,
	DG_VMOP_SHRIGHT,
	// DG_VMOP_GET, // dict['test'], array[0]
	// DG_VMOP_PUT, // dict['test'] = 'test', array[0] = 'test'
	// DG_VMOP_DELETE, // dict.remove('test')
	// DG_VMOP_HAS, // dict.has('test')
} DgVMOpcode;

typedef enum DgVMCondFlags {
	DG_VMCOND_EQ = 0,
	DG_VMCOND_NEQ,
	DG_VMCOND_LT,
	DG_VMCOND_GTEQ,
	DG_VMCOND_GT,
	DG_VMCOND_LTEQ,
	DG_VMCOND_AL, // unconditional
};

typedef uint16_t DgVMInstruction;

typedef struct DgScriptClosure {
	DgVMInstruction *bytecode;
	size_t bytecode_size;
} DgScriptClosure;

// #define DG_MAKE_OPCODE_C(op, cn) ((op << 26) | (cn & 0x03ffffff))
#define DG_MAKE_OPCODE(op, cn) ((op << 10) | (cn & 0x3FF))
#define DG_PARSE_INSTR_OP(in) (in >> 10)
#define DG_PARSE_INSTR_ARG(in) (in & 0x3FF)

typedef struct DgVM {
	
} DgVM;
