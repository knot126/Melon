/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * TCP sockets
 */

#pragma once

#include <inttypes.h>
#include <stdlib.h>

#include "memory_stream.h"

#ifdef __linux__
	#include <sys/socket.h>
#elif defined(_WIN32)
	#include <winsock2.h>
#endif

enum {
	DG_TCP_SOCKET_ANY_PORT = 0x10000,
};

typedef uint32_t DgTCPSocketStatus;
enum {
	DG_TCP_SOCKET_INITIALISING = (1 << 0),
	DG_TCP_SOCKET_READY = (1 << 1),
	DG_TCP_SOCKET_FAILED = (1 << 2),
	DG_TCP_SOCKET_READING = (1 << 3),
	DG_TCP_SOCKET_WRITING = (1 << 4),
};

typedef struct DgTCPSocket {
	DgMemoryStream *in;
	DgMemoryStream *out;
#ifdef __linux__
	int socket;
	bool connected;
#elif defined(_WIN32)
	SOCKET handle;
#endif
	DgTCPSocketStatus status;
} DgTCPSocket;
