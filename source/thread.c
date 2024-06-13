/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Thread abstraction
 */

#ifndef _WIN32
	#include <pthread.h>
#endif

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
#else
	return 1;
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
#else
	return 1;
#endif
}
