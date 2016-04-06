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

#include "utils/timer.h"

#include <ctime>
#ifndef OS_WINCE
#include <cerrno>
#endif
#include <cstring>

#include "utils/timer_task.h"
#include "utils/date_time.h"
#include "utils/macro.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

using date_time::DateTime;

namespace timer {

// Function HandlePosixTimer is not in anonymous namespace
// because we need to set this func as friend to Timer
// and for setting friend function must be located in same namespace with class
void HandlePosixTimer(sigval signal_value) {
  LOG4CXX_AUTO_TRACE(logger_);

  DCHECK_OR_RETURN_VOID(signal_value.sival_ptr)

  timer::Timer* timer = static_cast<timer::Timer*>(signal_value.sival_ptr);
  timer->OnTimeout();
}
}  // namespace timer

namespace {
const int kErrorCode = -1;

timespec MillisecondsToItimerspec(const timer::Milliseconds miliseconds) {
  struct timespec now;
  struct timespec wait_interval;
  clock_gettime(CLOCK_REALTIME, &now);
  wait_interval.tv_sec = now.tv_sec +
	(miliseconds / DateTime::MILLISECONDS_IN_SECOND);
  wait_interval.tv_nsec = now.tv_nsec +
	(miliseconds %  DateTime::MILLISECONDS_IN_SECOND) *  DateTime::NANOSECONDS_IN_MILLISECOND;
  wait_interval.tv_sec += wait_interval.tv_nsec / DateTime::NANOSECONDS_IN_MILLISECOND;
  wait_interval.tv_nsec %= DateTime::NANOSECONDS_IN_MILLISECOND;
  return wait_interval;
}

void* pthread_timer_run(void* arg)
{
	return NULL;
}

void* pthread_timer_notify(void* arg)
{
	timer_t sig_ = static_cast<timer_t>(arg);
	pthread_mutex_lock(&sig_->status_mutex);
	sig_->is_timeroff = false;
	while (sig_&&!sig_->is_timeroff){
		struct timespec waittime = MillisecondsToItimerspec(sig_->millionsecs);
		int32_t result=pthread_cond_timedwait(&sig_->condvar, 
			&sig_->cond_mutex, 
			&waittime);
		if (result ==ETIMEDOUT){
			sig_->sigev_notify_function(sig_->sigev_value);
		}
		else{
			if (sig_->is_timeroff)
		        break;
		}
		if (!sig_->is_repeated)
			break;

	}
	sig_->is_timeroff = true;
	pthread_mutex_unlock(&sig_->status_mutex);
	return NULL;
}

timer_t StartPosixTimer(timer::Timer& trackable,
                        const timer::Milliseconds timeout,bool isrepeated=false) {
  LOG4CXX_AUTO_TRACE(logger_);
  int result;
  timer_t internal_timer = NULL;
  internal_timer = (timer_t)malloc(sizeof(timer_struct));
  if (!internal_timer){
	  LOG4CXX_FATAL(logger_,
		  "Can`t create posix_timer. Error("
		  << ENOMEM << "): " << strerror(ENOMEM));
	  return NULL;
  }
  memset(internal_timer, 0, sizeof(timer_struct));

  if ((0 != pthread_mutex_init(&internal_timer->status_mutex, NULL))||
	  (0 != pthread_mutex_init(&internal_timer->cond_mutex, NULL)) ||
	  (0 != pthread_cond_init(&internal_timer->condvar, NULL))){
	  int error_code = errno;
	  LOG4CXX_FATAL(logger_,
		  "Can`t create posix_timer. Error("
		  << error_code << "): " << strerror(error_code));
	  free(internal_timer);
	  return NULL;
  }
  internal_timer->is_repeated = isrepeated;
  internal_timer->millionsecs = timeout;
  internal_timer->sigev_value.sival_ptr = static_cast<void*>(&trackable);;
  internal_timer->sigev_notify_function = timer::HandlePosixTimer;
  
  if ((result = pthread_create(&internal_timer->tid, NULL, pthread_timer_notify, internal_timer)) != 0){
	  LOG4CXX_FATAL(logger_,
		  "Can`t create posix_timer. Error("
		  << result << "): " << strerror(result));
	  free(internal_timer);
	  return NULL;
  }
  return internal_timer;
}

bool StopPosixTimer(timer_t timer) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!timer)
	  return false;
  timer->is_timeroff = true;
  pthread_cond_signal(&timer->condvar);
  pthread_join(timer->tid, NULL);
  pthread_cond_destroy(&timer->condvar);
  pthread_mutex_destroy(&timer->cond_mutex);
  pthread_mutex_destroy(&timer->status_mutex);
  free(timer);
  return true;
}
}  // namespace

timer::Timer::Timer(const std::string& name, const TimerTask* task_for_tracking)
    : name_(name)
    , task_(task_for_tracking)
    , repeatable_(false)
    , timeout_ms_(0u)
    , is_running_(false)
    , timer_(NULL) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(!name_.empty());
  DCHECK(task_);
}

timer::Timer::~Timer() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Timer is to be destroyed " << name_);
  Stop();
  sync_primitives::AutoLock auto_lock(task_lock_);
  DCHECK(task_);
  delete task_;
}

void timer::Timer::Start(const Milliseconds timeout, const bool repeatable) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(lock_);
  SetTimeoutUnsafe(timeout);
  repeatable_ = repeatable;
  if (is_running_) {
    const bool stop_result = StopUnsafe();
    DCHECK_OR_RETURN_VOID(stop_result);
  }
  StartUnsafe();
}

void timer::Timer::Stop() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(lock_);
  repeatable_ = false;
  if (is_running_) {
    const bool stop_result = StopUnsafe();
    DCHECK(stop_result);
  }
}

bool timer::Timer::IsRunning() const {
  sync_primitives::AutoLock auto_lock(lock_);
  return is_running_;
}

void timer::Timer::OnTimeout() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "Timer has finished counting. Timeout(ms): "
                    << static_cast<uint32_t>(timeout_ms_));
  {
    // Task locked by own lock because from this task in callback we can
    // call Stop of this timer and get DeadLock
    sync_primitives::AutoLock auto_lock(task_lock_);
    DCHECK(task_);
    task_->run();
  }
  //sync_primitives::AutoLock auto_lock(lock_);
  //if (is_running_) {
  //  const bool stop_result = StopUnsafe();
  //  DCHECK_OR_RETURN_VOID(stop_result);
  //}
  //if (repeatable_) {
  //  StartUnsafe();
  //}
}

void timer::Timer::SetTimeoutUnsafe(const timer::Milliseconds timeout) {
  timeout_ms_ = (0u != timeout) ? timeout : 1u;
}

void timer::Timer::StartUnsafe() {
  LOG4CXX_DEBUG(logger_, "Creating posix_timer in " << name_);
  // Create new posix timer
  timer_ = StartPosixTimer(*this, timeout_ms_,repeatable_);
  DCHECK_OR_RETURN_VOID(timer_);
  is_running_ = true;
}

bool timer::Timer::StopUnsafe() {
  LOG4CXX_DEBUG(logger_, "Stopping timer  " << name_);
  //  Destroing of posix timer
  if (StopPosixTimer(timer_)) {
    is_running_ = false;
    timer_ = NULL;
    return true;
  }
  return false;
}

timer::Milliseconds timer::Timer::GetTimeout() const {
  sync_primitives::AutoLock auto_lock(lock_);
  return timeout_ms_;
}
