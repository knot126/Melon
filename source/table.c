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
 **/

static void DgTableQuickInit(DgTableQuick *this) {
	/**
	 * Init a quick lookup entry.
	 * 
	 * @param this Quick lookup entry
	 */
	
	this->index = DG_TABLE_QUICK_NONE;
	this->next = NULL;
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
		if (cur->index == DG_TABLE_QUICK_NONE) {
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

static DgError DgTableQuickLookupInit(DgTable *this, size_t size) {
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
		DgTableLookupInit(&this->lookup[i]);
	}
	
	// Set the size
	this->lookup_alloc = size;
	
	return DG_ERROR_SUCCESSFUL;
}

static size_t DgTableQuickLookupTrimHash(DgTable *this, uint64_t hash) {
	/**
	 * Trim the hash to its proper size
	 */
	
	return (size_t) (hash & ((this->lookup_alloc << 1) - 1));
}

static size_t DgTableQuickLookupIndexForKey(DgTable *this, DgValue *key) {
	/**
	 * Find the index of the entry with the given `key`
	 * 
	 * @param this Table to preform the lookup in
	 * @param key Key to look for
	 * @return Index if found, otherwise DG_TABLE_QUICK_NONE
	 */
	
	// Get the hash and trim it to size
	size_t qt_index = DgTableQuickLookupTrimHash(this, DgValueQuickHash(key));
	
	// Traverse the lookup table for possible matches
	DgTableQuick *cur = &this->lookup[qt_index];
	
	while (cur) {
		if (cur->index != DG_TABLE_QUICK_NONE) {
			// see if the key value at that index matches, if so return it
			DgValue *value = DgArrayAt(&this->array);
			
			if (DgValueEqual(value, key)) {
				return cur->index;
			}
		}
		
		// Try the next one
		cur = cur->next;
	}
	
	return DG_TABLE_QUICK_NONE;
}

static DgError DgTableQuickInsertIndexForKey(DgTable *this, DgValue *key, size_t index) {
	/**
	 * Insert an entry in the quick lookup table for the given key and index
	 * 
	 * @param this Table to insert key for
	 * @param key Key to insert into the table
	 * @param index Index to use for the key
	 * @return Error
	 */
	
	// Get the hash and trim it to size
	size_t qt_index = DgTableQuickLookupTrimHash(this, DgValueQuickHash(key));
	size_t depth;
	
	return DgTableQuickAdd(&this->lookup[qt_index], index, &depth);
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
	
	if ((error = DgTableQuickLookupInit(this, 8))) {
		return error;
	}
	
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
		
		if (this->key == NULL) {
			return DG_ERROR_ALLOCATION_FAILED;
		}
		
		this->value = DgMemoryReallocate(this->value, sizeof *this->value * this->allocated);
		
		if (this->value == NULL) {
			DgMemoryFree(this->key);
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
