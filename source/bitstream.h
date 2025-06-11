/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Bit buffers, also functional as a single header library!
 */ 

// header
#if _DG_BITBUFFER_H_
#define _DG_BITBUFFER_H_

#include <inttypes.h>

typedef size_t (*DgBitStreamIOFunc)(void *context, );

typedef struct DgBitStream {
	void *context;
	union {
		DgBitStreamIOFunc func, read, write;
	};
	size_t head;
	size_t avail;
	size_t length;
	uint8_t buffer[];
} DgBitStream;

typedef DgBitStream DgBitReadStream;
typedef DgBitStream DgBitWriteStream;

#endif

// implementation
#ifdef DG_BIT_BUFFER_IMPLEMENTATION

void DgBitStreamInit(DgBitStream *self, size_t size) {
	memset(self, 0, sizeof *self + size);
}

void DgBitStreamSetContext(DgBitStream *self, void *context, DgBitStreamIOFunc func) {
	self->context = context;
	self->func = func;
}

void DgBitStreamFlush(DgBitStream *self, bool pad) {
	
}

#undef DG_BIT_BUFFER_IMPLEMENTATION
#endif
