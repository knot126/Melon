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
 * Bèzier surface core implementation and anything needed for them not provided
 * by the maths library.
 */

#pragma once

#include "maths.h"

DgVec3 DgBezCurveVec3(size_t n, DgVec3 *points, float u);
DgVec3 DgBezSurfVec3(size_t n, size_t m, DgVec3 *points, float u, float v);
