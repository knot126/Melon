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
 * Math Utilites: Vectors and Matricies
 */

#pragma once

#include <stddef.h>
#include <inttypes.h>

// Trigonometry
float DgCos(float angle);
float DgSin(float angle);
float DgTan(float angle);

// other functions
float DgSqrt(float n);
int DgSign(float n);

// DgVec**

typedef struct DgVec2 {
	union {
		struct {
			float x, y;
		};
		struct {
			float u, v;
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
		struct {
			float r, g, b;
		};
		struct {
			float u, v, w;
		};
		float data[3];
	};
} DgVec3;

typedef DgVec3 DgColour3;
typedef DgVec3 DgBary3;

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
		uint32_t raw[4];
	};
} DgVec4;

typedef DgVec4 DgColour;
typedef DgVec4 DgColour4;

// DgVec2
DgVec2 DgVec2Add(DgVec2 a, DgVec2 b);
DgVec2 DgVec2Subtract(DgVec2 a, DgVec2 b);
DgVec2 DgVec2Scale(float a, DgVec2 b);
DgVec2 DgVec2Multiply(DgVec2 a, DgVec2 b);
float DgVec2Magnitude(DgVec2 a);
float DgVec2Dot(DgVec2 a, DgVec2 b);
float DgVec2RotDot(DgVec2 a, DgVec2 b);
DgVec2 DgVec2Normalise(DgVec2 a);

DgVec2 DgVec2FromString(const char * const s);
DgBary3 DgVec2Bary3(DgVec2 p1, DgVec2 p2, DgVec2 p3, DgVec2 point);

// DgVec3
DgVec3 DgVec3Add(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Subtract(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Multiply(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Scale(float a, DgVec3 b);
float DgVec3Magnitude(DgVec3 a);
float DgVec3Dot(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Cross(DgVec3 a, DgVec3 b);
DgVec3 DgVec3Normalise(DgVec3 a);
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

DgVec4 DgVec4Bary3Evaluate(float u, DgVec4 *a, float v, DgVec4 *b, float w, DgVec4 *c);

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
DgMat4 DgMat4Translate(DgMat4 a, DgVec3 b);
DgMat4 DgMat4Scale(DgMat4 a, DgVec3 b);
DgMat4 DgMat4Rotate(DgMat4 a, DgVec3 b, float angle);
DgMat4 DgMat4New(float a);
void DgMat4Print(DgMat4 a);

// Misc.
DgMat4 DgTransfromBasicCamera(DgVec3 trans, DgVec3 rot);

// Utility functions
float DgFloatMin3(float a, float b, float c);
float DgFloatMax3(float a, float b, float c);
