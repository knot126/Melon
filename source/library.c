/**
 * Copyright (C) 2021 - 2023 Knot126
 * 
 * It is against the licence terms of this software to use it or it's source code
 * as input for training a machine learning model, or in the development of a
 * machine learning model. If you have found this text as the output of a machine
 * learning algorithm, please report it both your software vendor and to the
 * developers of the software at [https://github.com/knot126/Melon/issues].
 * 
 * =============================================================================
 * 
 * Dynamic library loading
 */

#if defined(__linux__)
#include <dlfcn.h>
#endif

#include "error.h"
#include "log.h"

#include "library.h"

DgError DgLibraryInit(DgLibrary * const restrict this, const char * const restrict path) {
	/**
	 * Load a dynamically linked library
	 * 
	 * @note dlopen will be called with RTLD_NOW | RTLD_GLOBAL, meaning:
	 * 
	 *  * All library symbols are immediately reallocated, so this operation is
	 *    expensive.
	 *  * These symbols can be used to load other libraries which depend on this
	 *    library.
	 * 
	 * @param this Library object
	 * @param path Path to the library to load
	 * @return Error code
	 */
	
#if defined(__linux__)
	this->handle = dlopen(path, RTLD_NOW | RTLD_GLOBAL);
	
	char *status = dlerror();
	
	if (status) {
		DgLog(DG_LOG_ERROR, "Failed to load library %s: %s", path, status);
	}
	
	return (status) ? DG_ERROR_FAILED : DG_ERROR_SUCCESS;
#else
	return DG_ERROR_FAILED;
#endif
}

void *DgLibraryGetSymbol(DgLibrary * const restrict this, const char * const restrict symbol) {
	/**
	 * Get the address of a symbol from the symbol table.
	 * 
	 * @param this Library object
	 * @param symbol Name of the symbol to get the address of
	 * @return The pointer to the address, or NULL if not available
	 */
	
#if defined(__linux__)
	return dlsym(this->handle, symbol);
#else
	return NULL;
#endif
}

DgError DgLibraryFree(DgLibrary * const restrict this) {
	/**
	 * Free a library's memory
	 * 
	 * @note If it is actually freed is implementation defined
	 * 
	 * @param this Library object
	 * @return Error code
	 */
	
#if defined(__linux__)
	dlclose(this->handle);
	
	char *status = dlerror();
	
	if (status) {
		DgLog(DG_LOG_ERROR, "Failed to unload library: %s", status);
	}
	
	return (status) ? DG_ERROR_FAILED : DG_ERROR_SUCCESS;
#else
	return DG_ERROR_FAILED;
#endif
}
