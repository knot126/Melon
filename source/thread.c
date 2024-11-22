/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Thread abstraction
 */

#ifndef _WIN32
	#include <threads.h>
#endif

#include "memory.h"

#include "thread.h"

DgError DgThreadInit(DgThread *this, DgThreadFunction func, void *arg) {
	/**
	 * Create a thread object which can be started with func and arg.
	 * 
	 * @param this Thread object to use
	 * @param func Function to run on thread
	 * @param arg Argument that will be passed to the thread function
	 * @return Error code
	 */
	
	DgMemoryZero(this, sizeof *this);
	
	this->function = func;
	this->argument = arg;
	
	return DG_ERROR_SUCCESS;
}

DgError DgThreadInitWithPrototype(DgThread *this, DgThread *prototype) {
	/**
	 * Create a thread object which uses the prototype for the function and
	 * argument if they are not NULL.
	 * 
	 * @param this Thread object to use
	 * @param prototype Prototype thread object
	 * @return Error code
	 */
	
	return DgThreadInit(this, prototype->function, prototype->argument);
}

DgError DgThreadStart(DgThread *this) {
	/**
	 * Start the thread.
	 * 
	 * @param this Thread to start
	 * @return Error code
	 */
	
	return (thrd_create(&this->native_thread, (thrd_start_t) this->function, this->argument) == thrd_success) ? DG_SUCCESS : DG_FAIL;
}

DgError DgThreadStartWithArgument(DgThread *this, void *argument) {
	/**
	 * Start a thread while also setting the argument.
	 * 
	 * @param this Thread to start
	 * @param argument Value to set argument to
	 * @return Error code
	 */
	
	this->argument = argument;
	return DgThreadStart(this);
}

DgError DgThreadAwait(DgThread *this) {
	/**
	 * Wait on the thread to finish execution
	 * 
	 * @param this Thread object to wait upon
	 * @return Error code
	 */
	
	return (thrd_join(this->native_thread, NULL) == thrd_success) ? DG_SUCCESS : DG_FAIL;
}

void DgThreadFree(DgThread *this) {
	/**
	 * Free any assocaited resources with the given thread object
	 * 
	 * @param this Thread object
	 */
	
	return;
}

void DgThreadYield(void) {
	/**
	 * Give a hint that the current thread should yeild execution to another
	 * thread.
	 */
	
	thrd_yield();
}

void DgThreadExit(void) {
	/**
	 * Exit the current thread.
	 */
	
	thrd_exit(0);
}

DgError DgLockInit(DgLock *this) {
	/**
	 * Initialise a new lock
	 * 
	 * @param this Lock to initialise
	 * @return Error code
	 */
	
	return (mtx_init(&this->native_lock, mtx_plain) == thrd_success) ? DG_SUCCESS : DG_FAIL;
}

DgError DgLockLock(DgLock *this) {
	/**
	 * Lock a lock, awaiting its availability
	 * 
	 * @param this Lock object
	 * @return Error code
	 */
	
	return (mtx_lock(&this->native_lock) == thrd_success) ? DG_SUCCESS : DG_FAIL;
}

bool DgLockTryToLock(DgLock *this) {
	/**
	 * Try to lock a lock, returning false if locking fails.
	 * 
	 * @param this Lock object
	 * @return true if the lock was successfully accquired
	 */
	
	return (mtx_trylock(&this->native_lock) == thrd_success);
}

DgError DgLockUnlock(DgLock *this) {
	/**
	 * Unlock a locked lock
	 * 
	 * @param this Lock to unlock
	 * @return Error code
	 */
	
	return (mtx_unlock(&this->native_lock) == thrd_success) ? DG_SUCCESS : DG_FAIL;
}

void DgLockFree(DgLock *this) {
	/**
	 * Frees resources assocaited with a lock
	 */
	
	mtx_destroy(&this->native_lock);
}
