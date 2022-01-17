/**
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Math Utilites: Vectors and Matricies
 * 
 * @warning This maths library will probably be replaced in the future.
 * 
 * For now here are some notes:
 * 
 *   * Passing arguments basically always happens by value, not by reference,
 *     except for when it does. This is fine for vec2 since it is the same size
 *     as a pointer on 64-bit platforms, but for other situations it means
 *     preformance issues.
 *   * Any of the matrix maths is probably not going to be any good. I forget
 *     how half of linear algebra works every month or so, which is of course
 *     annoying.
 *   * Rotations are in turns. 0 = no rotation and 1 = full rotation. This is
 *     the only advantage to this library, and it's really objective. Nicely,
 *     you can convert to rad/deg easily:
 *         rad = 2pi * turns = tau * turns
 *         deg = 360 * turns
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <inttypes.h>

// The use of tau over pi is debatable but for this tau works better
#define DG_MATHS_TAU 6.283185307179586476925286766559

#include "maths.h"

/*
 * Various angle operations
 */

float DgCos(float angle) {
	return (float) cos(angle * DG_MATHS_TAU);
}

float DgSin(float angle) {
	return (float) sin(angle * DG_MATHS_TAU);
}

float DgTan(float angle) {
	return (float) tan(angle * DG_MATHS_TAU);
}

float DgSqrt(float n) {
	return (float) sqrt(n);
}

int DgSign(float n) {
	if (n > 0.0f) {
		return 1;
	}
	else if (n == 0.0f) {
		return 0;
	}
	else {
		return -1;
	}
}

float xcos(float n) {
	return DgCos(n);
}

float xsin(float n) {
	return DgSin(n);
}

float xtan(float n) {
	return DgTan(n);
}

float xsqrt(float n) {
	return DgSqrt(n);
}

/**
 * Other misc. utility functions
 */

float xfac(float n) {
	const uint64_t num = (uint64_t) n;
	uint64_t res = 1;
	
	for (uint64_t i = 1; i <= num; i++) {
		res *= i;
	}
	
	return (float) res;
}

float xfloor(float n) {
	return (float)(int64_t)(n - 0.5);
}

float xpow(float n, float e) {
	return pow(n, e);
}

/* 
 * DgVec2 
 */

inline DgVec2 DgVec2Add(DgVec2 a, DgVec2 b) {
	DgVec2 c;
	
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	
	return c;
}

inline DgVec2 DgVec2Subtract(DgVec2 a, DgVec2 b) {
	DgVec2 c;
	
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	
	return c;
}

inline DgVec2 DgVec2Scale(float a, DgVec2 b) {
	DgVec2 c;
	
	c.x = a * b.x;
	c.y = a * b.y;
	
	return c;
}

inline DgVec2 DgVec2Multiply(DgVec2 a, DgVec2 b) {
	DgVec2 c;
	
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	
	return c;
}

inline float DgVec2Magnitude(DgVec2 a) {
	return sqrt((a.x * a.x) + (a.y * a.y));
}

inline float DgVec2Dot(DgVec2 a, DgVec2 b) {
	return (a.x * b.x) + (a.y * b.y);
}

inline float DgVec2RotDot(DgVec2 a, DgVec2 b) {
	/**
	 * Dot product combined with a 0.25 turn rotation. This will tell what "side"
	 * of vector a that b is on.
	 */
	
	return (a.x * b.y) - (a.y * b.x);
}

inline DgVec2 DgVec2Normalise(DgVec2 a) {
	DgVec2 c;
	
	float m = 1 / DgVec2Magnitude(a);
	
	c.x = a.x * m;
	c.y = a.y * m;
	
	return c;
}

inline DgVec2 DgVec2New(float x, float y) {
	/**
	 * @deprecated Use (DgVec2){x, y} or &(DgVec2){x, y} now.
	 */
	
	DgVec2 c;
	
	c.x = x;
	c.y = y;
	
	return c;
}

inline DgVec2 DgVec2FromString(const char * const s) {
	DgVec2 c = DgVec2New(0.0f, 0.0f);
	
	// Return zero vector on null string
	if (!s) {
		return c;
	}
	
	char * t = (char *) s;
	
	c.x = strtof(t, &t);
	if (t) {
		c.y = strtof(t, NULL);
	}
	
	return c;
}

/**
 * DgVec3 
 */

inline DgVec3 DgVec3Add(DgVec3 a, DgVec3 b) {
	DgVec3 c;
	
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	
	return c;
}

inline DgVec3 DgVec3Subtract(DgVec3 a, DgVec3 b) {
	DgVec3 c;
	
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	
	return c;
}

