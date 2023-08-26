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
 * Raw network sockets
 */

#pragma once

#include <inttypes.h>
#include <stdlib.h>

#ifdef __linux__
	#include <sys/socket.h>
#elif defined(_WIN32)
	#include <winsock2.h>
#endif

typedef struct DgSocket {
#ifdef __linux__
	int handle;
#elif defined(_WIN32)
	SOCKET handle;
#endif
} DgSocket;

typedef struct DgSocketIPv4Address {
	char host[4];
	uint16_t port;
} DgSocketIPv4Address;

typedef struct sockaddr_in Dg_sockaddr_in;
typedef struct in_addr Dg_in_addr;
