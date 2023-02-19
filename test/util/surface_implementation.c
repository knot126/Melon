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
 * 
 * @note For general bezier curves, use util::surface
 * @todo This file should be merged or reorganised
 * @warning Do not include this file directly
 */

#if defined(DG_MELON_OLD_SURFACE)
#include "maths.h"
#include "log.h"

#include "surface_implementation.h"

static float DgFac(float n) {
	const uint64_t num = (uint64_t) n;
	uint64_t res = 1;
	
	for (uint64_t i = 1; i <= num; i++) {
		res *= i;
	}
	
	return (float) res;
}

static float DgFacBetween(float n, float m) {
	/**
	 * Compute factorial between two values. ( in range [n, m] )
	 * 
	 * @param n Lower bound
	 * @param m Upper bound
	 * @return Result
	 */
	
	float r = 1.0f;
	
	for (float j = m + 1.0f; j <= n; j += 1.0f) {
		r *= j;
	}
	
	return r;
}

static float DgCombination(float n, float k) {
	/**
	 * Calculate a binomial coefficent (same as a combination)
	 * 
	 * @param n n
	 * @param k k
	 * @return Result
	 * 
	 * @see https://mathworld.wolfram.com/BinomialCoefficient.html
	 * @see https://www.khanacademy.org/math/precalculus/x9e81a4f98389efdf:prob-comb/x9e81a4f98389efdf:combinations/v/introduction-to-combinations
	 */
	
// 	float res = xfac(n) / (xfac(k) * xfac(n - k));
// 	DgLog(DG_LOG_VERBOSE, "(%f %f) = %f", n, k, res);
	
	float res = DgFacBetween(n, n - k) / DgFac(k);
	return res;
}

static float DgBersteinPolynomial(float n, float i, float t) {
	/**
	 * Calculate a berstien basis polynomial.
	 * 
	 * @note This the basis function for the parametric curve called a Bezier
	 * curve, if you replace this with another type of basis function then you
	 * get other types of parametric curves. For example, this can be replaced
	 * with deBoor (I think?) and then there is a B-spline.
	 * 
	 * @see https://mathworld.wolfram.com/BernsteinPolynomial.html
	 */
	
	float res = DgCombination(n, i) * pow(1.0f - t, n - i) * pow(t, i);
// 	DgLog(DG_LOG_VERBOSE, "(%f %f) * (1 - %f)^(%f - %f) * %f^%f = %f", n, i, t, n, i, t, i, res);
	return res;
}

DgVec3 DgBezCurveVec3(size_t n, DgVec3 *points, float u) {
	/**
	 * Calculate an n-demensional bezier curve for a 3d object.
	 * 
	 * @note This will not be deprecated but it may be effectively superseeded
	 * by both better functions and implementations of the more flexible
	 * B-spline and NURBS curves and surfaces.
	 * 
	 * @param n Order of the curve
	 * @param points Control points of the curve
	 * @param u Parameter that is used to evaluate the point on the curve
	 * @return Point on the curve u of the way along the curve
	 */
	
	DgVec3 s = (DgVec3) {0.0f, 0.0f, 0.0f};
	
	for (size_t i = 0; i < n; i++) {
		s = DgVec3Add(s, DgVec3Scale(DgBersteinPolynomial(n - 1.0f, i, u), points[i]));
	}
	
	return s;
}

DgVec3 DgBezSurfVec3(size_t n, size_t m, DgVec3 *points, float u, float v) {
	/**
	 * Calculate the mapping from (u, v) to a point on a (n, m) order bezier
	 * surface.
	 * 
	 * @note This will not be deprecated but it may be effectively superseeded
	 * by both better functions and implementations of the more flexible
	 * B-spline and NURBS curves and surfaces.
	 * 
	 * @param n Order of the curve in u direction
	 * @param m Order of the curve in m direction
	 * @param points A 2D array of the points
	 * @param u Count of points in the u direction
	 * @param v Count of points in the v direction
	 * @return Point on the surface
	 * 
	 * @see https://en.wikipedia.org/wiki/B%C3%A9zier_surface
	 */
	
// 	for (size_t u = 0; u < n; u++) {
// 		for (size_t v = 0; v < m; v++) {
// 			printf("[(%d) * %d + %d = %d](%f %f %f)  ", n, u, v, (n * u) + v, points[(n * u) + v].x, points[(n * u) + v].y, points[(n * u) + v].z);
// 		}
// 		printf("\n");
// 	}
	
	DgVec3 mpoints[m];
	
	for (size_t j = 0; j < m; j++) {
		mpoints[j] = DgBezCurveVec3(n, &points[n * j], u);
	}
	
	DgVec3 result = DgBezCurveVec3(m, mpoints, v);
	
// 	if (result.x == 0.0f && result.y == 0.0f && result.z == 0.0f) {
// 		DgLog(DG_LOG_ERROR, "DgBezSurfVec3 is returning zero vector for u = %f and v = %f !!", u, v);
// 	}
	
	return result;
}
#endif
