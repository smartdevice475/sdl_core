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

struct itimerspec {
	struct timespec it_interval; /* timer period */
	struct timespec it_value;    /* timer expiration */
};
//struct itimerval {
//	struct timeval it_interval; /* timer interval */
//	struct timeval it_value;    /* current value */
//};
typedef union sigval {
	int  sival_int;
	void *sival_ptr;
}sigval_t;

//typedef int clockid_t;
//
//#define SIGEV_NONE    0
//#define SIGEV_SIGNAL  1
//#define SIGEV_THREAD  2
//
//#define TIMER_ABSTIME  0
//struct sigevent
//{
//	int sigev_notify; //notification type
//	int sigev_signo; //signal number
//	union sigval   sigev_value; //signal value
//	void(*sigev_notify_function)(union sigval);
//	pthread_attr_t *sigev_notify_attributes;
//};

//struct timer_struct
//{
//	bool isTimerOff;
//	clockid_t clockid;
//	pthread_t tid;
//	struct sigevent sigevt;
//	pthread_cond_t  condvar;
//	pthread_mutex_t mutex;
//	struct itimerspec timerval;
//	struct itimerspec *timerover;//set NULL
//};
struct timer_struct
{
	bool is_timeroff;
	union sigval   sigev_value; //signal value
	void(*sigev_notify_function)(union sigval);
	struct itimerspec timerval;    /* timer expiration */
	pthread_t tid;
	pthread_cond_t  condvar;
	pthread_mutex_t cond_mutex;
	pthread_mutex_t status_mutex;
};
typedef struct timer_struct* timer_t;

#endif