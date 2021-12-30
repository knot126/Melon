/*
 * Copyright (C) 2021 - 2022 Knot126 and Descentix Software
 * ========================================================
 * 
 * Time Functions
 */

#include <time.h>
#include <inttypes.h>

#include "time.h"

struct timespec st;

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
