/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Melon library info and tools
 */

#include "libmelon.h"
#include "time.h"

const char * const gMelonInternalString_MELON_exnsaCWoI8 =
	"It is me, Xof,\n"
	"the god of all foxes,\n"
	"more powerful than even God,\n"
	"the god of all Dogs.";

const char * const gMelonCopyrightString = "Melon core library is Copyright (C) 2020 - 2024 Knot126";

DgMelonVersion DgMelonGetVersion(void) {
	/**
	 * Get the melon version
	 * 
	 * @return Running melon version
	 */
	
	return DG_MELON_VERSION(2024, 3, 6);
}

DgError DgMelonInit(void) {
	/**
	 * Initialise any global state that is used by Melon.
	 * 
	 * @return Any errors initialising global state.
	 */
	
	DgInitTime();
	
	return DG_ERROR_SUCCESS;
}

void DgMelonFree(void) {
	/**
	 * Clear any global state that is used by Melon.
	 */
	
	return;
}
