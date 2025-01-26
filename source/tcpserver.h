/**
 * Melon Software Framework is Copyright (C) 2021 - 2025 Knot126
 * 
 * =============================================================================
 * 
 * Basic mutli-threaded TCP server framework
 */

#pragma once

#include "tcpsocket.h"

struct DgTCPServerRequest;
typedef void (*DgTCPServerCallback)(struct DgTCPServerRequest *request);

typedef struct DgTCPServer {
	DgTCPSocket socket;
	DgTCPServerCallback handler;
	void *context;
	bool should_exit;
} DgTCPServer;

typedef struct DgTCPServerRequest {
	DgTCPServer *server;
	DgTCPSocket socket;
	void *context;
} DgTCPServerRequest;

DgError DgTCPServerInit(DgTCPServer *this, const char *host, uint16_t port, DgTCPServerCallback handler, void *context);
void DgTCPServerRun(DgTCPServer *this);
void DgTCPServerFree(DgTCPServer *this);
