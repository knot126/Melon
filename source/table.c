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
 * Table/Dictionary type
 * 
 * @note This is implemented using an ordered hash table - that is, references
 * to the actual values are made in the hash map, which are then stored in an
 * array in the order they were inserted.
 */

#include "alloc.h"
#include "error.h"
#include "log.h"

#include "table.h"

/**
 * ---- QUICK LOOKUP TABLE ----
 * 
 * TODO: Expanding LUT when its getting full
 **/

static void DgTableLUTEntryInit(DgTableQuick *this) {
	/**
	 * Init a quick lookup entry.
	 * 
	 * @param this Quick lookup entry
	 */
	
	this->index = DG_TABLE_LUT_NIL;
	this->next = NULL;
}

static void DgTableLUTEntryFree(DgTableQuick *this) {
	/**
	 * Free a quick lookup entry.
	 * 
	 * @param this Quick lookup entry, Must be a root entry.
	 */
	
	// We start at the second since only those are alloc'd dynamically
	DgTableQuick *cur = this->next;
	
	while (cur) {
		DgTableQuick *cur_next = cur->next;
		DgMemoryFree(cur);
		cur = cur_next;
	}
}

static DgError DgTableQuickAdd(DgTableQuick *this, size_t index, size_t *depth) {
	/**
	 * Add an index to a linked list. Also returns depth info for use when
	 * deciding to remake the table with larger indexes.
	 * 
	 * @param this Quick lookup entry
	 * @param index Index to insert
	 * @param depth Output parameter for the number of entries that were
	 * traversed before either needing to make a new entry or finding a blank
	 * entry.
	 */
	
	// Traverse over each entry
	DgTableQuick *cur = this;
	depth[0] = 0;
	
	while (true) {
		depth[0]++;
		
		// If we found one with a NIL index, we can just use that
		if (cur->index == DG_TABLE_LUT_NIL) {
			cur->index = index;
			return DG_ERROR_SUCCESSFUL;
		}
		// Otherwise if the next is NULL we can append a new entry
		else if (cur->next == NULL) {
			// Allocate memory for it
			cur->next = DgMemoryAllocate(sizeof *cur->next);
			
			if (!cur->next) {
				return DG_ERROR_ALLOCATION_FAILED;
			}
			
			// Set values
			cur->next->index = index;
			cur->next->next = NULL;
			
			return DG_ERROR_SUCCESSFUL;
		}
		// Otherwise we try the next one
		else {
			cur = cur->next;
		}
	}
}

static DgError DgTableLUTInit(DgTable *this, size_t size) {
	/**
	 * Initialise the contents of the quick lookup table.
	 * 
	 * @note Normally I would try to corse initialisation into a normal routine
	 * (like a resize of an array) but in this case I feel like it's just
	 * simpler to make a seprate init routine. :P
	 * 
	 * @param this Table to initialise the contents for
	 * @param size Initial size of the lookup table, should be a power of two
	 * lest the entire hash table explode in your face.
	 * @return Any error that occured
	 */
	
	// Allocate memory for the table
	this->lookup = DgMemoryAllocate(sizeof *this->lookup * size);
	
	if (!this->lookup) {
		return DG_ERROR_ALLOCATION_FAILED;
	}
	
	// Init each entry
	for (size_t i = 0; i < size; i++) {
		DgTableLUTEntryInit(&this->lookup[i]);
	}
	
	// Set the size
	this->lookup_alloc = size;
	
	return DG_ERROR_SUCCESSFUL;
}

static size_t DgTableLUTTrimHash(DgTable *this, uint64_t hash) {
	/**
	 * Trim the hash to its proper size
	 */
	
	return (size_t) (hash & ((this->lookup_alloc) - 1));
}

static void DgTableLUTLogEntries(DgTable *this) {
	for (size_t i = 0; i < this->lookup_alloc; i++) {
		DgLog(DG_LOG_VERBOSE, "[%d] <0x%llx> 0x%llx <0x%llx>", i, &this->lookup[i], this->lookup[i].index, this->lookup[i].next);
	}
}

