/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
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
		DgLog(DG_LOG_ERROR, "DgLibraryInit(): Failed to load library %s: %s", path ? path : "(self)", status);
	}
	
	return (status) ? DG_ERROR_FAILED : DG_ERROR_SUCCESS;
#else
	return DG_ERROR_NOT_IMPLEMENTED;
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
		DgLog(DG_LOG_ERROR, "DgLibraryFree(): Failed to unload library: %s", status);
	}
	
	return (status) ? DG_ERROR_FAILED : DG_ERROR_SUCCESS;
#else
	return DG_ERROR_NOT_IMPLEMENTED;
#endif
}

void *DgGetSymbol(const char * const symbol) {
	/**
	 * Get a function in the current program with the given symbol name.
	 * 
	 * @param symbol Name of the symbol
	 * @return Pointer to the symbol
	 */
	
	DgLibrary library;
	
	DgError error = DgLibraryInit(&library, DG_LIBRARY_OWN_MODULE);
	
	if (error) {
		return NULL;
	}
	
	void *func = DgLibraryGetSymbol(&library, symbol);
	
	error = DgLibraryFree(&library);
	
	if (error) {
		DgLog(DG_LOG_WARNING, "DgGetSymbol(): Could not unload main program handle.");
	}
	
	return func;
}
