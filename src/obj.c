/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * OBJ Model Loader
 */

#include <string.h>
#include <inttypes.h>

#include "maths.h"
#include "str.h"
#include "alloc.h"
#include "log.h"
#include "fs.h"

#include "obj.h"

static uint8_t pushVertex(DgOBJMesh *mesh, DgVec3 v) {
	if (mesh->vertex_count >= mesh->vertex_alloc) {
		mesh->vertex_alloc = 4 + (2 * mesh->vertex_alloc);
		mesh->vertex = (DgVec3 *) DgRealloc(mesh->vertex, sizeof *mesh->vertex * mesh->vertex_alloc);
		
		if (!mesh->vertex) {
			return 1;
		}
	}
	
	mesh->vertex[mesh->vertex_count] = v;
	mesh->vertex_count++;
	
	return 0;
}

static uint8_t pushFace(DgOBJMesh *mesh, uint32_t a, uint32_t b, uint32_t c) {
	if (mesh->face_count >= mesh->face_alloc) {
		mesh->face_alloc = 4 + (2 * mesh->face_alloc);
		mesh->face = (uint32_t *) DgRealloc(mesh->face, sizeof *mesh->face * mesh->face_alloc);
		
		if (!mesh->face) {
			return 1;
		}
	}
	
	mesh->face_count += 3;
	
	mesh->face[mesh->face_count - 3] = a;
	mesh->face[mesh->face_count - 2] = b;
	mesh->face[mesh->face_count - 1] = c;
	
	return 0;
}

uint32_t DgOBJLoad(DgOBJMesh *mesh, char * const path) {
	/**
	 * Load an OBJ mesh.
	 */
	
	memset(mesh, 0, sizeof *mesh);
	
	char *real_path = DgEvalPath(path);
	
	if (!real_path) {
		return 1;
	}
	
	DgFileStream *f = DgFileStreamOpen(real_path, "rb");
	
	if (!f) {
		DgLog(DG_LOG_ERROR, "Failed to load OBJ mesh at %s: Failed to open file stream.", real_path);
		return 1;
	}
	
	size_t len;
	char *line;
	
	while (line = DgFileStreamGetString(f, &len)) {
		if (line[0] == 'v' && line[1] == ' ') {
			if (pushVertex(mesh, DgVec3FromString(&line[2]))) {
				DgLog(DG_LOG_ERROR, "Failed to load OBJ mesh at %s: Failed to push vertex.", real_path);
				return 2;
			}
		}
		else if (line[0] == 'f' && line[1] == ' ') {
			size_t sz;
			uint32_t *v = DgInt32ListFromString(&line[2], &sz);
			
			if (sz < 3) {
				DgLog(DG_LOG_ERROR, "Failed to load OBJ mesh at %s: Failed to push face.", real_path);
				return 2;
			}
			
			if (pushFace(mesh, v[0] - 1, v[1] - 1, v[2] - 1)) {
				DgLog(DG_LOG_ERROR, "Failed to load OBJ mesh at %s: Failed to push face.", real_path);
				return 2;
			}
		}
	}
	
	DgFileStreamClose(f);
	
	if (mesh->vertex_count < 1 || mesh->face_count < 1) {
		DgLog(DG_LOG_ERROR, "Failed to load OBJ mesh at %s: Have no faces or verticies.", real_path);
		return 3;
	}
	
	DgFree(real_path);
	
	return 0;
}

uint32_t DgOBJFree(DgOBJMesh *mesh) {
	/**
	 * Free an OBJ mesh.
	 */
	
	DgFree(mesh->vertex);
	DgFree(mesh->face);
	
	return 0;
}
