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
}

void DgMelonFree(void) {
	/**
	 * Clear any global state that is used by Melon.
	 */
	
	return;
}
