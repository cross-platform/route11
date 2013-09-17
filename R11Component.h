#ifndef R11COMPONENT_H
#define R11COMPONENT_H

//-----------------------------------------------------------------------------

#include "R11ComponentThread.h"

#include <vector>

//=============================================================================

namespace Route11
{

template< typename Process >
class R11Component
{
private:
  Process _process;

public:
  ~R11Component();

  void SetThreadCount( char threadCount );

  void Tick();

  template< int input, typename T >
  void SetInput( const T& value );

  template< int output >
  auto GetInput() -> decltype( _process.template GetInput< output >() );

  template< int output >
  auto GetOutput() -> decltype( _process.template GetOutput< output >() );

public:
  static const unsigned int inputCount = Process::inputCount;
  static const unsigned int outputCount = Process::outputCount;

private:
  void _ThreadTick();

private:
  std::vector< R11ComponentThread > threads_;
  char threadCount_ = 0;
  char currentThread_ = 0;
};

//=============================================================================

template< typename Process >
R11Component< Process >::~R11Component()
{
  SetThreadCount( 0 );
}

//=============================================================================

template< typename Process >
void R11Component< Process >::SetThreadCount( char threadCount )
{
  while( threadCount_ != 0 && currentThread_ != 0 )
  {
    _ThreadTick();
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

template< typename Process >
void R11Component< Process >::Tick()
{
  if( threadCount_ > 0 )
  {
    _ThreadTick();
  }
  else
  {
    _process.Tick();
  }
}

//-----------------------------------------------------------------------------

template< typename Process >
template< int input, typename T >
void R11Component< Process >::SetInput( const T& value )
{
  if( threadCount_ > 0 )
  {
    threads_[currentThread_].Sync();
    _process.template SetInput< input >( value, currentThread_ );
  }
  else
  {
    _process.template SetInput< input >( value );
  }
}

//-----------------------------------------------------------------------------

template< typename Process >
template< int output >
auto R11Component< Process >::GetInput() -> decltype( _process.template GetInput< output >() )
{
  if( threadCount_ > 0 )
  {
    threads_[currentThread_].Sync();
    return _process.template GetInput< output >( currentThread_ );
  }
  else
  {
    return _process.template GetInput< output >();
  }
}

//-----------------------------------------------------------------------------

template< typename Process >
template< int output >
auto R11Component< Process >::GetOutput() -> decltype( _process.template GetOutput< output >() )
{
  if( threadCount_ > 0 )
  {
    threads_[currentThread_].Sync();
    return _process.template GetOutput< output >( currentThread_ );
  }
  else
  {
    return _process.template GetOutput< output >();
  }
}

//=============================================================================

template< typename Process >
void R11Component< Process >::_ThreadTick()
{
  threads_[currentThread_].Sync();
  threads_[currentThread_].Resume();
  ++currentThread_ %= threadCount_;
}

}

//=============================================================================

#endif // R11COMPONENT_H
