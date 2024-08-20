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

#include "socket.h"

DgError DgTCPSocketInit(DgTCPSocket *this, const char *host, uint32_t port) {
	/**
	 * Initialise a TCP client socket
	 */
	
	this->in = NULL;
	this->out = NULL;
	
	// Open the socket
	this->socket = socket(AF_INET6, SOCK_STREAM, SOCK_NONBLOCK);
	
	if (this->socket < 0) {
		return DG_ERROR_FAILED;
	}
	
	// Port as a string
	char *port_str = DgIntegerToString(10, port);
	
	if (!port_str) {
		close(this->socket);
		return DG_ERROR_ALLOCATION_FAILED;
	}
	
	// Translate host to address
	struct addrinfo *connection_info;
	
	if (getaddrinfo(host, port_str, NULL, &connection_info)) {
		close(this->socket);
		return DG_ERROR_NAME_LOOKUP_FAILED;
	}
	
	struct sockaddr *address = connection_info->ai_addr;
	
	// Finally connect to the host
	if (connect(this->socket, address, sizeof *address)) {
		DgMemoryFree(port_str);
		freeaddrinfo(connection_info);
		return DG_ERROR_CONNECTION_FAILED; // TODO nonblocking
	}
	
	this->connected = false;
	
	// Free shit
	DgMemoryFree(port_str);
	freeaddrinfo(connection_info);
	
	return DG_ERROR_SUCCESS;
}

void DgTCPSocketFree(DgTCPSocket *this) {
	/**
	 * Free a Tcp socket
	 */
	
	close(this->socket);
}

static short DgTCPSocket_abstrct_poll(int socket) {
	/**
	 * 
	 */
	
	do {
		struct pollfd poll_info = {
			.fd = socket,
			.events = POLLIN | POLLPRI | POLLOUT,
			.revents = 0
		};
		
		int status = poll(&poll_info, 1, 0);
		
		if (status == -1 && errno != EINTR) {
			return POLLERR;
		}
		else if (status >= 0) {
			return poll_info.revents;
		}
	} while (errno == EINTR); // UNIX sucks
}

static bool DgTCPSocket_IsReady(DgTCPSocket *this, bool read, bool write) {
	/**
	 * Check that a socket is ready for reading and/or writing.
	 */
	
	short flags = DgTCPSocket_abstrct_poll(this->socket);
	
	return (!read || ((flags & POLLIN) == POLLIN)) && (!write || ((flags & POLLOUT) == POLLOUT)) && ((flags & POLLERR) != POLLERR);
}

static DgError DgTCPSocket_IsConnected(DgTCPSocket *this) {
	if (!this->connected && DgTCPSocket_IsReady(this, read, write)) {
		int status;
		socklen_t status_size = sizeof status;
		int gso_status = getsockopt(this->socket, SOL_SOCKET, SO_ERROR, &status, &status_size);
		
		if (gso_status || status) {
			return DG_ERROR_CONNECTION_FAILED;
		}
		else {
			this->connected = true;
			return DG_ERROR_SUCCESS;
		}
	}
	else if (!this->connected) {
		return DG_ERROR_WAITING;
	}
	else {
		return DG_ERROR_SUCCESS;
	}
}

DgTCPSocketStatus DgTCPSocketUpdate(DgTCPSocket *this) {
	/**
	 * Do the actual sending and recieving for this socket.
	 */
	
	DgError error;
	
	if ((error = DgTCPSocket_IsConnected(this))) {
		if (error == DG_ERROR_WAITING) {
			return DG_TCP_SOCKET_INITIALISING;
		}
		else {
			return DG_TCP_SOCKET_FAILED;
		}
	}
	
	DgTCPSocketStatus status = 0;
	bool failed = false;
	
	// Are we sending anything?
	if (this->out) {
		// Are we ready to send?
		if (DgTCPSocket_IsReady(this, false, true)) {
			size_t out_remain = DgMemoryStreamLength(this->out) - DgMemoryStreamGetpos(this->out);
			
			// Have we actually got anything to send?
			if (out_remain) {
				// Get a pointer to output buffer.
				void *out_buffer;
				DgMemoryStreamGetPointersAndSize(this->out, NULL, &out_buffer);
				out_buffer += DgMemoryStreamGetpos(this->out);
				
				// send() it!
				ssize_t sent = send(this->socket, out_buffer, out_remain, MSG_NOSIGNAL);
				
				if (sent == -1) {
					DgLog(DG_LOG_ERROR, "Failed to send(): %d", errno);
					failed = true;
				}
				
				// Set the pos for our output stream
				DgMemoryStreamSetpos(this->out, DG_MEMORY_STREAM_CUR, sent);
				
				status ^= DG_TCP_SOCKET_WRITING;
			}
			else {
				// Free the output stream as we'll not be needing it anymore
				DgMemoryStreamFree(this->out);
			}
		}
	}
	
	// Are we reading anything?
	if (this->in && DgTCPSocket_IsReady(this, true, false)) {
		status ^= DG_TCP_SOCKET_READING;
	}
	
	return failed ? DG_TCP_SOCKET_FAILED : (status ? status : DG_TCP_SOCKET_READY);
}

DgError DgTCPSocketSend(DgTCPSocket *this, size_t size, void *data) {
	/**
	 * Add `data` to the queue of data to send
	 * 
	 * @param this TCP Socket
	 * @param size Size of the data to send
	 * @param data Pointer to the data to send, should be at least size bytes
	 */
	
	this->out = DgMemoryStreamFromBuffer(data, size);
	
	return (this->out) ? DG_ERROR_SUCCESS : DG_ERROR_ALLOCATION_FAILED;
}

DgError DgTCPSocketRecieve(DgTCPSocket *this, size_t size) {
	/**
	 * Start recieving a given amount of data
	 * 
	 * @param this TCP Socket
	 * @param size Size of data to recieve
	 */
	
	return DG_ERROR_SUCCESS;
}

bool DgTCPSocketIdle(DgTCPSocket *this) {
	/**
	 * Check if there is anything for this socket to do at the moment
	 * 
	 * @param this TCP socket
	 */
	
	return !this->in && !this->out;
}
