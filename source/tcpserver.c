/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Basic mutli-threaded TCP server framework
 */

#include "memory.h"
#include "thread.h"

#include "tcpserver.h"

DgError DgTCPServerInit(DgTCPServer *this, const char *host, uint16_t port, DgTCPServerCallback handler, void *context) {
	/**
	 * Create a TCP server
	 */
	
	DgMemoryZero(this, sizeof *this);
	
	DgError error = DgTCPSocketInitServer(&this->socket, host, port);
	
	if (error) {
		return error;
	}
	
	this->handler = handler;
	this->context = context;
	
	return DG_SUCCESS;
}

static void DgTCPServer_ThreadWrapper(DgTCPServerRequest *info) {
	/**
	 * Does TCPServer thread startup and shutdown
	 */
	
	info->server->handler(info);
	DgTCPSocketFree(&info->socket);
	DgMemoryFree(info);
}

void DgTCPServerRun(DgTCPServer *this) {
	/**
	 * Run the TCP server until an exit is requested
	 */
	
	while (!this->should_exit) {
		// Create request object
		DgTCPServerRequest *request = DgMemoryAllocate(sizeof *request);
		request->server = this;
		request->context = this->context;
		
		if (!request) {
			continue;
		}
		
		// Accept next connection
		DgError error = DgTCPSocketAccept(&this->socket, &request->socket);
		
		if (error) {
			DgMemoryFree(request);
			continue;
		}
		
		// Handle request in a new thread
		DgStartInNewThread((DgThreadFunction) &DgTCPServer_ThreadWrapper, request);
	}
}

void DgTCPServerFree(DgTCPServer *this) {
	/**
	 * Free resources assocaited with the TCP server
	 */
	
	DgTCPSocketFree(&this->socket);
}
