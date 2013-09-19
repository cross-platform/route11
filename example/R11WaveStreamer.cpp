#include "R11WaveStreamer.h"

#include <fstream>
#include <iostream>
#include <string.h>

//=============================================================================

R11WaveStreamer::R11WaveStreamer()
{
  _leftChannel.resize( _bufferSize );
  _rightChannel.resize( _bufferSize );
}

//-----------------------------------------------------------------------------

R11WaveStreamer::~R11WaveStreamer() {}

//=============================================================================

bool R11WaveStreamer::LoadFile( std::string& filePath )
{
  if( filePath == "" || filePath == _filePath )
  {
    return true;
  }

  _filePath = filePath;

  bool wasPlaying = _isPlaying;
  Stop();

  std::ifstream inFile( filePath, std::ios::binary | std::ios::in );
  if( inFile.bad() )
  {
    return false;
  }

  uint_fast32_t dwFileSize = 0, dwChunkSize = 0;
  char* dwChunkId = new char[5];
  char* dwExtra = new char[5];

  dwChunkId[4] = 0;
  dwExtra[4] = 0;

  //look for 'RIFF' chunk identifier
  inFile.seekg( 0, std::ios::beg );
  inFile.read( dwChunkId, 4 );
  if( strcmp( dwChunkId, "RIFF" ) )
  {
    std::cerr << "'" << filePath << "' not found.\n";
    inFile.close();
    return false;
  }

  inFile.seekg( 4, std::ios::beg ); //get file size
  inFile.read( reinterpret_cast<char*>( &dwFileSize ), sizeof( dwFileSize ) );
  if( dwFileSize <= 16 )
  {
    inFile.close();
    return false;
  }

  inFile.seekg( 8, std::ios::beg ); //get file format
  inFile.read( dwExtra, 4 );
  if( strcmp( dwExtra, "WAVE" ) )
  {
    inFile.close();
    return false;
  }

  //look for 'fmt ' chunk id
  bool bFilledFormat = false;
  for( uint_fast32_t i = 12; i < dwFileSize; )
  {
    inFile.seekg( i, std::ios::beg );
    inFile.read( dwChunkId, 4 );
    inFile.seekg( i + 4, std::ios::beg );
    inFile.read( reinterpret_cast<char*>( &dwChunkSize ), sizeof( dwChunkSize ) );
    if( !strcmp( dwChunkId, "fmt " ) )
    {
      inFile.seekg( i + 8, std::ios::beg );

      _waveFormat.Clear();
      inFile.read( reinterpret_cast<char*>( &_waveFormat.format ), 2 );
      inFile.read( reinterpret_cast<char*>( &_waveFormat.channelCount ), 2 );
      inFile.read( reinterpret_cast<char*>( &_waveFormat.sampleRate ), 4 );
      inFile.read( reinterpret_cast<char*>( &_waveFormat.byteRate ), 4 );
      inFile.read( reinterpret_cast<char*>( &_waveFormat.frameSize ), 2 );
      inFile.read( reinterpret_cast<char*>( &_waveFormat.bitDepth ), 2 );
      inFile.read( reinterpret_cast<char*>( &_waveFormat.extraDataSize ), 2 );

      bFilledFormat = true;
      break;
    }

    dwChunkSize += 8; //add offsets of the chunk id, and chunk size data entries
    dwChunkSize += 1;
    dwChunkSize &= 0xfffffffe; //guarantees WORD padding alignment
    i += dwChunkSize;
  }

  if( !bFilledFormat )
  {
    inFile.close();
    return false;
  }

  //look for 'data' chunk id
  bool bFilledData = false;
  for( uint_fast32_t i = 12; i < dwFileSize; )
  {
    inFile.seekg( i, std::ios::beg );
    inFile.read( dwChunkId, 4 );
    inFile.seekg( i + 4, std::ios::beg );
    inFile.read( reinterpret_cast<char*>( &dwChunkSize ), sizeof( dwChunkSize ) );
    if( !strcmp( dwChunkId, "data" ) )
    {
      _waveData.resize( dwChunkSize / 2 );
      inFile.seekg( i + 8, std::ios::beg );
      inFile.read( reinterpret_cast<char*>( &_waveData[0] ), dwChunkSize );
      bFilledData = true;
      break;
    }

    dwChunkSize += 8; //add offsets of the chunk id, and chunk size data entries
    dwChunkSize += 1;
    dwChunkSize &= 0xfffffffe; //guarantees WORD padding alignment
    i += dwChunkSize;
  }

  if( !bFilledData )
  {
    inFile.close();
    return false;
  }

  inFile.close();

  if( wasPlaying )
  {
    Play();
  }

  return true;
}

//-----------------------------------------------------------------------------

void R11WaveStreamer::Play()
{
  _isPlaying = true;
}

//-----------------------------------------------------------------------------

void R11WaveStreamer::Pause()
{
  _isPlaying = false;
}

//-----------------------------------------------------------------------------

void R11WaveStreamer::Stop()
{
  _busyMutex.lock();

  _sampleIndex = 0;
  _isPlaying = false;

  _busyMutex.unlock();
}

//-----------------------------------------------------------------------------

bool R11WaveStreamer::IsPlaying()
{
  return _isPlaying;
}

//=============================================================================

void R11WaveStreamer::Process_()
{
  LoadFile( std::get< 0 >( input_ ) );
  std::get< 1 >( input_ ) ? Play() : Pause();

  if( _isPlaying && _waveData.size() > 0 )
  {
    _busyMutex.lock();

    uint_fast16_t index = 0;
    for( uint_fast16_t i = 0; i < _bufferSize * 2; i += 2 )
    {
      _leftChannel[index++] = ( float ) _waveData[_sampleIndex + i] * _shortToFloatCoeff;
    }

    index = 0;
    for( uint_fast16_t i = 1; i < _bufferSize * 2; i += 2 )
    {
      _rightChannel[index++] = ( float ) _waveData[_sampleIndex + i] * _shortToFloatCoeff;
    }

    _sampleIndex += _bufferSize * 2;
    _sampleIndex %= _waveData.size() - _bufferSize * 2;

    _busyMutex.unlock();

    std::get< 0 >( output_ ) = _leftChannel;
    std::get< 1 >( output_ ) = _rightChannel;
  }
  else
  {
    std::get< 0 >( output_ ) = {};
    std::get< 1 >( output_ ) = {};
  }
}

//=============================================================================