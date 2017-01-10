#include "media_manager/audio/play_wave.h"
#include <stdio.h>

namespace media_manager {
#define PCM_HEAD_LENTH 46

  WavePlayer::WavePlayer()
    :audio_play_shutdown_(true), play_lock_(), to_close_(false), next_wave_header_(NULL)
  {
    wave_header_[0] = new WAVEHDR;
    memset(wave_header_[0], 0, sizeof(WAVEHDR));
    wave_header_[1] = new WAVEHDR;
    memset(wave_header_[1], 0, sizeof(WAVEHDR));

    pthread_cond_init(&ready_to_play_, NULL);
    pthread_mutex_init(&play_lock_, NULL);
  }

  WavePlayer::~WavePlayer()
  {
    delete wave_header_[0];
    delete wave_header_[1];
    next_wave_header_ = NULL;
  }

  void WavePlayer::StartActivity() {
    play_done_ = true;
    to_close_ = false;
    next_wave_header_ = wave_header_[1];
  }

  void WavePlayer::StopActivity() {
    pthread_mutex_lock(&play_lock_);
    to_close_ = true;
    pthread_mutex_unlock(&play_lock_);
    return;
  }

  void WavePlayer::PrepareWaveData(LPWAVEHDR wave_data) {
    AudioData *to_play_data = NULL;
    int ret = 0;

    if (audio_data_deque_.empty()) {
      return;
    }    

    wave_data->dwLoops = 1;
    to_play_data = audio_data_deque_.front();
    wave_data->dwBufferLength = to_play_data->data_lenth;
    wave_data->lpData = new char[to_play_data->data_lenth];
    memcpy(wave_data->lpData, to_play_data->data, to_play_data->data_lenth);

    delete audio_data_deque_.front()->data;
    delete audio_data_deque_.front();
    audio_data_deque_.pop_front();

    ret = ::waveOutPrepareHeader(wave_out_handle_, wave_data,
      sizeof(WAVEHDR));
    ret = ::waveOutWrite(wave_out_handle_, wave_data,
      sizeof(WAVEHDR));
  }

  void WavePlayer::PlayAudio() {
    Sleep(1000);
    pthread_mutex_lock(&play_lock_);

    PrepareWaveData(wave_header_[0]);

    while (!audio_play_shutdown_)
    {
      if (next_wave_header_ != NULL) {
        PrepareWaveData(next_wave_header_);
        next_wave_header_ = NULL;
      }

      if (NULL == next_wave_header_) {
        pthread_cond_wait(&ready_to_play_, &play_lock_);
      }      
    }
    pthread_mutex_unlock(&play_lock_);
  }

  void WavePlayer::FramePlayDone() {
    pthread_mutex_lock(&play_lock_);
    int ret = ::waveOutUnprepareHeader(wave_out_handle_,
      next_wave_header_, sizeof(WAVEHDR));

    delete next_wave_header_->lpData;
    next_wave_header_->lpData = NULL;

    if (to_close_)
    {
      audio_play_shutdown_ = true;
      while (!audio_data_deque_.empty()) {
        delete audio_data_deque_.front()->data;
        delete audio_data_deque_.front();
        audio_data_deque_.pop_front();
      }

      next_wave_header_ = NULL;
      ret = ::waveOutClose(wave_out_handle_);
    }

    pthread_mutex_unlock(&play_lock_);
    pthread_cond_signal(&ready_to_play_);
  }

  void* AudioPlayThread(void* data) {
    WavePlayer *wave_player = (WavePlayer *)data;
    wave_player->PlayAudio();
    return NULL;
  }

  void* FramePlayDoneThread(void* data) {
    WavePlayer *wave_player = (WavePlayer *)data;
    wave_player->FramePlayDone();
    return NULL;
  }

  void WavePlayer::SendData(char *data, int len) {
    if (len > 0 && !to_close_) {
      if (audio_play_shutdown_) {
        if (len > PCM_HEAD_LENTH) {
          int read_lenth = DevOpen(data, len);

          if (read_lenth > 0) {
            AudioData *audio_data = new AudioData;
            audio_data->data = new char[len];
            memcpy(audio_data->data, data + read_lenth,
              len - read_lenth);
            audio_data->data_lenth = len;

            pthread_mutex_lock(&play_lock_);
            audio_data_deque_.push_back(audio_data);
            pthread_mutex_unlock(&play_lock_);

            audio_play_shutdown_ = false;
            pthread_create(&audio_play_thread_, 0,
              AudioPlayThread, this);
          }
        }
      }
      else {
        //continue play audio
        AudioData *audio_data = new AudioData;
        audio_data->data = new char[len];
        memcpy(audio_data->data, data, len);
        audio_data->data_lenth = len;

        pthread_mutex_lock(&play_lock_);
        audio_data_deque_.push_back(audio_data);
        pthread_mutex_unlock(&play_lock_);

        pthread_cond_signal(&ready_to_play_);
      }
    }
  }

  void CALLBACK waveOutProc(
    HWAVEOUT  hwo,
    UINT      uMsg,
    DWORD_PTR dwInstance,
    DWORD_PTR dwParam1,
    DWORD_PTR dwParam2
    )
  {
    if (WOM_DONE == uMsg)
    {
      WavePlayer *wave_player = (WavePlayer *)dwInstance;
      wave_player->next_wave_header_ = (LPWAVEHDR)dwParam1;
      wave_player->CreateFramePlayDoneThread();
    }
    return;
  }

  void WavePlayer::CreateFramePlayDoneThread() {
    pthread_create(&frame_play_done_thread_, 0, FramePlayDoneThread, this);
  }

  int WavePlayer::FindFormatString(char *data, int len) {
    std::string target("fmt ");
    char key[4];
    for (size_t i = 0; i < 4; i++)
    {
      key[i] = target.at(i);
    }

    for (size_t i = 0; i < len - 3; i++)
    {
      if (data[i] == key[0] && data[i + 1] == key[1]
        && data[i + 2] == key[2] && data[i + 3] == key[3]) {
        return i + 4;
      }
    }

    return -1;
  }

  int WavePlayer::DevOpen(char * buffer, int len)
  {
    std::string fmt_string(buffer);
    int pos = FindFormatString(buffer, len);
    if (-1 == pos) {
      return -1;
    }
    WAVEFORMATEX wfx;
    if (pos != std::string::npos) {
      int fmt_lenth = 0;
      char *buffer_index = buffer + pos;
      memcpy(&fmt_lenth, buffer_index, 4);
      buffer_index += 4;

      memcpy(&wfx, buffer_index, sizeof(WAVEFORMATEX));

      MMRESULT ret = ::waveOutOpen(0, 0, &wfx, 0, 0, WAVE_FORMAT_QUERY);
      if (ret) {
        return 0;
      }

      ret = waveOutOpen(&wave_out_handle_, WAVE_MAPPER, &wfx,
        (DWORD)waveOutProc, (DWORD)this, CALLBACK_FUNCTION);

      if (ret) {
        return 0;
      }

      return pos + fmt_lenth + 12;
    }
    else {
      return 0;
    }
  }
}