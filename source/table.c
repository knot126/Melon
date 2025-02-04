/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Table/Dictionary type
 * 
 * @note This is implemented using an ordered hash table - that is, references
 * to the actual values are made in the hash map, which are then stored in an
 * array in the order they were inserted.
 */

#include "memory.h"
#include "error.h"
#include "log.h"
#include "value.h"

#include "table.h"

#define NOT_FOUND ((size_t) -1)

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
	
	if ((error = DgArrayFree(&this->array, deep))) {
		return error;
	}
	
	return error;
}

static size_t DgTableIndexForKey(DgTable * restrict this, const DgValue * restrict key) {
	/**
	 * TODO: optimising this using some kind of hash table, preferably better
	 * than my first attempt at doing so...
	 */
	
	size_t len = DgTableLength(this);
	
	for (size_t i = 0; i < len; i++) {
		if (DgValueEqual(key, DgArrayAt(&this->array, 2 * i))) {
			return i;
		}
	}
	
	return NOT_FOUND;
}

bool DgTableHas(DgTable * restrict this, DgValue * restrict key) {
	/**
	 * Check if the table has an entry with the given key.
	 * 
	 * @param this Table to check in
	 * @param key Key to check for
	 * @return true if the table has an entry with `key`, false if not
	 */
	
	return DgTableIndexForKey(this, key) != NOT_FOUND;
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
	
	size_t index = DgTableIndexForKey(this, key);
	
	// Handle the case where key/value already exists
	if (index != NOT_FOUND) {
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
		// Append the new key/value pair
		if ((error = DgArrayAppend(&this->array, key))) {
			return error;
		}
		
		if ((error = DgArrayAppend(&this->array, value))) {
			return error;
		}
	}
	
	return DG_ERROR_SUCCESS;
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
	
	DgValue *value = NULL;
	
	size_t index = DgTableIndexForKey(this, key);
	
	if (index != NOT_FOUND) {
		value = DgArrayAt(&this->array, 2 * index + 1);
	}
	
	DgValueFree(key);
	
	return value;
}

DgError DgTableRemove(DgTable * restrict this, DgValue * restrict key) {
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
	
	size_t index = DgTableIndexForKey(this, key);
	
	DgValueFree(key);
	
	if (index == NOT_FOUND) {
		return DG_ERROR_NOT_FOUND;
	}
	
	return DgArrayRemoveND(&this->array, 2 * index, 2, true);
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

DgError DgTableSetPointer(DgTable * restrict this, const char *key, void *value) {
	DgValue k = DgMakeString(key);
	DgValue v = DgMakePointer(value);
	return DgTablePut(this, &k, &v);
}

void *DgTableGetPointer(DgTable * restrict this, const char *key) {
	DgValue k = DgMakeStaticString(key);
	DgValue *v = DgTableAt(this, &k);
	
	if (!v) { return NULL; }
	
	DgValueType t = DgValueGetType(v);
	
	if (t != DG_TYPE_POINTER) {
		return NULL;
	}
	
	return v->data.asPointer;
}
