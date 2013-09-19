#ifndef R11ASYNCPROCESS_H
#define R11ASYNCPROCESS_H

//-----------------------------------------------------------------------------

#include "R11AsyncProcessThread.h"

#include <algorithm>
#include <cstdint>
#include <vector>

//=============================================================================

namespace Route11
{

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
  auto GetInput() -> decltype( _process.template GetInput< input >() );

  template< uint_fast16_t output >
  auto GetOutput() -> decltype( _process.template GetOutput< output >() );

public:
  static const uint_fast16_t inputCount = PT::inputCount;
  static const uint_fast16_t outputCount = PT::outputCount;

private:
  std::vector< R11AsyncProcessThread > threads_;
  int_fast8_t threadCount_ = 0;
  int_fast8_t currentThread_ = 0;
};

//=============================================================================

template< typename PT >
R11AsyncProcess< PT >::R11AsyncProcess( int_fast8_t threadCount )
{
  SetThreadCount( threadCount );
}

//-----------------------------------------------------------------------------

template< typename PT >
R11AsyncProcess< PT >::R11AsyncProcess( R11ThreadConfig threadConfig )
: R11AsyncProcess( threadConfig == R11ThreadConfig::ThreadPerCore ?
                std::thread::hardware_concurrency() : 0 ) {}

//-----------------------------------------------------------------------------

template< typename PT >
R11AsyncProcess< PT >::~R11AsyncProcess()
{
  SetThreadCount( 0 );
}

//=============================================================================

template< typename PT >
void R11AsyncProcess< PT >::SetThreadCount( int_fast8_t threadCount )
{
  if( threadCount < 0 || threadCount == threadCount_ )
  {
    return;
  }

  for_each( begin( threads_ ) + currentThread_, end( threads_ ),
            [ this ]( R11AsyncProcessThread& thread )
            {
              thread.Sync();
              thread.Resume();
            } );

  currentThread_ = 0;

  for( auto& thread : threads_ )
  {
    thread.Sync();
  }

  threadCount_ = 0;

  threads_.resize( threadCount );

  for( uint_fast8_t i = 0; i < threads_.size(); ++i )
  {
    threads_[ i ].Initialise( std::bind( &PT::Tick, &_process, i ) );
  }

  _process.SetBufferCount( threadCount );

  threadCount_ = threadCount;
}

//-----------------------------------------------------------------------------

template< typename PT >
void R11AsyncProcess< PT >::Tick()
{
  if( threadCount_ > 0 )
  {
    threads_[ currentThread_ ].Sync();
    threads_[ currentThread_ ].Resume();
    ++currentThread_ %= threadCount_;
  }
  else
  {
    _process.Tick();
  }
}

//-----------------------------------------------------------------------------

template< typename PT >
template< uint_fast16_t input, typename T >
void R11AsyncProcess< PT >::SetInput( const T& value )
{
  if( threadCount_ > 0 )
  {
    for( uint_fast8_t i = 0; i < threads_.size(); ++i )
    {
      threads_[ i ].Sync();
      _process.template SetInput< input >( value, i );
    }
  }
  else
  {
    _process.template SetInput< input >( value );
  }
}

//-----------------------------------------------------------------------------

template< typename PT >
template< uint_fast16_t input >
auto R11AsyncProcess< PT >::GetInput() -> decltype( _process.template GetInput< input >() )
{
  if( threadCount_ > 0 )
  {
    threads_[ currentThread_ ].Sync();
    return _process.template GetInput< input >( currentThread_ );
  }
  else
  {
    return _process.template GetInput< input >();
  }
}

//-----------------------------------------------------------------------------

template< typename PT >
template< uint_fast16_t output >
auto R11AsyncProcess< PT >::GetOutput() -> decltype( _process.template GetOutput< output >() )
{
  if( threadCount_ > 0 )
  {
    threads_[ currentThread_ ].Sync();
    return _process.template GetOutput< output >( currentThread_ );
  }
  else
  {
    return _process.template GetOutput< output >();
  }
}

}

//=============================================================================

#endif // R11ASYNCPROCESS_H
