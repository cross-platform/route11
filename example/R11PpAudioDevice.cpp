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

#include "R11PpAudioDevice.h"

#include "../include/Route11.h"
#include "rtaudio/RtAudio.h"

#include <iostream>
#include "string.h"
#include <cstdlib>
#include <thread>

//=============================================================================

struct RtAudioMembers
{
  std::vector< RtAudio::DeviceInfo > deviceList;

  RtAudio audioStream;
  RtAudio::StreamParameters outputParams;
  RtAudio::StreamParameters inputParams;
};

//=============================================================================

R11PpAudioDevice::R11PpAudioDevice()
: _rtAudio( new RtAudioMembers() )
{
  _outputChannels.resize( 2 );
  _inputChannels.resize( 2 );

  _deviceCount = _rtAudio->audioStream.getDeviceCount();
  _rtAudio->deviceList.resize( _deviceCount );

  for( int_fast16_t i = 0; i < _deviceCount; ++i )
  {
    _rtAudio->deviceList[i] = _rtAudio->audioStream.getDeviceInfo( i );
  }

  SetDevice( _rtAudio->audioStream.getDefaultOutputDevice() );

  SetBufferSize( _bufferSize );
  SetSampleRate( _sampleRate );
}

//-----------------------------------------------------------------------------

R11PpAudioDevice::~R11PpAudioDevice()
{
  _StopStream();

  delete _rtAudio;
}

//-----------------------------------------------------------------------------

bool R11PpAudioDevice::SetDevice( int_fast16_t deviceIndex )
{
  if( deviceIndex >= 0 && deviceIndex < ( _deviceCount ) )
  {
    _currentDevice = deviceIndex;

    _StopStream();

    _rtAudio->inputParams.nChannels = _rtAudio->deviceList[deviceIndex].inputChannels;
    _rtAudio->inputParams.deviceId = deviceIndex;

    _rtAudio->outputParams.nChannels = _rtAudio->deviceList[deviceIndex].outputChannels;
    _rtAudio->outputParams.deviceId = deviceIndex;

    _StartStream();

    return true;
  }

  return false;
}

//-----------------------------------------------------------------------------

std::string R11PpAudioDevice::GetDeviceName( int_fast16_t deviceIndex )
{
  if( deviceIndex >= 0 && deviceIndex < ( _deviceCount ) )
  {
    return _rtAudio->deviceList[deviceIndex].name;
  }

  return "";
}

//-----------------------------------------------------------------------------

uint_fast16_t R11PpAudioDevice::GetDeviceInputCount( int_fast16_t deviceIndex )
{
  return _rtAudio->deviceList[deviceIndex].inputChannels;
}

//-----------------------------------------------------------------------------

uint_fast16_t R11PpAudioDevice::GetDeviceOutputCount( int_fast16_t deviceIndex )
{
  return _rtAudio->deviceList[deviceIndex].outputChannels;
}

//-----------------------------------------------------------------------------

uint_fast16_t R11PpAudioDevice::GetCurrentDevice()
{
  return _currentDevice;
}

//-----------------------------------------------------------------------------

uint_fast16_t R11PpAudioDevice::GetDeviceCount()
{
  return _deviceCount;
}

//-----------------------------------------------------------------------------

bool R11PpAudioDevice::IsStreaming()
{
  return _isStreaming;
}

//-----------------------------------------------------------------------------

void R11PpAudioDevice::SetBufferSize( uint_fast32_t bufferSize )
{
  _StopStream();

  _bufferSize = bufferSize;

  for( uint_fast16_t i = 0; i < _inputChannels.size(); ++i )
  {
    _inputChannels[i].resize( _bufferSize );
  }

  _StartStream();
}

//-----------------------------------------------------------------------------

void R11PpAudioDevice::SetSampleRate( uint_fast32_t sampleRate )
{
  _StopStream();

  _sampleRate = sampleRate;

  _StartStream();
}

//-----------------------------------------------------------------------------

uint_fast32_t R11PpAudioDevice::GetSampleRate()
{
  return _sampleRate;
}

//=============================================================================

