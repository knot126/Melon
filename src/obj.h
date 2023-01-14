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
 * OBJ Model Loader
 */

#pragma once

#include <inttypes.h>

typedef struct DgOBJMesh {
	DgVec3 *vertex;
	uint32_t *face;
	
	uint32_t vertex_count;
	uint32_t face_count;
	
	uint32_t vertex_alloc;
	uint32_t face_alloc;
} DgOBJMesh;

uint32_t DgOBJLoad(DgOBJMesh *mesh, char * const path);
uint32_t DgOBJFree(DgOBJMesh *mesh);
