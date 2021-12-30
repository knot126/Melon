/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Thread abstraction
 */

#include <pthread.h>

// First two typedefs may change depending on threading library
typedef void *DgThreadArg;
typedef void *DgThreadReturn;

typedef DgThreadReturn (*DgThreadFunction)(DgThreadArg);

typedef struct DgThread {
	pthread_t _info;
} DgThread;

int DgThreadNew(DgThread* thread, DgThreadFunction func, DgThreadArg arg);
int DgThreadJoin(DgThread* thread);
