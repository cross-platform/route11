#ifndef R11AUDIODEVICE_H
#define R11AUDIODEVICE_H

//-----------------------------------------------------------------------------

#include <string>
#include <vector>
#include <condition_variable>

struct RtAudioMembers;

//=============================================================================

class R11AudioDevice
{
public:
  R11AudioDevice();

  bool SetDevice( int_fast16_t deviceIndex );

  std::string GetDeviceName( int_fast16_t deviceIndex );
  uint_fast16_t GetDeviceInputCount( int_fast16_t deviceIndex );
  uint_fast16_t GetDeviceOutputCount( int_fast16_t deviceIndex );
  uint_fast16_t GetCurrentDevice();
  uint_fast16_t GetDeviceCount();

  bool IsStreaming();

  void SetBufferSize( uint_fast32_t bufferSize );
  void SetSampleRate( uint_fast32_t sampleRate );
  uint_fast32_t GetSampleRate();

protected:
  ~R11AudioDevice();

  void Process_();

protected:
  std::tuple< std::vector< float >, std::vector< float > > input_;
  std::tuple< std::vector< float >, std::vector< float > > output_;

private:
  void _WaitForBuffer();
  void _SyncBuffer();

  void _StopStream();
  void _StartStream();

  static int _StaticCallback( void* outputBuffer,
                              void* inputBuffer,
                              unsigned int nBufferFrames,
                              double streamTime,
                              unsigned int status,
                              void* userData );

  int _DynamicCallback( void* inputBuffer, void* outputBuffer );

private:
  uint_fast32_t _bufferSize = 128;
  uint_fast32_t _sampleRate = 11025;
  int_fast16_t _deviceCount = 0;

  bool _gotWaitReady = false;
  bool _gotSyncReady = true;
  bool _streamStop = false;
  bool _isStreaming = false;

  std::vector< std::vector< float > > _outputChannels;
  std::vector< std::vector< float > > _inputChannels;

  RtAudioMembers* _rtAudio;

  std::mutex _buffersMutex;
  std::mutex _syncMutex;
  std::condition_variable_any _waitCondt;
  std::condition_variable_any _syncCondt;

  uint_fast16_t _currentDevice = 0;
};

//=============================================================================

#endif // R11AUDIODEVICE_H
