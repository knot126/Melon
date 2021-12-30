/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Bèzier surface core implementation and anything needed for them not provided
 * by the maths library.
 */

#pragma once

#include "maths.h"

float DgCombination(float n, float k);
float DgBersteinPolynomial(float n, float i, float t);
DgVec3 DgBezCurveVec3(size_t n, DgVec3 *points, float u);
DgVec3 DgBezSurfVec3(size_t n, size_t m, DgVec3 *points, float u, float v);
