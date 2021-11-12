/*
 * Copyright (c) 2015, Aleksey Demakov
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
#include "pthr.h"

#ifdef __cplusplus
extern "C" {
#endif

#if !defined(PTHREAD_BARRIER_SERIAL_THREAD)
#define PTHREAD_BARRIER_SERIAL_THREAD (1)
#endif

	typedef struct {
	} pthr_barrierattr_t;

	typedef struct {
		pthr_mutex_t mutex;
		pthr_cond_t cond;
		unsigned int limit;
		unsigned int count;
		unsigned int phase;
	} pthr_barrier_t;

	int pthr_barrier_init(pthr_barrier_t *restrict barrier,
			const pthr_barrierattr_t *restrict attr, unsigned int count);

    int pthr_barrier_destroy(pthr_barrier_t *barrier);
    
	int pthr_barrier_wait(pthr_barrier_t *barrier);

#ifdef  __cplusplus
}
#endif
