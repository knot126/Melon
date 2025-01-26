/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * TCP sockets
 */

#pragma once

#include <inttypes.h>
#include <stdlib.h>

#ifdef __linux__
	#include <sys/socket.h>
#elif defined(_WIN32)
	#include <winsock2.h>
#endif

typedef struct DgTCPSocket {
#ifdef __linux__
	int socket;
#elif defined(_WIN32)
	SOCKET socket;
#endif
	bool server;
} DgTCPSocket;

DgError DgTCPSocketInit(DgTCPSocket *this, const char *host, uint32_t port, bool server);
DgError DgTCPSocketInitClient(DgTCPSocket *this, const char *host, uint16_t port);
DgError DgTCPSocketInitServer(DgTCPSocket *this, const char *host, uint16_t port);
void DgTCPSocketFree(DgTCPSocket *this);
DgError DgTCPSocketAccept(DgTCPSocket *this, DgTCPSocket *new_socket);
DgError DgTCPSocketSend(DgTCPSocket *this, size_t size, const void *data);
DgError DgTCPSocketRecieve(DgTCPSocket *this, size_t size, void *data);
bool DgTCPSocketConnected(DgTCPSocket *this);
char *DgTCPSocketGetPeerAddressString(DgTCPSocket *this);
