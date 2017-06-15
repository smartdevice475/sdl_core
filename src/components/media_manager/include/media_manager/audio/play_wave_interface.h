#ifndef SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_AUDIO_PLAY_WAVE_INTERFACE_H_
#define SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_AUDIO_PLAY_WAVE_INTERFACE_H_

#include "play_wave_interface.h"

namespace media_manager {

  class WavePlayerInterface
  {
  public:
    WavePlayerInterface() {};
    virtual ~WavePlayerInterface() {};

    virtual void StartActivity() {};
    virtual void StopActivity() {};
    virtual void SendData(char *data, int len) {};
  };
}

#endif // SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_AUDIO_PLAY_WAVE_INTERFACE_H_
