/*
 * xxHash - Extremely Fast Hash algorithm
 * Copyright (C) 2012-2023 Yann Collet
 *
 * BSD 2-Clause License (https://www.opensource.org/licenses/bsd-license.php)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following disclaimer
 *      in the documentation and/or other materials provided with the
 *      distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * You can contact the author at:
 *   - xxHash homepage: https://www.xxhash.com
 *   - xxHash source repository: https://github.com/Cyan4973/xxHash
 */

/*
 * xxhash.c instantiates functions defined in xxhash.h
 */


/* If we're using jemalloc or mimalloc, we need to provide our own malloc/free
 * functions to xxhash, or else we won't use the same heaps or get the
 * advantages of memory leak tracking, etc.
 */
#if defined(USE_JEMALLOC) || defined(USE_MIMALLOC)
#  define XXH_CUSTOM_MALLOC

#  include <stdlib.h>

#  ifndef CDECL
#    ifdef _WIN32
#      define CDECL __cdecl
#    else
#      define CDECL
#    endif
#  endif

#  ifdef __cplusplus
extern "C" {
#  endif
	extern void *CDECL internal_alloc(size_t count);
	extern void CDECL internal_free(void *s);

	inline void *XXH_malloc(size_t s) { return internal_alloc(s); }
	inline void XXH_free(void *s) { internal_free(s); }
#  ifdef __cplusplus
}
#  endif

#endif

/* Ensure that we have a vectorization target */
#if defined(_M_ARM64) || defined(_M_ARM) || defined(__aarch64__) || defined(__arm__)
#  define XXH_VECTOR XXH_NEON
#elif defined(__SSE2__) || defined(_M_X64) || defined(_M_IX86) || defined(__i386__) || defined(__x86_64__)
#  define XXH_VECTOR XXH_SSE2
#endif

#define XXH_STATIC_LINKING_ONLY /* access advanced declarations */
#define XXH_IMPLEMENTATION      /* access definitions */

#include "xxhash.h"
