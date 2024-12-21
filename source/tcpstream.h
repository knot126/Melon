/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Use TCP sockets as streams
 */

#pragma once

#include "common.h"
#include "tcpsocket.h"
#include "stream.h"

void DgStreamForTCPSocket(DgTCPSocket *socket, DgStream *stream);
