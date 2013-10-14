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

#ifndef R11PROCESS_H
#define R11PROCESS_H

//-----------------------------------------------------------------------------

#include <condition_variable>
#include <cstdint>
#include <deque>
#include <memory>
#include <thread>
#include <tuple>
#include <vector>

//=============================================================================

namespace Route11
{

/// This is a process policy host class used to form Route11 process primitives

template< typename PP >
class R11Process : public PP
{
  static_assert( !std::is_destructible< PP >::value, "Process policy should not be destructible" );

public:
  using PP::PP;

  void SetBufferCount( int_fast8_t bufferCount );

  void Tick( int_fast8_t bufferNo = -1 );

  template< uint_fast16_t input, typename T >
  void SetInput( const T& value, int_fast8_t bufferNo = -1 );

  //-----------------------------------------------------------------------------

  template< uint_fast16_t input >
  auto GetInput( int_fast8_t bufferNo ) -> decltype( std::get< input >( this->input_ ) )
  {
    // if multi-threaded, get the requested buffer's input
    if( bufferNo >= 0 && bufferNo < _bufferCount )
    {
      return std::get< input >( _inputBuffers[ bufferNo ] );
    }
    else
    {
      return std::get< input >( PP::input_ );
    }
  }

  //-----------------------------------------------------------------------------

  template< uint_fast16_t output >
  auto GetOutput( int_fast8_t bufferNo ) -> decltype( std::get< output >( this->output_ ) )
  {
    // if multi-threaded, get the requested buffer's output
    if( bufferNo >= 0 && bufferNo < _bufferCount )
    {
      return std::get< output >( _outputBuffers[ bufferNo ] );
    }
    else
    {
      return std::get< output >( PP::output_ );
    }
  }

  //-----------------------------------------------------------------------------

public:
  static const uint_fast16_t inputCount = std::tuple_size< decltype( R11Process::input_ ) >::value;
  static const uint_fast16_t outputCount = std::tuple_size< decltype( R11Process::output_ ) >::value;

private:
  void _WaitForRelease( int_fast8_t bufferNo );
  void _ReleaseNextBuffer( int_fast8_t bufferNo );

private:
  bool _ticked = false;

  int_fast8_t _bufferCount = 0;
  std::vector< decltype( R11Process::input_ ) > _inputBuffers;
  std::vector< decltype( R11Process::output_ ) > _outputBuffers;

  std::deque< bool > _gotReleases;
  std::unique_ptr< std::mutex[] > _releaseMutexes;
  std::unique_ptr< std::condition_variable_any[] > _releaseCondts;
};

//=============================================================================

template< typename PP >
void R11Process< PP >::SetBufferCount( int_fast8_t bufferCount )
{
  if( bufferCount < 0 || bufferCount == _bufferCount )
  {
    return;
  }

  _bufferCount = 0;

  // resize internal buffers
  _inputBuffers.resize( bufferCount );
  _outputBuffers.resize( bufferCount );

  // resize _gotReleases
  _gotReleases.resize( bufferCount );

  if( bufferCount > 0 )
  {
    // set all gotReleases to false
    for( auto& gotRelease : _gotReleases )
    {
      gotRelease = false;
    }

    // set first _gotRelease to true to "kick start" the system
    _gotReleases[0] = true;
  }

  // reset the mutex and condition variable pointers to new array sizes
  _releaseMutexes.reset( new std::mutex[ bufferCount ] );
  _releaseCondts.reset( new std::condition_variable_any[ bufferCount ] );

  // update _bufferCount
  _bufferCount = bufferCount;
}

//-----------------------------------------------------------------------------

template< typename PP >
void R11Process< PP >::Tick( int_fast8_t bufferNo )
{
  // if multi-threaded, sync with previous thread before continuing
  if( bufferNo >= 0 && bufferNo < _bufferCount )
  {
    _WaitForRelease( bufferNo );
    PP::input_ = _inputBuffers[ bufferNo ];
  }

  // call PP's Process_ method
  PP::Process_();

  // if multi-threaded, notify the next waiting thread
  if( bufferNo >= 0 && bufferNo < _bufferCount )
  {
    _outputBuffers[ bufferNo ] = PP::output_;
    _ReleaseNextBuffer( bufferNo );
  }
}

//-----------------------------------------------------------------------------

template< typename PP >
template< uint_fast16_t input, typename T >
void R11Process< PP >::SetInput( const T& value, int_fast8_t bufferNo )
{
  // if multi-threaded, set the requested buffer's input
  if( bufferNo >= 0 && bufferNo < _bufferCount )
  {
    std::get< input >( _inputBuffers[ bufferNo ] ) = value;
  }
  else
  {
    std::get< input >( PP::input_ ) = value;
  }
}

//=============================================================================

template< typename PP >
void R11Process< PP >::_WaitForRelease( int_fast8_t currentBufferNo )
{
  _releaseMutexes[ currentBufferNo ].lock();
  if( !_gotReleases[ currentBufferNo ] )
  {
    _releaseCondts[ currentBufferNo ].wait( _releaseMutexes[ currentBufferNo ] ); // wait for resume
  }
  _gotReleases[ currentBufferNo ] = false; // reset the release flag
  _releaseMutexes[ currentBufferNo ].unlock();
}

//=============================================================================

template< typename PP >
void R11Process< PP >::_ReleaseNextBuffer( int_fast8_t currentBufferNo )
{
  ++currentBufferNo %= _bufferCount;

  _releaseMutexes[ currentBufferNo ].lock();
  _gotReleases[ currentBufferNo ] = true;
  _releaseCondts[ currentBufferNo ].notify_all();
  _releaseMutexes[ currentBufferNo ].unlock();
}

}

//=============================================================================

#endif // R11PROCESS_H
