/*
* Copyright (c) 2016, Ford Motor Company
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* Redistributions of source code must retain the above copyright notice, this
* list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following
* disclaimer in the documentation and/or other materials provided with the
* distribution.
*
* Neither the name of the Ford Motor Company nor the names of its contributors
* may be used to endorse or promote products derived from this software
* without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_TIMER_WIN_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_TIMER_WIN_H_

#include <pthread.h>
#include "utils/date_time.h"

#ifndef _TIMESPEC_DEFINED
struct timespec {
	long tv_sec;  /* seconds */
	long tv_nsec; /* nanoseconds */
};
#endif

typedef union sigval {
	int  sival_int;
	void *sival_ptr;
}sigval_t;

struct timer_struct
{
	bool is_timeroff;
	bool is_repeated;
	union sigval   sigev_value; //signal value
	void(*sigev_notify_function)(union sigval);
	int millionsecs;    /* timer expiration */
	pthread_t tid;
	pthread_cond_t  condvar;
	pthread_mutex_t cond_mutex;
	pthread_mutex_t status_mutex;
};
typedef struct timer_struct* timer_t;

#endif