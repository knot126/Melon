/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Thread abstraction
 */

#pragma once

#ifdef _WIN32
#error Platform not supported by threads yet!
#endif
#include <threads.h>

#include "error.h"

// First two typedefs may change depending on threading library
typedef void (*DgThreadFunction)(void *);

typedef struct DgThread {
	DgThreadFunction function;
	void *argument;
	thrd_t native_thread;
} DgThread;

DgError DgThreadInit(DgThread *this, DgThreadFunction func, void *arg);
DgError DgThreadInitWithPrototype(DgThread *this, DgThread *prototype);
DgError DgThreadStart(DgThread *this);
DgError DgThreadStartWithArgument(DgThread *this, void *argument);
DgError DgThreadAwait(DgThread *this);
void DgThreadFree(DgThread *this);

void DgThreadYield(void);
void DgThreadExit(void);

// Locks
typedef struct DgLock {
	mtx_t native_lock;
} DgLock;

DgError DgThreadJoin(DgThread* thread);
DgError DgLockInit(DgLock *this);
DgError DgLockLock(DgLock *this);
bool DgLockTryToLock(DgLock *this);
DgError DgLockUnlock(DgLock *this);
void DgLockFree(DgLock *this);
