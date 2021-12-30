/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
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
