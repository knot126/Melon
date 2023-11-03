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
 * Time Functions
 */

#include <time.h>
#include <inttypes.h>

#include "time.h"

static struct timespec st;

void DgInitTime(void) {
	timespec_get(&st, TIME_UTC);
}

double DgTime(void) {
	/**
	 * Returns the time in seconds since the program started.
	 * 
	 * @todo Should t.tv_nsec be (t.tv_nsec - st.tv_nsec) instead? Bug?
	 * 
	 * @return Time since DgInitTime was called
	 */
	
	struct timespec t;
	timespec_get(&t, TIME_UTC);
	
	return (double) (t.tv_sec - st.tv_sec) + (t.tv_nsec / 1000000000.0);
}

double DgRealTime(void) {
	/**
	 * Returns the current real time in seconds
	 * 
	 * @return Current time since 1970-01-01T00:00:00+00:00 in seconds as a double
	 */
	
	struct timespec t;
	timespec_get(&t, TIME_UTC);
	
	return (double) (t.tv_sec) + (t.tv_nsec / 1000000000.0);
}

uint32_t DgNsecTime(void) {
	/**
	 * Get the current nanosecond part of the time.
	 * 
	 * @return Nanosecond part of the current real time.
	 * 
	 * @deprecated Not very useful, some RNGs use this but they probably shouldn't.
	 */
	
	struct timespec t;
	timespec_get(&t, TIME_UTC);
	
	return t.tv_nsec;
}
