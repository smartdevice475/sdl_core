/*
 * Copyright (c) 2014-2015, Ford Motor Company
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

#include "media_manager/audio/callback_audio_streamer_adapter.h"

#if defined(WIN32) || defined(WINCE)
#include "media_manager/audio/play_wave.h"
#endif

namespace media_manager {

CallbackAudioStreamerAdapter::CallbackAudioStreamerAdapter() {
#if defined(WIN32) || defined(WINCE)
  wave_player_ = new WavePlayer;
#else
  wave_player_ = new WavePlayerInterface;
#endif	
}

void CallbackAudioStreamerAdapter::SendData(int32_t application_key,
    const ::protocol_handler::RawMessagePtr msg)
{
	if (!is_app_performing_activity(application_key)) {
		return;
	}
#ifdef BUILD_TARGET_LIB
	//s_mediaAudioStreamSendCallback((const char *)msg->data(), msg.get()->data_size());
#endif
	//printf("%s, Line:%d, Streamer::sent %d\n", __FUNCTION__, __LINE__, msg->data_size());

	wave_player_->SendData((char *)msg->data(), msg->data_size());
}

CallbackAudioStreamerAdapter::~CallbackAudioStreamerAdapter() {
	delete wave_player_;
}

void CallbackAudioStreamerAdapter::StartActivity(int32_t application_key) {
	StreamerAdapter::StartActivity(application_key);
	wave_player_->StartActivity();
}

void CallbackAudioStreamerAdapter::StopActivity(int32_t application_key) {
	wave_player_->StopActivity();
	StreamerAdapter::StopActivity(application_key);
}

}  // namespace media_manager
