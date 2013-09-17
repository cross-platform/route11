#ifndef R11COMPONENT_H
#define R11COMPONENT_H

//-----------------------------------------------------------------------------

#include "R11ComponentThread.h"

#include <vector>

//-----------------------------------------------------------------------------

namespace Route11
{

template< typename Process >
class R11Component
{
private:
  Process _process;

  std::vector< R11ComponentThread > threads_;
  char threadCount_ = 0;
  char currentThread_ = 0;

  void ThreadTick()
  {
    threads_[currentThread_].Sync();
    threads_[currentThread_].Resume();
    ++currentThread_ %= threadCount_;
  }

  //-----------------------------------------------------------------------------

public:
  ~R11Component()
  {
    SetThreadCount( 0 );
  }

  void SetThreadCount( char threadCount )
  {
    while( threadCount_ != 0 && currentThread_ != 0 )
    {
      ThreadTick();
    }

    for( auto& thread : threads_ )
    {
      thread.Sync();
    }

    threadCount_ = 0;

    threads_.resize( threadCount );

    for( unsigned char i = 0; i < threads_.size(); ++i )
    {
      threads_[i].Initialise( std::bind( &Process::Tick, &_process, i ) );
    }

    _process.SetBufferCount( threadCount );

    threadCount_ = threadCount;
  }

  //-----------------------------------------------------------------------------

  void Tick()
  {
    if( threadCount_ > 0 )
    {
      ThreadTick();
    }
    else
    {
      _process.Tick();
    }
  }

  //-----------------------------------------------------------------------------

  template< int input, typename T >
  void SetInput( const T& value )
  {
    _process.template SetInput< input >( value );
  }

  //-----------------------------------------------------------------------------

  template< int output >
  auto GetInput() -> decltype( _process.template GetInput< output >() )
  {
    return _process.template GetInput< output >();
  }

  //-----------------------------------------------------------------------------

  template< int output >
  auto GetOutput() -> decltype( _process.template GetOutput< output >() )
  {
    return _process.template GetOutput< output >();
  }

public:
  static const unsigned int inputCount = Process::inputCount;
  static const unsigned int outputCount = Process::outputCount;
};

}

//-----------------------------------------------------------------------------

#endif // R11COMPONENT_H
