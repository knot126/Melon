/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
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
