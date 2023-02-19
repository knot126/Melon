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

DgError DgSocketInit(DgSocket *this, const char *address, uint16_t port) {
	/**
	 * 
	 */
	
	this->handle = socket(AF_INET, SOCK_STREAM, 0);
	
	if (this->handle < 0) {
		return DG_ERROR_FAILED;
	}
	
	return DG_ERROR_SUCCESS;
}

DgError DgSocketSend(DgSocket *this, size_t size, void *data) {
	
}

DgError DgSocketRecieve(DgSocket *this, size_t *size, void **data) {
	
}