inline DgVec3 DgVec3Multiply(DgVec3 a, DgVec3 b) {
	DgVec3 c;
	
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
	
	return c;
}

inline DgVec3 DgVec3Scale(float a, DgVec3 b) {
	DgVec3 c;
	
	c.x = b.x * a;
	c.y = b.y * a;
	c.z = b.z * a;
	
	return c;
}

inline float DgVec3Magnitude(DgVec3 a) {
	if (a.x == 0.0f && a.y == 0.0f && a.z == 0.0f) {
		return 0.0f;
	}
	
	return (float) sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z));
}

inline float DgVec3Dot(DgVec3 a, DgVec3 b) {
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

inline DgVec3 DgVec3Cross(DgVec3 a, DgVec3 b) {
	DgVec3 c;
	
	c.x = (a.y * b.z) + (a.z * b.y);
	c.y = (a.z * b.x) + (a.x * b.z);
	c.z = (a.x * b.y) + (a.y * b.x);
	
	return c;
}

inline DgVec3 DgVec3Normalise(DgVec3 a) {
	DgVec3 c;
	
	float m = DgVec3Magnitude(a);
	
	if (m != 0.0f) {
		m = 1.0f / m;
	}
	
	c.x = a.x * m;
	c.y = a.y * m;
	c.z = a.z * m;
	
	return c;
}

inline DgVec3 DgVec3New(float x, float y, float z) {
	DgVec3 c;
	
	c.x = x;
	c.y = y;
	c.z = z;
	
	return c;
}

inline DgVec3 DgVec3Negate(DgVec3 a) {
	DgVec3 c;
	
	c.x = -a.x;
	c.y = -a.y;
	c.z = -a.z;
	
	return c;
}

inline DgVec3 DgVec3FromString(const char * const s) {
	DgVec3 c = DgVec3New(0.0f, 0.0f, 0.0f);
	
	// Return zero vector on null string
	if (!s) {
		return c;
	}
	
	char * t = (char *) s;
	
	c.x = strtof(t, &t);
	if (t) {
		c.y = strtof(t, &t);
		if (t) {
			c.z = strtof(t, NULL);
		}
	}
	
	return c;
}

inline DgVec3 DgVec3Rotate(DgVec3 base, DgVec3 rot) {
	/**
	 * Rotate the base vector by the given rotation vector.
	 * 
	 * @warning Try not to use this, instead use matricies.
	 */
	
	DgVec3 orig = base;
	
	// X-Rotation
	base.y = orig.y * DgCos(rot.x) + orig.z * DgSin(rot.x);
	base.z = orig.z * DgCos(rot.x) + orig.y * DgSin(rot.x);
	orig = base;
	
	// Y-Rotation
	base.x = orig.x * DgCos(rot.y) + orig.z * DgSin(rot.y);
	base.z = orig.z * DgCos(rot.y) + orig.x * DgSin(rot.y);
	orig = base;
	
	// Z-Rotation
	base.x = orig.x * DgCos(rot.z) + orig.y * DgSin(rot.z);
	base.y = orig.y * DgCos(rot.z) + orig.x * DgSin(rot.z);
	orig = base;
	
	return base;
}

inline DgVec3 DgVec3Lerp(float t, DgVec3 a, DgVec3 b) {
	/**
	 * Linearly interpolate two 3D vectors.
	 */
	
	return DgVec3Add(DgVec3Scale((1.0f - t), a), DgVec3Scale(t, b));
}

/**
 * DgVec4
 */

inline DgVec4 DgVec4Add(DgVec4 a, DgVec4 b) {
	DgVec4 c;
	
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	c.w = a.w + b.w;
	
	return c;
}

inline DgVec4 DgVec4Subtract(DgVec4 a, DgVec4 b) {
	DgVec4 c;
	
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	c.w = a.w - b.w;
	
	return c;
}

inline DgVec4 DgVec4Multiply(DgVec4 a, DgVec4 b) {
	DgVec4 c;
	
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
	c.w = a.w * b.w;
	
	return c;
}

inline DgVec4 DgVec4Scale(float a, DgVec4 b) {
	DgVec4 c;
	
	c.x = a * b.x;
	c.y = a * b.y;
	c.z = a * b.z;
	c.w = a * b.w;
	
	return c;
}

inline float DgVec4Magnitude(DgVec4 a) {
	return sqrt((a.x * a.x) + (a.y * a.y) + (a.z * a.z) + (a.w * a.w));
}

inline float DgVec4Dot(DgVec4 a, DgVec4 b) {
	return (a.x * b.x) + (a.y + b.y) + (a.z * b.z) + (a.w * b.w);
}

inline DgVec4 DgVec4Normalise(DgVec4 a) {
	DgVec4 c;
	
	float m = 1 / DgVec4Magnitude(a);
	
	c.x = a.x / m;
	c.y = a.y / m;
	c.z = a.z / m;
	c.w = a.w / m;
	
	return c;
}

inline DgVec4 DgVec4New(float x, float y, float z, float w) {
	DgVec4 c;
	
	c.x = x;
	c.y = y;
	c.z = z;
	c.w = w;
	
	return c;
}

/* 
 * DgMat4
 */

inline DgVec4 DgMat4ByVec4Multiply(DgMat4 a, DgVec4 b) {
	DgVec4 c;
	
	c.x = (b.x * a.ax) + (b.x * a.ay) + (b.x * a.az) + (b.x * a.aw);
	c.y = (b.y * a.bx) + (b.y * a.by) + (b.y * a.bz) + (b.y * a.bw);
	c.z = (b.z * a.cx) + (b.z * a.cy) + (b.z * a.cz) + (b.z * a.cw);
	c.w = (b.y * a.dx) + (b.y * a.dy) + (b.y * a.dz) + (b.y * a.dw);
	
	return c;
}

inline DgMat4 DgMat4ByMat4Multiply(DgMat4 a, DgMat4 b) {
	DgMat4 c;
	
	c.ax = (a.ax * b.ax) + (a.ay * b.bx) + (a.az * b.cx) + (a.aw * b.dx);
	c.ay = (a.ax * b.ay) + (a.ay * b.by) + (a.az * b.cy) + (a.aw * b.dy);
	c.az = (a.ax * b.az) + (a.ay * b.bz) + (a.az * b.cz) + (a.aw * b.dz);
	c.aw = (a.ax * b.aw) + (a.ay * b.bw) + (a.az * b.cw) + (a.aw * b.dw);
	
	c.bx = (a.bx * b.ax) + (a.by * b.bx) + (a.bz * b.cx) + (a.bw * b.dx);
	c.by = (a.bx * b.ay) + (a.by * b.by) + (a.bz * b.cy) + (a.bw * b.dy);
	c.bz = (a.bx * b.az) + (a.by * b.bz) + (a.bz * b.cz) + (a.bw * b.dz);
	c.bw = (a.bx * b.aw) + (a.by * b.bw) + (a.bz * b.cw) + (a.bw * b.dw);
	
	c.cx = (a.cx * b.ax) + (a.cy * b.bx) + (a.cz * b.cx) + (a.cw * b.dx);
	c.cy = (a.cx * b.ay) + (a.cy * b.by) + (a.cz * b.cy) + (a.cw * b.dy);
	c.cz = (a.cx * b.az) + (a.cy * b.bz) + (a.cz * b.cz) + (a.cw * b.dz);
	c.cw = (a.cx * b.aw) + (a.cy * b.bw) + (a.cz * b.cw) + (a.cw * b.dw);
	
	c.dx = (a.dx * b.ax) + (a.dy * b.bx) + (a.dz * b.cx) + (a.dw * b.dx);
	c.dy = (a.dx * b.ay) + (a.dy * b.by) + (a.dz * b.cy) + (a.dw * b.dy);
	c.dz = (a.dx * b.az) + (a.dy * b.bz) + (a.dz * b.cz) + (a.dw * b.dz);
	c.dw = (a.dx * b.aw) + (a.dy * b.bw) + (a.dz * b.cw) + (a.dw * b.dw);
	
	return c;
}

inline DgMat4 DgMat4Translate(DgMat4 a, DgVec3 b) {
	DgMat4 c = DgMat4New(1.0f);
	
	c.aw = b.x;
	c.bw = b.y;
	c.cw = b.z;
	
	a = DgMat4ByMat4Multiply(a, c);
	
	return a;
}

inline DgMat4 DgMat4Scale(DgMat4 a, DgVec3 b) {
	DgMat4 c = DgMat4New(1.0f);
	
	c.ax = b.x;
	c.by = b.y;
	c.cz = b.z;
	
	a = DgMat4ByMat4Multiply(a, c);
	
	return a;
}

inline DgMat4 DgMat4Rotate(DgMat4 a, DgVec3 b, float angle) {
	/**
	 * a = (the matrix that rotation will be applied to)
	 * b = (the axis that will be rotated upon)
	 */
	
	DgMat4 c = DgMat4New(1.0f);
	
	c.ax = DgCos(angle) + (b.x * b.x * (1 - DgCos(angle)));
	c.ay = (b.x * b.y * (1 - DgCos(angle))) - (b.z * DgSin(angle));
	c.az = (b.x * b.z * (1 - DgCos(angle))) + (b.y * DgSin(angle));
	
	c.bx = (b.y * b.x * (1 - DgCos(angle))) + (b.z * DgSin(angle));
	c.by = DgCos(angle) + (b.y * b.y * (1 - DgCos(angle)));
	c.bz = (b.y * b.z * (1 - DgCos(angle))) - (b.x * DgSin(angle));
	
	c.cx = (b.z * b.x * (1 - DgCos(angle))) - (b.y * DgSin(angle));
	c.cy = (b.z * b.y * (1 - DgCos(angle))) + (b.x * DgSin(angle));
	c.cz = DgCos(angle) + (b.z * b.z * (1 - DgCos(angle)));
	
	a = DgMat4ByMat4Multiply(a, c);
	
	return a;
}

inline DgMat4 DgMat4New(float a) {
	DgMat4 c;
	
	c.ax = a; c.ay = 0.0f; c.az = 0.0f; c.aw = 0.0f;
	c.bx = 0.0f; c.by = a; c.bz = 0.0f; c.bw = 0.0f;
	c.cx = 0.0f; c.cy = 0.0f; c.cz = a; c.cw = 0.0f;
	c.dx = 0.0f; c.dy = 0.0f; c.dz = 0.0f; c.dw = a;
	
	return c;
}

void DgMat4Print(DgMat4 a) {
	printf("⎡%f %f %f %f⎤\n", a.ax, a.ay, a.az, a.aw);
	printf("⎢%f %f %f %f⎥\n", a.bx, a.by, a.bz, a.bw);
	printf("⎢%f %f %f %f⎥\n", a.cx, a.cy, a.cz, a.cw);
	printf("⎣%f %f %f %f⎦\n", a.dx, a.dy, a.dz, a.dw);
}

/* 
 * Other misc. functions
 */

DgMat4 DgTransfromBasicCamera(DgVec3 trans, DgVec3 rot) {
	/**
	 * <summary>Brute-force camera transfrom in that (... I left the descript at that I guess)</summary>
	 * <input type="DgVec3" name="trans">The translation that will be applied to the camera.</input>
	 * <input type="DgVec3" name="rot">The rotation that will be applied to the camera.</input>
	 */
	
	DgMat4 pos = DgMat4Translate(DgMat4New(1.0f), DgVec3New(-trans.x, -trans.y, -trans.z));
	
	DgMat4 rot_x = DgMat4Rotate(DgMat4New(1.0f), DgVec3New(1.0f, 0.0f, 0.0f), rot.x);
	DgMat4 rot_y = DgMat4Rotate(DgMat4New(1.0f), DgVec3New(0.0f, 1.0f, 0.0f), rot.y);
	DgMat4 rot_z = DgMat4Rotate(DgMat4New(1.0f), DgVec3New(0.0f, 0.0f, 1.0f), rot.z);
	
	return DgMat4ByMat4Multiply(DgMat4ByMat4Multiply(rot_x, DgMat4ByMat4Multiply(rot_y, rot_z)), pos);
}

/*
 * Interpolation Schemes and Bèzier Curves
 */

DgVec3 DgVec3Bez3(float t, DgVec3 p0, DgVec3 p1, DgVec3 p2) {
	/**
	 * Compute a 3D quadratic bezier cruve.
	 * 
	 * These are the simplest "real" curves.
	 * 
	 * @deprecated New functions in surface.c replace this
	 */
	
	return DgVec3Lerp(t, DgVec3Lerp(t, p0, p1), DgVec3Lerp(t, p1, p2));
}

DgVec3 DgVec3Bez4(float t, DgVec3 p0, DgVec3 p1, DgVec3 p2, DgVec3 p3) {
	/**
	 * Compute a 3D cubic bezier cruve.
	 * 
	 * These are the simplest curves that look the best.
	 * 
	 * @deprecated New functions in surface.c replace this
	 */
	
	return DgVec3Lerp(t, DgVec3Lerp(t, DgVec3Lerp(t, p0, p1), DgVec3Lerp(t, p1, p2)), DgVec3Lerp(t, DgVec3Lerp(t, p1, p2), DgVec3Lerp(t, p2, p3)));
}

DgVec3 DgVec3BezN(float t, size_t length, DgVec3 * restrict points) {
	/**
	 * Compute the current point of an N-order 3D bezier curve. This will need to
	 * modify the list of control points.
	 * 
	 * @deprecated New functions in surface.c replace this
	 */
	
	while (length != 1) {
		for (size_t i = 0; i < length - 1; i++) {
			points[i] = DgVec3Lerp(t, points[i], points[i + 1]);
		}
		length--;
	}
	
	return *points;
}