static size_t DgTableLUTIndexForKey(DgTable *this, DgValue *key) {
	/**
	 * Find the index of the entry with the given `key`
	 * 
	 * @param this Table to preform the lookup in
	 * @param key Key to look for
	 * @return Index if found, otherwise DG_TABLE_LUT_NIL
	 */
	
	// Get the hash and trim it to size
	size_t qt_index = DgTableLUTTrimHash(this, DgValueQuickHash(key));
	
	DgLog(DG_LOG_VERBOSE, "qt_index = %d", qt_index);
	
	// Traverse the lookup table for possible matches
	DgTableQuick *cur = &this->lookup[qt_index];
	
	DgTableLUTLogEntries(this);
	
	while (cur) {
		if (cur->index != DG_TABLE_LUT_NIL) {
			// see if the key at that index matches, if so return it
			DgValue *cand_key = DgArrayAt(&this->array, 2 * cur->index);
			
			if (cand_key == NULL) {
				DgLog(DG_LOG_ERROR, "candidate key is out of bounds for the array!!!??? index = %lld", 2 * cur->index);
			}
			
			if (DgValueEqual(cand_key, key)) {
				return cur->index;
			}
		}
		
		// Try the next one
		cur = cur->next;
	}
	
	return DG_TABLE_LUT_NIL;
}

static DgError DgTableLUTInsertIndexForKey(DgTable *this, DgValue *key, size_t index) {
	/**
	 * Insert an entry in the quick lookup table for the given key and index
	 * 
	 * @param this Table to insert key for
	 * @param key Key to insert into the table
	 * @param index Index to use for the key
	 * @return Error
	 */
	
	// Get the hash and trim it to size
	size_t qt_index = DgTableLUTTrimHash(this, DgValueQuickHash(key));
	size_t depth;
	
	return DgTableQuickAdd(&this->lookup[qt_index], index, &depth);
}

static void DgTableLUTFree(DgTable *this) {
	/**
	 * Free the quick lookup table
	 */
	
	for (size_t i = 0; i < this->lookup_alloc; i++) {
		DgTableLUTEntryFree(&this->lookup[i]);
	}
}

/**
 * ---- THE ACTUAL SHIT ----
 **/

DgError DgTableInit(DgTable *this) {
	/**
	 * Initialise a table
	 * 
	 * @param this Table object
	 * @return Error code
	 */
	
	DgError error;
	
	// Zero it all! (not needed ?)
	DgMemoryZero(this, sizeof *this);
	
	// Prepare the main array
	if ((error = DgArrayInit(&this->array))) {
		return error;
	}
	
	// Prepare the quick lookup table
	if ((error = DgTableLUTInit(this, 8))) {
		return error;
	}
	
	return DG_ERROR_SUCCESSFUL;
}

DgError DgTableFree(DgTable *this, bool deep) {
	/**
	 * Free a table
	 * 
	 * @param this Table object
	 * @return Error code
	 */
	
	DgError error;
	
	DgTableLUTFree(this);
	
	if ((error = DgArrayFree(&this->array, deep))) {
		return error;
	}
	
	return error;
}

bool DgTableHas(DgTable * restrict this, DgValue * restrict key) {
	/**
	 * Check if the table has an entry with the given key.
	 * 
	 * @param this Table to check in
	 * @param key Key to check for
	 * @return true if the table has an entry with `key`, false if not
	 */
	
	return DgTableLUTIndexForKey(this, key) != DG_TABLE_LUT_NIL;
}

