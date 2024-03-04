/**
 * Copyright (C) 2021 - 2024 Knot126
 * 
 * It is against the licence terms of this software to use it or it's source code
 * as input for training a machine learning model, or in the development of a
 * machine learning model. If you have found this text as the output of a machine
 * learning algorithm, please report it both your software vendor and to the
 * developers of the software at [https://github.com/knot126/Melon/issues].
 * 
 * =============================================================================
 * 
 * Map/Table array type
 */

#include "alloc.h"
#include "error.h"
#include "log.h"

#include "table.h"

DgError DgTableInit(DgTable *this) {
	/**
	 * Initialise a table
	 * 
	 * @param this Table object
	 * @return Error code
	 */
	
	this->key = NULL;
	this->value = NULL;
	this->length = 0;
	this->allocated = 0;
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgTableFree(DgTable *this) {
	/**
	 * Free a table
	 * 
	 * @param this Table object
	 * @return Error code
	 */
	
	DgError error = DG_ERROR_SUCCESSFUL;
	
	for (size_t i = 0; i < this->length; i++) {
		DgError status = DgValueFree(&this->key[i]);
		
		if (status != DG_ERROR_SUCCESSFUL) {
			error = status;
		}
		
		status = DgValueFree(&this->value[i]);
		
		if (status != DG_ERROR_SUCCESSFUL) {
			error = status;
		}
	}
	
	DgMemoryFree(this->key);
	DgMemoryFree(this->value);
	
	return error;
}

static DgError DgTablePreallocMore(DgTable *this) {
	/**
	 * Preallocate more memory for the table. This must make sure at least one
	 * more space is available.
	 * 
	 * @param this Table object
	 * @return Error code
	 */
	
	if (this->length >= this->allocated) {
		this->allocated = 2 + (2 * this->allocated);
		
		this->key = DgMemoryReallocate(this->key, sizeof *this->key * this->allocated);
		this->value = DgMemoryReallocate(this->value, sizeof *this->value * this->allocated);
		
		if (this->key == NULL || this->value == NULL) {
			// TODO Memory leak!
			return DG_ERROR_ALLOCATION_FAILED;
		}
	}
	
	return DG_ERROR_SUCCESSFUL;
}

static DgError DgTableFind(DgTable * restrict this, const DgValue * restrict key, size_t * restrict index) {
	/**
	 * Find the index of the pair with the given key
	 * 
	 * @param this Table object
	 * @param key Key value
	 * @param index Pointer to write the index of the value if it exists (can be NULL)
	 * @return Error code
	 */
	
	for (size_t i = 0; i < this->length; i++) {
		if (DgValueEqual(&this->key[i], key)) {
			if (index) {
				index[0] = i;
			}
			return DG_ERROR_SUCCESSFUL;
		}
	}
	
	return DG_ERROR_NOT_FOUND;
}

DgError DgTableSet(DgTable * restrict this, DgValue * restrict key, DgValue * restrict value) {
	/**
	 * Set a key/value pair
	 * 
	 * @note This effectively frees the key and value (if successful).
	 * 
	 * @param this Table object
	 * @param key Key
	 * @param value Value
	 */
	
	// Handle the case where key/value already exists
	size_t index = 0;
	
	if (DgTableFind(this, key, &index) == DG_ERROR_SUCCESSFUL) {
		// Free old value
		DgError status = DgValueFree(&this->value[index]);
		
		if (status != DG_ERROR_SUCCESSFUL) {
			return status;
		}
		
		// Set key and value
		this->value[index] = *value;
		
		// Free the key value used for search
		DgValueFree(key);
	}
	
	// Handle the case where the key does not exist yet
	else {
		// Preallocate more
		if (DgTablePreallocMore(this) == DG_ERROR_ALLOCATION_FAILED) {
			DgLog(DG_LOG_ERROR, "Allocation failed for table <0x%16x>", this);
			return DG_ERROR_ALLOCATION_FAILED;
		}
		
		// Set key and value
		this->key[this->length] = *key;
		this->value[this->length] = *value;
		
		// Increment length
		this->length++;
	}
	
	// Return success status
	return DG_ERROR_SUCCESSFUL;
}

DgError DgTableGet(DgTable * restrict this, DgValue * restrict key, DgValue * restrict value) {
	/**
	 * Get a value assocaited with a key
	 * 
	 * @note Automatically frees the key (regardless if success or failure)
	 * 
	 * @param this Table object
	 * @param key Key
	 * @param value Value
	 */
	
	size_t index = 0;
	
	DgError status = DgTableFind(this, key, &index);
	
	if (status == DG_ERROR_SUCCESSFUL) {
		value[0] = this->value[index];
	}
	
	DgValueFree(key);
	
	return status;
}

DgError DgTableRemove(DgTable * restrict this, DgValue * const restrict key) {
	/**
	 * Remove an element from the table
	 * 
	 * @note This will return DG_ERROR_NOT_FOUND if the entry does not exist.
	 * Make sure to handle this correctly!
	 * 
	 * @param this Table object
	 * @param key The key assocaited with the entry to remove
	 * @return Error status
	 */
	
	return DG_ERROR_NOT_IMPLEMENTED;
}

DgError DgTableAt(DgTable * restrict this, size_t index, DgValue * const restrict key, DgValue * const restrict value) {
	/**
	 * Get the value at the given index.
	 * 
	 * @param this Table object
	 * @param index The index to get
	 * @param key The key for the index (or NULL to ignore)
	 * @param value The value for the index (or NULL to ignore)
	 */
	
	if (index >= this->length) {
		return DG_ERROR_NOT_FOUND;
	}
	
	if (key) {
		key[0] = this->key[index];
	}
	
	if (value) {
		value[0] = this->value[index];
	}
	
	return DG_ERROR_SUCCESSFUL;
}

size_t DgTableLength(DgTable * restrict this) {
	/**
	 * Get the length of the table
	 * 
	 * @param this Table to find the length of
	 * @return Length of the table
	 */
	
	return this->length;
}
