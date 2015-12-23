/*
 * Copyright (c) 2014, Ford Motor Company
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
#ifndef OS_WINCE
#include <csignal>
#endif
#include <cstdlib>
#include <stdint.h>

#include "utils/signals.h"

namespace utils {

#if defined(OS_WIN32) || defined(OS_WINCE)
bool SubscribeToTerminateSignal(void (*func)(int32_t p)) {
  void (*prev_func)(int32_t p);
#ifdef OS_WINCE
  return true;
#else
  prev_func = signal(SIGINT, func);
  return (SIG_ERR != prev_func);
#endif
}

bool ResetSubscribeToTerminateSignal() {
  void (*prev_func)(int32_t p);
#ifdef OS_WINCE
  return true;
#else
  prev_func = signal(SIGINT, SIG_DFL);
  return (SIG_ERR != prev_func);
#endif
}

void ForwardSignal() {
#ifndef OS_WINCE
  int32_t signal_id = SIGINT;
  raise(signal_id);
#endif
}

#else
bool SubscribeToTerminateSignal(sighandler_t func) {
  struct sigaction act;
  act.sa_handler = func;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  bool sigint_subscribed = (sigaction(SIGINT, &act, NULL) == 0);
  bool sigterm_subscribed = (sigaction(SIGTERM, &act, NULL) == 0);

  return sigint_subscribed && sigterm_subscribed;
}

bool SubscribeToFaultSignal(sighandler_t func) {
  struct sigaction act;
  act.sa_handler = func;
  sigemptyset(&act.sa_mask);
  act.sa_flags = SA_RESETHAND; // we only want to catch SIGSEGV once to flush logger queue

  bool sigsegv_subscribed = (sigaction(SIGSEGV, &act, NULL) == 0);

  return sigsegv_subscribed;
}
#endif
}  //  namespace utils
