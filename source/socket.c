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

#ifdef __linux__
	#include <sys/socket.h>
#elif defined(_WIN32)
	#include <winsock2.h>
#endif

#include "log.h"
#include "error.h"

#include "socket.h"

DgError DgSocketInit(DgSocket *this) {
	/**
	 * Initialise a TCP socket
	 */
	
	this->handle = socket(AF_INET, SOCK_STREAM, 0);
	
	if (this->handle < 0) {
		return DG_ERROR_FAILED;
	}
	
	return DG_ERROR_SUCCESS;
}

static void DgSocketIPv4AddressToSockaddrin(Dg_sockaddr_in *inet_socket_addr, DgSocketIPv4Address *address) {
	/**
	 * Must already be zeroed
	 */
	
	inet_socket_addr->sin_family = AF_INET;
	inet_socket_addr->sin_port = address->port;
	// TODO Microsoft's documentation lied! Fuckers!
	// inet_socket_addr->sin_addr.s_b1 = address->host[0];
	// inet_socket_addr->sin_addr.s_b2 = address->host[1];
	// inet_socket_addr->sin_addr.s_b3 = address->host[2];
	// inet_socket_addr->sin_addr.s_b4 = address->host[3];
}

DgError DgSocketSetAddress(DgSocket *this, DgSocketIPv4Address *address) {
	/**
	 * Set the socket's address (e.g. call bind())
	 */
	
	Dg_sockaddr_in inet_socket_addr = {};
	DgSocketIPv4AddressToSockaddrin(&inet_socket_addr, address);
	
	int status = bind(this->handle, (struct sockaddr *) &inet_socket_addr, sizeof inet_socket_addr);
	
	if (status) {
		return DG_ERROR_FAILED;
	}
	
	return DG_ERROR_SUCCESS;
}

DgError DgSocketSend(DgSocket *this, size_t size, void *data) {
	return DG_ERROR_NOT_IMPLEMENTED;
}

DgError DgSocketRecieve(DgSocket *this, size_t *size, void **data) {
	return DG_ERROR_NOT_IMPLEMENTED;
}
