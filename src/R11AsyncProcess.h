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

#ifndef R11ASYNCPROCESS_H
#define R11ASYNCPROCESS_H

#include "R11AsyncProcessThread.h"

#include <algorithm>
#include <cstdint>
#include <vector>

namespace Route11
{

/// This class enables a Route11 process to be multi-threaded

template< typename PT >
class R11AsyncProcess
{
private:
  PT _process;

public:
  explicit R11AsyncProcess( int_fast8_t threadCount = 0 );
  R11AsyncProcess( R11ThreadConfig threadConfig );

  ~R11AsyncProcess();

  void SetThreadCount( int_fast8_t threadCount );

  void Tick();

  template< uint_fast16_t input, typename T >
  void SetInput( const T& value );

  template< uint_fast16_t input >
  auto GetInput() -> decltype( _process.template GetInput< input >() )
  {
    // if multi-threaded, sync with current thread then get it's current input
    if( threadCount_ > 0 )
    {
      threads_[currentThread_].Sync();
      return _process.template GetInput< input >( currentThread_ );
    }
    // else get input normally
    else
    {
      return _process.template GetInput< input >();
    }
  }

  template< uint_fast16_t output >
  auto GetOutput() -> decltype( _process.template GetOutput< output >() )
  {
    // if multi-threaded, sync with current thread then get it's current output
    if( threadCount_ > 0 )
    {
      threads_[currentThread_].Sync();
      return _process.template GetOutput< output >( currentThread_ );
    }
    // else get output normally
    else
    {
      return _process.template GetOutput< output >();
    }
  }

public:
  static const uint_fast16_t inputCount = PT::inputCount;
  static const uint_fast16_t outputCount = PT::outputCount;

private:
  std::vector< R11AsyncProcessThread > threads_;
  int_fast8_t threadCount_ = 0;
  int_fast8_t currentThread_ = 0;
};

template< typename PT >
R11AsyncProcess< PT >::R11AsyncProcess( int_fast8_t threadCount )
{
  SetThreadCount( threadCount );
}

template< typename PT >
R11AsyncProcess< PT >::R11AsyncProcess( R11ThreadConfig threadConfig )
    : R11AsyncProcess( threadConfig == R11ThreadConfig::ThreadPerCore ? std::thread::hardware_concurrency() : 0 )
{
}

template< typename PT >
R11AsyncProcess< PT >::~R11AsyncProcess()
{
  SetThreadCount( 0 );
}

template< typename PT >
void R11AsyncProcess< PT >::SetThreadCount( int_fast8_t threadCount )
{
  if( threadCount < 0 || threadCount == threadCount_ )
  {
    return;
  }

  // manually tick until 0
  for_each( begin( threads_ ) + currentThread_, end( threads_ ), [ this ]( R11AsyncProcessThread& thread )
  {
    thread.Sync();
    thread.Resume();
  } );

  currentThread_ = 0;

  // sync with all threads
  for( auto& thread : threads_ )
  {
    thread.Sync();
  }

  threadCount_ = 0;

  threads_.resize( threadCount );

  // initialize all threads with appropriate callback method
  for( uint_fast8_t i = 0; i < threads_.size(); ++i )
  {
    threads_[i].Initialise( std::bind( &PT::Tick, &_process, i ) );
  }

  // update _process buffer count to match new thread count
  _process.SetBufferCount( threadCount );

  threadCount_ = threadCount;
}

template< typename PT >
void R11AsyncProcess< PT >::Tick()
{
  // if multi-threaded, tick the appropriate thread
  if( threadCount_ > 0 )
  {
    threads_[currentThread_].Sync();
    threads_[currentThread_].Resume();
    ++currentThread_ %= threadCount_;
  }
  // else tick within current thread
  else
  {
    _process.Tick();
  }
}

template< typename PT >
template< uint_fast16_t input, typename T >
void R11AsyncProcess< PT >::SetInput( const T& value )
{
  // if multi-threaded, sync with and set input for all threads
  if( threadCount_ > 0 )
  {
    for( uint_fast8_t i = 0; i < threads_.size(); ++i )
    {
      threads_[i].Sync();
      _process.template SetInput< input >( value, i );
    }
  }
  // else set input normally
  else
  {
    _process.template SetInput< input >( value );
  }
}

} // namespace Route11

#endif // R11ASYNCPROCESS_H
