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
