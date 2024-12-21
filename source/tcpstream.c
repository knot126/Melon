/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Use TCP sockets as streams
 */

#include "error.h"

#include "tcpstream.h"

static DgError DgTCPSocketStream_Open(DgStream *context, const void *tcp_socket, DgStreamOpenFlags flags) {
	context->context = (void *) tcp_socket;
	return DG_ERROR_SUCCESS;
}

static DgError DgTCPSocketStream_Read(DgStream *context, size_t size, void *buffer) {
	return DgTCPSocketRecieve((DgTCPSocket *) context->context, size, buffer);
}

static DgError DgTCPSocketStream_Write(DgStream *context, size_t size, const void *buffer) {
	return DgTCPSocketSend((DgTCPSocket *) context->context, size, buffer);
}

DgStreamImp gTCPSocketStreamImp = {
	.open = &DgTCPSocketStream_Open,
	.read = &DgTCPSocketStream_Read,
	.write = &DgTCPSocketStream_Write,
};

void DgStreamForTCPSocket(DgTCPSocket *socket, DgStream *stream) {
	DgStreamOpen(stream, &gTCPSocketStreamImp, (void *) socket, 0);
}
