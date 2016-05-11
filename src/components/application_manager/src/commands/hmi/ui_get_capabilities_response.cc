/**
 * Copyright (c) 2013, Ford Motor Company
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
#include "application_manager/commands/hmi/ui_get_capabilities_response.h"
#include "application_manager/application_manager_impl.h"

namespace application_manager {

namespace commands {

UIGetCapabilitiesResponse::UIGetCapabilitiesResponse(
    const MessageSharedPtr& message)
    : ResponseFromHMI(message) {
}

UIGetCapabilitiesResponse::~UIGetCapabilitiesResponse() {
}

void UIGetCapabilitiesResponse::Run() {
  LOG4CXX_INFO(logger_, "UIGetCapabilitiesResponse::Run");

  HMICapabilities& hmi_capabilities =
      ApplicationManagerImpl::instance()->hmi_capabilities();

  hmi_capabilities.set_display_capabilities(
      (*message_)[strings::msg_params][hmi_response::display_capabilities]);

  hmi_capabilities.set_hmi_zone_capabilities(
      (*message_)[strings::msg_params][hmi_response::hmi_zone_capabilities]);

  if ((*message_)[strings::msg_params].keyExists(
                                      hmi_response::soft_button_capabilities)) {
    hmi_capabilities.set_soft_button_capabilities(
      (*message_)[strings::msg_params][hmi_response::soft_button_capabilities]);
  }

  if ((*message_)[strings::msg_params].keyExists(
                                      strings::audio_pass_thru_capabilities)) {

    hmi_capabilities.set_audio_pass_thru_capabilities(
      (*message_)[strings::msg_params][strings::audio_pass_thru_capabilities]);
  }
}

}  // namespace commands

}  // namespace application_manager
