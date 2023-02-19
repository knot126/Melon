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
 * Bit flag feilds
 */ 

#include "flags.h"

DgFlags DgFlagSet(DgFlags feild, DgFlags flag) {
	/**
	 * Set a flag in a flag feild
	 * 
	 * @param feild The flag feild to modify
	 * @param flag The flag to set
	 * @return The modified flag feild
	 */
	
	return feild | flag;
}

DgFlags DgFlagUnset(DgFlags feild, DgFlags flag) {
	/**
	 * Clear a flag in a flag feild
	 * 
	 * @param feild The flag feild to modify
	 * @param flag The flag to clear
	 * @return The modified flag feild
	 */
	
	return feild & (~flag);
}

DgFlags DgFlagToggle(DgFlags feild, DgFlags flag) {
	/**
	 * Invert a flag in a flag feild
	 * 
	 * @param feild The flag feild to modify
	 * @param flag The flag to invert
	 * @return The modified flag feild
	 */
	
	return feild ^ flag;
}

bool DgFlagCheckOr(DgFlags feild, DgFlags flags) {
	/**
	 * Check if any flag in flags is set
	 * 
	 * @param feild The flag feild to check from
	 * @param flag The flag to check
	 * @return If any flag is set (true) or not (false)
	 */
	
	return !!(feild & flags);
}

bool DgFlagCheckAnd(DgFlags feild, DgFlags flags) {
	/**
	 * Check if every flag in flags is set
	 * 
	 * @param feild The flag feild to check from
	 * @param flags The flags to check
	 * @return If every flag is set (true) or if at least one flag is not set (false)
	 */
	
	return (feild & flags) == flags;
}
