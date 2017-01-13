#ifndef SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_AUDIO_PLAY_WAVE_H_
#define SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_AUDIO_PLAY_WAVE_H_

#undef WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <string>
#include "pthread.h"
#include <deque>
#ifdef OS_WIN32
#include <mmeapi.h>
#endif

namespace media_manager {

  class WavePlayer
  {
  public:
    WavePlayer();
    ~WavePlayer();

    void StartActivity();
    void StopActivity();
    void SendData(char *data, int len);
  private:
    HWAVEOUT wave_out_handle_;

    int WavePlayer::DevOpen(char * buffer, int len);

    bool audio_play_shutdown_;
    bool to_close_;
    LPWAVEHDR wave_header_[2];
    LPWAVEHDR next_wave_header_;

    struct AudioData
    {
      char *data;
      unsigned int data_lenth;

      AudioData() {
        data = NULL;
        data_lenth = 0;
      }
    };


    std::deque<AudioData *> audio_data_deque_;

    pthread_t audio_play_thread_;
    pthread_t frame_play_done_thread_;

    void OnPlayDone();

    friend void* AudioPlayThread(void* data);
    friend void* FramePlayDoneThread(void* data);
    friend void CALLBACK waveOutProc(HWAVEOUT hwo,
                                     UINT uMsg,
                                     DWORD dwInstance,
                                     DWORD dwParam1,
                                     DWORD dwParam2);
    void PlayAudio();
    void FramePlayDone();
    int FindFormatString(char *data, int len);

    int PrepareWaveData(LPWAVEHDR wave_data);

    pthread_cond_t wait_data_cond;
    pthread_cond_t play_done_cond;
    pthread_mutex_t play_lock_;
  };
}
#define WIN32_LEAN_AND_MEAN
#endif // SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_AUDIO_PLAY_WAVE_H_