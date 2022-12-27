/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Thread abstraction
 */

#include <pthread.h>

#include "thread.h"

int DgThreadNew(DgThread* thread, DgThreadFunction func, DgThreadArg arg) {
	/**
	 * Create a thread object and start execution.
	 * 
	 * @param thread Thread object to use
	 * @param func Thread function to free
	 * @param arg Argument that will be passed to the thread function
	 * @return Integer status code, dependent on thread library
	 */
	
#ifndef _WIN32
	return pthread_create(&thread->_info, NULL, func, arg);
#endif
}

int DgThreadJoin(DgThread* thread) {
	/**
	 * Make the thread object join with the current thread
	 * 
	 * @param thread Thread object to free
	 * @return Integer status code, dependent on thread library
	 */
	
#ifndef _WIN32
	return pthread_join(thread->_info, NULL);
#endif
}
