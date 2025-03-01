/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Bit oriented streams
 */

#include "memory.h"
#include "error.h"

#include "stream.h"
#include "bitstream.h"

void DgBitStreamInit(DgBitStream *this, DgStream *base) {
	/**
	 * Initialise a bit stream backed by an abstract byte stream.
	 */
	
	DgMemoryZero(this, sizeof *this);
	this->base = base;
}

DgError DgBitStreamFinish(DgBitStream *this) {
	/**
	 * Finish writing any queued bytes, adding padding zeros if needed, but
	 * don't close the underlying stream.
	 */
	
	if (this->amount_queued) {
		DgError err = DgStreamWriteUInt8(this->base, this->queued);
		this->amount_queued = 0;
		return err;
	}
	
	return DG_ERROR_SUCCESS;
}

DgError DgBitStreamClose(DgBitStream *this) {
	/**
	 * Finish the bit stream and close the underlying byte stream.
	 */
	
	DgError e = DgBitStreamFinish(this);
	
	if (e) {
		return e;
	}
	
	DgStreamClose(this->base);
	
	return DG_ERROR_SUCCESS;
}

DgError DgBitStreamWriteBit(DgBitStream *this, bool bit) {
	/**
	 * Write a single bit to the bit stream.
	 * 
	 * @warning Reads and writes cannot be intermixed on bit streams.
	 * 
	 * @param this Bit stream
	 * @param bit Bit value
	 * @return Error writing, if any
	 */
	
	this->queued |= bit << (7 - this->amount_queued++);
	
	if (this->amount_queued >= 8) {
		DgError err = DgStreamWriteUInt8(this->base, this->queued);
		this->amount_queued = 0;
		return err;
	}
	
	return DG_ERROR_SUCCESS;
}

DgError DgBitStreamReadBit(DgBitStream *this, bool *bit) {
	/**
	 * Read a single bit from the bit stream.
	 * 
	 * @warning Reads and writes cannot be intermixed on bit streams.
	 * 
	 * @param this Bit stream
	 * @param bit Bit value
	 * @return Error if there was a problem reading the bit
	 */
	
	if (!this->amount_queued_in) {
		DgError e = DgStreamReadUInt8(this->base, &this->queued_in);
		if (e) { return e; }
		this->amount_queued_in = 8;
	}
	
	*bit = (this->queued_in >> (--this->amount_queued_in)) & 1;
	
	return DG_ERROR_SUCCESS;
}
