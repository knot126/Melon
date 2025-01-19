/**
 * Melon Software Framework is Copyright (C) 2021 - 2024 Knot126
 * 
 * =============================================================================
 * 
 * Truly random numbers
 */

#include "error.h"
#include "log.h"

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
#elif defined(__linux__)
	#include <sys/random.h>
#else
// ...
#endif

#include "crypto.random.h"

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
	 * @see https://sockpuppet.org/blog/2014/02/25/safely-generate-random-numbers/
	 * @see https://www.2uo.de/myths-about-urandom/
	 * 
	 * getrandom() is basically using using /dev/urandom and is what I'm using
	 * now:
	 * 
	 * @see https://man7.org/linux/man-pages/man2/getrandom.2.html
	 * 
	 * @param size Number of random bytes to generate
	 * @param buffer Buffer to write bytes in
	 */
	
#ifdef _WIN32
	NTSTATUS status = BCryptGenRandom(BCRYPT_RNG_ALG_HANDLE, buffer, size, 0);
	
	return (status == STATUS_SUCCESS) ? DG_ERROR_SUCCESS : DG_ERROR_FAILED;
#elif defined(__linux__)
	size_t real_size = getrandom(buffer, size, 0);
	
	return (real_size == size) ? DG_ERROR_SUCCESS : DG_ERROR_FAILED;
#else
	DgLog(DG_LOG_FATAL, "High quality random number generator is not available on this platform.");
	
	return DG_ERROR_NOT_IMPLEMENTED;
#endif
}