DgError DgTablePut(DgTable * restrict this, DgValue * restrict key, DgValue * restrict value) {
	/**
	 * Put a key -> value pair into the table
	 * 
	 * @note This effectively takes ownership of the key and value. If you want
	 * them to be copied instead use DgTableSet().
	 * 
	 * @param this Table object
	 * @param key Key
	 * @param value Value
	 */
	
	DgError error;
	
	// Handle the case where key/value already exists
	if (DgTableHas(this, key)) {
		size_t index = DgTableLUTIndexForKey(this, key);
		
		// Place the value into its slot
		error = DgArrayPut(&this->array, 2 * index + 1, value);
		
		// Free the key, since we'll never be using it
		DgValueFree(key);
		
		// Return our error from earlier, if any
		if (error) {
			return error;
		}
	}
	
	// Handle the case where the key does not exist yet
	else {
		// Calculate the new index
		size_t index = DgTableLength(this);
		
		// Add the new index to the LUT
		if ((error = DgTableLUTInsertIndexForKey(this, key, index))) {
			return error;
		}
		
		// Append the new key/value pair
		if ((error = DgArrayAppend(&this->array, key))) {
			return error;
		}
		
		if ((error = DgArrayAppend(&this->array, value))) {
			return error;
		}
	}
	
	return DG_ERROR_SUCCESS;
// 	// Handle the case where key/value already exists
// 	size_t index = 0;
// 	
// 	if (DgTableFind(this, key, &index) == DG_ERROR_SUCCESSFUL) {
// 		// Free old value
// 		DgError status = DgValueFree(&this->value[index]);
// 		
// 		if (status != DG_ERROR_SUCCESSFUL) {
// 			return status;
// 		}
// 		
// 		// Set key and value
// 		this->value[index] = *value;
// 		
// 		// Free the key value used for search
// 		DgValueFree(key);
// 	}
// 	
// 	// Handle the case where the key does not exist yet
// 	else {
// 		// Preallocate more
// 		if (DgTablePreallocMore(this) == DG_ERROR_ALLOCATION_FAILED) {
// 			DgLog(DG_LOG_ERROR, "Allocation failed for table <0x%16x>", this);
// 			return DG_ERROR_ALLOCATION_FAILED;
// 		}
// 		
// 		// Set key and value
// 		this->key[this->length] = *key;
// 		this->value[this->length] = *value;
// 		
// 		// Increment length
// 		this->length++;
// 	}
// 	
// 	// Return success status
// 	return DG_ERROR_SUCCESSFUL;
}

DgValue *DgTableAt(DgTable * restrict this, DgValue * restrict key) {
	/**
	 * Get a direct pointer to the value assocaited with a key
	 * 
	 * @note Automatically frees the key (regardless if success or failure)
	 * 
	 * @param this Table object
	 * @param key Key (in)
	 * @param value Value (out)
	 */
	
	size_t index = DgTableLUTIndexForKey(this, key);
	
	if (index == DG_TABLE_LUT_NIL) {
		return NULL;
	}
	
	return DgArrayAt(&this->array, 2 * index + 1);
	
// 	size_t index = 0;
// 	
// 	DgError status = DgTableFind(this, key, &index);
// 	
// 	if (status == DG_ERROR_SUCCESSFUL) {
// 		value[0] = this->value[index];
// 	}
// 	
// 	DgValueFree(key);
// 	
// 	return status;
}

DgError DgTableRemove(DgTable * restrict this, const DgValue * const restrict key) {
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

DgError DgTablePairAt(DgTable * restrict this, size_t index, DgValue ** const restrict key, DgValue ** const restrict value) {
	/**
	 * Get direct pointers to the key and value entries at index. This is good
	 * for iteration.
	 * 
	 * @note This function this most useful for serialisation or iteration,
	 * since it will show you the preserved order of pairs.
	 * 
	 * @param this Table object
	 * @param index The index to get
	 * @param key The key for the index (or NULL to ignore)
	 * @param value The value for the index (or NULL to ignore)
	 */
	
	if (index >= DgTableLength(this)) {
		return DG_ERROR_OUT_OF_RANGE;
	}
	
	if (key) {
		key[0] = DgArrayAt(&this->array, 2 * index);
	}
	
	if (value) {
		value[0] = DgArrayAt(&this->array, 2 * index + 1);
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
	
	return DgArrayLength(&this->array) / 2;
}
