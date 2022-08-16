/**
 * Copyright (C) 2021 - 2022 Knot126
 * =================================
 * 
 * Error handling
 */

#include <stdbool.h>

#include "error.h"

bool DgErrorFatal(DgError error) {
	/**
	 * Return if the error is fatal or not.
	 */
	
	return (error != DG_ERROR_SUCCESSFUL);
}
