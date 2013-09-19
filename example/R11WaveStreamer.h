/************************************************************************
Route11 - C++ Flow-Based Metaprogramming Library
Copyright (c) 2013 Marcus Tomlinson

This file is part of Route11.

Simplified BSD Licence:
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
************************************************************************/

#ifndef R11WAVESTREAMER_H
#define R11WAVESTREAMER_H

//-----------------------------------------------------------------------------

#include <condition_variable>
#include <vector>

//=============================================================================

// this process policy implements a stereo wave file audio streamer

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
  // 1 input file name, 1 input play trigger
  std::tuple< std::string, bool > input_;

  // 2 output audio streams
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
