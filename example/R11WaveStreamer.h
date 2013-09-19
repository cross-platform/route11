#ifndef R11WAVESTREAMER_H
#define R11WAVESTREAMER_H

//-----------------------------------------------------------------------------

#include <condition_variable>
#include <vector>

//=============================================================================

class R11WaveStreamer
{
public:
  R11WaveStreamer();

  bool LoadFile( std::string& filePath );
  void Play();
  void Pause();
  void Stop();

  bool IsPlaying();

protected:
  ~R11WaveStreamer();

  void Process_();

protected:
  std::tuple< std::string, bool > input_;
  std::tuple< std::vector< float >, std::vector< float > > output_;

private:
  struct WaveFormat
  {
    void Clear()
    {
      format = 0;
      channelCount = 0;
      sampleRate = 0;
      byteRate = 0;
      frameSize = 0;
      bitDepth = 0;
      extraDataSize = 0;
    }

    uint_fast16_t format;        // Integer identifier of the format
    uint_fast16_t channelCount;  // Number of audio channels
    uint_fast32_t sampleRate;    // Audio sample rate
    uint_fast32_t byteRate;      // Bytes per second (possibly approximate)
    uint_fast16_t frameSize;     // Size in bytes of a sample block (all channels)
    uint_fast16_t bitDepth;      // Size in bits of a single per-channel sample
    uint_fast16_t extraDataSize; // Bytes of extra data appended to this struct
  };

private:
  std::string _filePath = "";
  bool _isPlaying = false;
  uint_fast32_t _bufferSize = 128;
  uint_fast32_t _sampleIndex = 0;
  float _shortToFloatCoeff = 1.0f / 32767.0f;

  WaveFormat _waveFormat;
  std::vector< int16_t > _waveData;
  std::mutex _busyMutex;

  std::vector< float > _leftChannel;
  std::vector< float > _rightChannel;
};

//=============================================================================

#endif // R11WAVESTREAMER_H
