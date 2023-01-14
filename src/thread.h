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
