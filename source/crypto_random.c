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
 * Truly random numbers
 */

#include "error.h"
#include "log.h"

#ifdef MELON_CRYPTOGRAPHY_RANDOM
#ifdef _WIN32
	#include <windows.h>
	/// @hack someone is an idiot
	#ifndef STATUS_SUCCESS
		#include <ntstatus.h>
	#endif
	#include <bcrypt.h>
	// The powers of fucking stupidity that are microsoft and windows developers
	// have come together to not only bring us not including NTSTATUS in bcrypt.h,
	// but also the fact that sometimes BCRYPT_RNG_ALG_HANDLE is just not fucking
	// defined sometimes.
	#ifndef BCRYPT_RNG_ALG_HANDLE
		#define BCRYPT_RNG_ALG_HANDLE ((BCRYPT_ALG_HANDLE) 0x00000081)
	#endif
#else
// ...
#endif

#include "crypto_random.h"

DgError DgRandom(size_t size, void *buffer) {
	/**
	 * Write `size` high quality random bytes to `buffer`.
	 * 
	 * @note By high quality random, we don't mean truly random, just random
	 * enough that it would qualify as cryptographically secure.
	 * 
	 * @comment knot126 I really can't believe how hard it is to generate a
	 * random number on a fucking platform that already exists. Are developers
	 * really expected to write secure code with this shit? I don't care if you
	 * *think* we need some special flags to control how things work, just give
	 * me a fucking function that puts some cryptographically secure bytes in a
	 * buffer without having to do some stupid API hacks or check if a file is a
	 * device file or if some crank replaced it with a gigabyte of zeros. Please
	 * just give a function that actually returns an error code and not some
	 * "it worked but I only wrote two of the 69 bytes you asked for" shit.
	 * I don't care how you internally implement things, I don't care if the low
	 * level shit is available - in fact, it should be - but it shouldn't be the
	 * only option. /rant
	 * 
	 * For windows:
	 * 
	 * @todo Does it really work, or is something wrong with it?
	 * 
	 * @see https://stackoverflow.com/questions/59241830/bcryptgenrandom-status-invalid-handle#63431748
	 * @see https://learn.microsoft.com/en-us/windows/win32/api/bcrypt/nf-bcrypt-bcryptgenrandom
	 * @see https://learn.microsoft.com/en-us/windows/win32/seccng/cng-algorithm-pseudo-handles
	 * 
	 * For linux:
	 * 
	 * @todo This needs to be implemented, but please avoid using /dev/(u)random
	 * if possible becuase that is a mess.
	 * 
	 * @see https://sockpuppet.org/blog/2014/02/25/safely-generate-random-numbers/
	 * @see https://www.2uo.de/myths-about-urandom/
	 * 
	 * @param size Number of random bytes to generate
	 * @param buffer Buffer to write bytes in
	 */
	
#ifdef _WIN32
	NTSTATUS status;
	
	status = BCryptGenRandom(BCRYPT_RNG_ALG_HANDLE, buffer, size, 0);
	
	return (status == STATUS_SUCCESS) ? (DG_ERROR_SUCCESS) : (DG_ERROR_FAILED);
#else
	DgLog(DG_LOG_FATAL, "High quality random number generator is not available on this platform.");
	
	return DG_ERROR_NOT_IMPLEMENTED;
#endif
}
#endif
