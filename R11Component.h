#ifndef R11COMPONENT_H
#define R11COMPONENT_H

//-----------------------------------------------------------------------------

#include "R11ComponentThread.h"

#include <cstdint>
#include <vector>

//=============================================================================

namespace Route11
{

template< typename PT >
class R11Component
{
private:
  PT _process;

public:
  explicit R11Component( int_fast8_t threadCount );
  R11Component( ThreadConfig startConfig = ThreadConfig::SingleThreaded );

  ~R11Component();

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
  void _ThreadTick();

private:
  std::vector< R11ComponentThread > threads_;
  int_fast8_t threadCount_ = 0;
  int_fast8_t currentThread_ = 0;
};

//=============================================================================

template< typename PT >
R11Component< PT >::R11Component( int_fast8_t threadCount )
{
  SetThreadCount( threadCount );
}

//-----------------------------------------------------------------------------

template< typename PT >
R11Component< PT >::R11Component( ThreadConfig threadConfig )
: R11Component( threadConfig == ThreadConfig::ThreadPerCore ?
                std::thread::hardware_concurrency() : 0 ) {}

//-----------------------------------------------------------------------------

template< typename PT >
R11Component< PT >::~R11Component()
{
  SetThreadCount( 0 );
}

//=============================================================================

template< typename PT >
void R11Component< PT >::SetThreadCount( int_fast8_t threadCount )
{
  if( threadCount < 0 || threadCount == threadCount_ )
  {
    return;
  }

  while( threadCount_ != 0 && currentThread_ != 0 )
  {
    _ThreadTick();
  }

  for( auto& thread : threads_)
  {
    thread.Sync();
  }

  threadCount_ = 0;

  threads_.resize( threadCount );

  for( auto it = begin( threads_ ); it != end( threads_ ); ++it )
  {
    it->Initialise( std::bind( &PT::Tick, &_process, std::distance( begin( threads_ ), it ) ) );
  }

  _process.SetBufferCount( threadCount );

  threadCount_ = threadCount;
}

//-----------------------------------------------------------------------------

template< typename PT >
void R11Component< PT >::Tick()
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

template< typename PT >
template< uint_fast16_t input, typename T >
void R11Component< PT >::SetInput( const T& value )
{
  if( threadCount_ > 0 )
  {
    threads_[ currentThread_ ].Sync();
    _process.template SetInput< input >( value, currentThread_ );
  }
  else
  {
    _process.template SetInput< input >( value );
  }
}

//-----------------------------------------------------------------------------

template< typename PT >
template< uint_fast16_t input >
auto R11Component< PT >::GetInput() -> decltype( _process.template GetInput< input >() )
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
auto R11Component< PT >::GetOutput() -> decltype( _process.template GetOutput< output >() )
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

//=============================================================================

template< typename PT >
void R11Component< PT >::_ThreadTick()
{
  threads_[ currentThread_ ].Sync();
  threads_[ currentThread_ ].Resume();
  ++currentThread_ %= threadCount_;
}

}

//=============================================================================

#endif // R11COMPONENT_H