void R11PpAudioDevice::Process_()
{
  // Wait until the sound card is ready for the next set of buffers

  _syncMutex.lock();
  if( !_gotSyncReady )              // if haven't already got the release
    _syncCondt.wait( _syncMutex );  // wait for sync
  _gotSyncReady = false;            // reset the release flag
  _syncMutex.unlock();

  // Retrieve incoming component buffers for the sound card to output

  _outputChannels[0] = std::get< 0 >( input_ );
  _outputChannels[1] = std::get< 1 >( input_ );

  // Retrieve incoming sound card buffers for the component to output

  std::get< 0 >( output_ ) = _inputChannels[0];
  std::get< 1 >( output_ ) = _inputChannels[1];

  // Inform the sound card that buffers are now ready

  _buffersMutex.lock();
  _gotWaitReady = true; // set release flag
  _waitCondt.notify_all(); // release sync
  _buffersMutex.unlock();
}

//=============================================================================

void R11PpAudioDevice::_WaitForBuffer()
{
  _buffersMutex.lock();
  if( !_gotWaitReady )										//if haven't already got the release
    _waitCondt.wait( _buffersMutex );		  //wait for sync
  _gotWaitReady = false;									//reset the release flag
  _buffersMutex.unlock();
}

//-----------------------------------------------------------------------------

void R11PpAudioDevice::_SyncBuffer()
{
  _syncMutex.lock();
  _gotSyncReady = true;								//set release flag
  _syncCondt.notify_all();						//release sync
  _syncMutex.unlock();
}

//-----------------------------------------------------------------------------

void R11PpAudioDevice::_StopStream()
{
  _streamStop = true;

  for( uint_fast16_t i = 0; i < 250; ++i )
  {
    _waitCondt.notify_all();

    if( _isStreaming )
    {
      std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }
    else
    {
      break;
    }
  }

  if( _rtAudio->audioStream.isStreamRunning() )
  {
    _rtAudio->audioStream.stopStream();
  }

  if( _rtAudio->audioStream.isStreamOpen() )
  {
    _rtAudio->audioStream.closeStream();
  }

  _syncCondt.notify_all();
}

//-----------------------------------------------------------------------------

void R11PpAudioDevice::_StartStream()
{
  RtAudio::StreamParameters* inputParams = NULL;
  RtAudio::StreamParameters* outputParams = NULL;

  if( _rtAudio->inputParams.nChannels != 0 )
  {
    inputParams = &_rtAudio->inputParams;
  }

  if( _rtAudio->outputParams.nChannels != 0 )
  {
    outputParams = &_rtAudio->outputParams;
  }

  RtAudio::StreamOptions options;
  options.flags |= RTAUDIO_SCHEDULE_REALTIME;
  options.flags |= RTAUDIO_NONINTERLEAVED;

  _rtAudio->audioStream.openStream( outputParams,
                                    inputParams,
                                    RTAUDIO_FLOAT32,
                                    _sampleRate,
                                    ( unsigned int* ) &_bufferSize,
                                    &_StaticCallback,
                                    this,
                                    &options );

  _rtAudio->audioStream.startStream();

  _streamStop = false;
  _isStreaming = true;
}

//-----------------------------------------------------------------------------

int R11PpAudioDevice::_StaticCallback( void* outputBuffer,
                                     void* inputBuffer,
                                     unsigned int nBufferFrames,
                                     double streamTime,
                                     unsigned int status,
                                     void* userData )
{
  unused( nBufferFrames, streamTime, status );
  return ( ( R11PpAudioDevice* ) userData )->_DynamicCallback( inputBuffer, outputBuffer );
}

//-----------------------------------------------------------------------------

int R11PpAudioDevice::_DynamicCallback( void* inputBuffer, void* outputBuffer )
{
  _WaitForBuffer();

  if( !_streamStop )
  {
    float* floatOutput = ( float* ) outputBuffer;
    float* floatInput = ( float* ) inputBuffer;

    if( outputBuffer != NULL )
    {
      for( uint_fast32_t i = 0; i < _outputChannels.size(); ++i )
      {
        if( _rtAudio->deviceList[_currentDevice].outputChannels >= ( i + 1 ) )
        {
          for( uint_fast32_t j = 0; j < _outputChannels[i].size(); ++j )
          {
            *floatOutput++ = _outputChannels[i][j];
          }
        }
      }
    }

    if( inputBuffer != NULL )
    {
      for( uint_fast32_t i = 0; i < _inputChannels.size(); ++i )
      {
        if( _rtAudio->deviceList[_currentDevice].inputChannels >= ( i + 1 ) )
        {
          for( uint_fast32_t j = 0; j < _inputChannels[i].size(); ++j )
          {
            _inputChannels[i][j] = *floatInput++;
          }
        }
      }
    }
  }
  else
  {
    _isStreaming = false;
  }

  _SyncBuffer();

  return 0;
}

//=============================================================================
