/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * TCP sockets
 */

#include "memory_stream.h"
#ifdef __linux__
	#include <sys/socket.h>
	#include <unistd.h>
	#include <netdb.h>
	#include <poll.h>
#elif defined(_WIN32)
	#include <winsock2.h>
#endif

#include "common.h"

#include "log.h"
#include "error.h"
#include "string.h"
#include "memory.h"
#include "bytes.h"

#include "tcpsocket.h"

DgError DgTCPSocketInit(DgTCPSocket *this, const char *host, uint32_t port, bool server) {
	/**
	 * Initialise a TCP client or server socket
	 */
	
	DgError error = DG_ERROR_SUCCESS;
	struct addrinfo *addresses = NULL;
	
	// Set server status
	this->server = server;
	this->socket = -1;
	
	// Port as a string
	char *port_str = DgIntegerToString(10, port);
	
	if (!port_str) {
		error = DG_ERROR_ALLOCATION_FAILED;
		goto fail;
	}
	
	// Translate host to address
	struct addrinfo hints = {
		.ai_family = AF_INET6,
		.ai_socktype = SOCK_STREAM,
		.ai_protocol = IPPROTO_TCP,
		.ai_flags = (server ? AI_PASSIVE : (AI_ADDRCONFIG | AI_V4MAPPED)) | AI_NUMERICSERV,
	};
	
	if (getaddrinfo(host, port_str, &hints, &addresses)) {
		error = DG_ERROR_NAME_LOOKUP_FAILED;
		goto fail;
	}
	
	// Open the socket
	this->socket = socket(addresses[0].ai_family, addresses[0].ai_socktype, addresses[0].ai_protocol);
	
	if (this->socket < 0) {
		error = DG_ERROR_FAILED;
		goto fail;
	}
	
	if (server) {
		// Bind and start listening (for server sockets)
		if (bind(this->socket, addresses[0].ai_addr, addresses[0].ai_addrlen)) {
			error = DG_ERROR_CONNECTION_FAILED;
			goto fail;
		}
		
		if (listen(this->socket, 128)) {
			error = DG_ERROR_CONNECTION_FAILED;
			goto fail;
		}
	}
	else {
		// Connect to the host (for client sockets)
		if (connect(this->socket, addresses[0].ai_addr, addresses[0].ai_addrlen)) {
			error = DG_ERROR_CONNECTION_FAILED;
			goto fail;
		}
	}
	
	// Free shit
fail:
	DgMemoryFree(port_str);
	
	if (addresses) {
		freeaddrinfo(addresses);
	}
	
	if (error != DG_ERROR_SUCCESS && this->socket >= 0) {
		close(this->socket);
	}
	
	return error;
}

DgError DgTCPSocketInitClient(DgTCPSocket *this, const char *host) {
	return DgTCPSocketInit(this, host, 12345, false);
}

DgError DgTCPSocketInitServer(DgTCPSocket *this, const char *host, uint16_t port) {
	return DgTCPSocketInit(this, host, port, true);
}

void DgTCPSocketFree(DgTCPSocket *this) {
	/**
	 * Free a Tcp socket
	 */
	
	shutdown(this->socket, SHUT_RDWR);
}

DgError DgTCPSocketAccept(DgTCPSocket *this, DgTCPSocket *new_socket) {
	/**
	 * Accept the next pending connection on the TCP socket, creating a new
	 * socket which can be used to talk to the client.
	 * 
	 * @param this Server socket
	 * @param new_socket Uninitialised socket which will be used to communicate
	 * with the client
	 * @return Error code
	 */
	
	if (this->server) {
		int new_fd = accept(this->socket, NULL, NULL);
		
		if (new_fd >= 0) {
			new_socket->socket = new_fd;
			new_socket->server = false;
			
			return DG_ERROR_SUCCESS;
		}
		else {
			return DG_ERROR_FAILED;
		}
	}
	else {
		return DG_ERROR_NOT_SUPPORTED;
	}
}

DgError DgTCPSocketSend(DgTCPSocket *this, size_t size, void *data) {
	/**
	 * Add `data` to the queue of data to send
	 * 
	 * @param this TCP Socket
	 * @param size Size of the data to send
	 * @param data Pointer to the data to send, should be at least size bytes
	 */
	
	size_t sent = 0;
	
	while (sent < size) {
		ssize_t sent_size = send(this->socket, data + sent, size - sent, 0);
		
		if (sent_size < 0) {
			return DG_ERROR_FAILED;
		}
		
		sent += sent_size;
	}
	
	return DG_ERROR_SUCCESS;
}

DgError DgTCPSocketRecieve(DgTCPSocket *this, size_t size, void *data) {
	/**
	 * Start recieving a given amount of data
	 * 
	 * @param this TCP Socket
	 * @param size Size of data to recieve
	 */
	
	size_t recieved = 0;
	
	while (recieved < size) {
		ssize_t recv_size = recv(this->socket, data + recieved, size - recieved, MSG_WAITALL);
		
		if (recv_size < 0) {
			return DG_ERROR_FAILED;
		}
		
		recieved += recv_size;
	}
	
	return DG_ERROR_SUCCESS;
}
