/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Math Utilites: Vectors and Matricies
 */

#pragma once

#include <stddef.h>
#include <inttypes.h>

// Angle ops

float DgCos(float angle);
float DgSin(float angle);
float DgTan(float angle);
float DgSqrt(float n);

float xcos(float n);
float xsin(float n);
float xtan(float n);
float xsqrt(float n);
float xfac(float n);
float xpow(float n, float e);

// DgVec**

typedef struct DgVec2 {
	union {
		struct {
			float x, y;
		};
		float data[2];
	};
} DgVec2;

typedef struct DgVec2I {
	int x, y;
} DgVec2I;

typedef struct DgVec3 {
	union {
		struct {
			float x, y, z;
		};
		float data[3];
	};
} DgVec3;

typedef struct DgVec4 {
	union {
		struct {
			union {
				float x, r;
			};
			union {
				float y, g;
			};
			union {
				float z, b;
			};
			union {
				float w, a;
			};
		};
		float data[4];
	};
} DgVec4;

typedef DgVec4 DgColour;

// DgVec2

DgVec2 DgVec2Add(DgVec2 a, DgVec2 b);
DgVec2 DgVec2Subtract(DgVec2 a, DgVec2 b);
DgVec2 DgVec2Multiply(DgVec2 a, DgVec2 b);
float DgVec2Magnitude(DgVec2 a);
float DgVec2Dot(DgVec2 a, DgVec2 b);
DgVec2 DgVec2Normalise(DgVec2 a);
DgVec2 DgVec2New(float x, float y);
DgVec2 DgVec2FromString(const char * const s);

// DgVec3

DgVec3 DgVec3Add(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Subtract(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Multiply(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Scale(float a, DgVec3 b);
float DgVec3Magnitude(DgVec3 a);
float DgVec3Dot(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Cross(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Normalise(DgVec3 a);
DgVec3 DgVec3New(float x, float y, float z);
DgVec3 DgVec3Negate(DgVec3 a);

DgVec3 DgVec3FromString(const char * const s);
DgVec3 DgVec3Rotate(DgVec3 base, DgVec3 rot);

// DgVec4

DgVec4 DgVec4Add(DgVec4 a, DgVec4 b);
DgVec4 DgVec4Subtract(DgVec4 a, DgVec4 b);
DgVec4 DgVec4Multiply(DgVec4 a, DgVec4 b);
DgVec4 DgVec4Scale(float a, DgVec4 b);
float DgVec4Magnitude(DgVec4 a);
float DgVec4Dot(DgVec4 a, DgVec4 b);
DgVec4 DgVec4Normalise(DgVec4 a);
DgVec4 DgVec4New(float x, float y, float z, float w);

// DgMat**

typedef struct DgMat2 {
	union {
		struct {
			float ax, ay;
			float bx, by;
		};
		float data[2][2];
	};
} DgMat2;

typedef struct DgMat3 {
	union {
		struct {
			float ax, ay, az;
			float bx, by, bz;
			float cx, cy, cz;
		};
		float data[3][3];
	};
} DgMat3;

typedef struct DgMat4 {
	union {
		struct {
			float ax, ay, az, aw;
			float bx, by, bz, bw;
			float cx, cy, cz, cw;
			float dx, dy, dz, dw;
		};
		float data[4][4];
	};
} DgMat4;

// DgMat4

DgVec4 DgMat4ByVec4Multiply(DgMat4 a, DgVec4 b);
DgMat4 DgMat4ByMat4Multiply(DgMat4 a, DgMat4 b);
DgMat4 DgMat4Inverse(DgMat4 a);
DgMat4 DgMat4Translate(DgMat4 a, DgVec3 b);
DgMat4 DgMat4Scale(DgMat4 a, DgVec3 b);
DgMat4 DgMat4Rotate(DgMat4 a, DgVec3 b, float angle);
DgMat4 DgMat4NewPerspective(float l, float r, float b, float t, float n, float f);
DgMat4 DgMat4NewPerspective2(float fov, float rat, float f, float b);
DgMat4 DgMat4New(float a);
void DgMat4Print(DgMat4 a);

// Misc.

DgMat4 DgTransfromBasicCamera(DgVec3 trans, DgVec3 rot);

// Additional maths functions

DgVec3 DgVec3Lerp(float t, DgVec3 a, DgVec3 b);
DgVec3 DgVec3Bez3(float t, DgVec3 p0, DgVec3 p1, DgVec3 p2);
DgVec3 DgVec3Bez4(float t, DgVec3 p0, DgVec3 p1, DgVec3 p2, DgVec3 p3);
DgVec3 DgVec3BezN(float t, size_t length, DgVec3 * restrict points);
