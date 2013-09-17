#ifndef R11PROCESS_H
#define R11PROCESS_H

//-----------------------------------------------------------------------------

#include <condition_variable>
#include <cstdint>
#include <deque>
#include <thread>
#include <tuple>
#include <vector>

//=============================================================================

namespace Route11
{

template< typename Policy >
class R11Process : public Policy
{
  static_assert( !std::is_destructible< Policy >::value, "Process policy should not be destructible" );

public:
  using Policy::Policy;

  void SetBufferCount( int_fast8_t bufferCount );

  void Tick( int_fast8_t bufferNo = -1 );

  template< uint_fast16_t input, typename T >
  void SetInput( const T& value, int_fast8_t bufferNo = -1 );

  template< uint_fast16_t input >
  auto GetInput( int_fast8_t bufferNo = -1 )
  -> decltype( std::get< input >( Policy::input_ ) );

  template< uint_fast16_t output >
  auto GetOutput( int_fast8_t bufferNo = -1 )
  -> decltype( std::get< output >( Policy::output_ ) );

public:
  static const uint_fast16_t inputCount = std::tuple_size< decltype( R11Process::input_ ) >::value;
  static const uint_fast16_t outputCount = std::tuple_size< decltype( R11Process::output_ ) >::value;

private:
  void _WaitForRelease( int_fast8_t bufferNo );
  void _ReleaseBuffer( int_fast8_t bufferNo );

private:
  bool _ticked = false;

  int_fast8_t _bufferCount = 0;
  std::vector< decltype( R11Process::input_ ) > _inputBuffers;
  std::vector< decltype( R11Process::output_ ) > _outputBuffers;

  // vector of bool is fine here as it is only written to by one thread at a time
  std::vector< bool > _gotReleases;
  std::deque< std::mutex > _releaseMutexes;
  std::deque< std::condition_variable_any > _releaseCondts;
};

//=============================================================================

template< typename Policy >
void R11Process< Policy >::SetBufferCount( int_fast8_t bufferCount )
{
  if( bufferCount < 0 || bufferCount == _bufferCount )
  {
    return;
  }

  _bufferCount = 0;

  _gotReleases.resize( 0 );
  _gotReleases.resize( bufferCount );

  if( bufferCount > 0 )
  {
    _gotReleases[0] = true;
  }

  _releaseMutexes.resize( bufferCount );
  _releaseCondts.resize( bufferCount );

  _inputBuffers.resize( bufferCount );
  _outputBuffers.resize( bufferCount );

  _bufferCount = bufferCount;
}

//-----------------------------------------------------------------------------

template< typename Policy >
void R11Process< Policy >::Tick( int_fast8_t bufferNo )
{
  if( bufferNo >= 0 && bufferNo < _bufferCount )
  {
    _WaitForRelease( bufferNo );
    Policy::input_ = _inputBuffers[ bufferNo ];
  }

  Policy::Process();

  if( bufferNo >= 0 && bufferNo < _bufferCount )
  {
    _outputBuffers[ bufferNo ] = Policy::output_;
    _ReleaseBuffer( bufferNo );
  }
}

//-----------------------------------------------------------------------------

template< typename Policy >
template< uint_fast16_t input, typename T >
void R11Process< Policy >::SetInput( const T& value, int_fast8_t bufferNo )
{
  if( bufferNo >= 0 && bufferNo < _bufferCount )
  {
    std::get< input >( _inputBuffers[ bufferNo ] ) = value;
  }
  else
  {
    std::get< input >( Policy::input_ ) = value;
  }
}

//-----------------------------------------------------------------------------

template< typename Policy >
template< uint_fast16_t input >
auto R11Process< Policy >::GetInput( int_fast8_t bufferNo )
-> decltype( std::get< input >( Policy::input_ ) )
{
  if( bufferNo >= 0 && bufferNo < _bufferCount )
  {
    return std::get< input >( _inputBuffers[ bufferNo ] );
  }
  else
  {
    return std::get< input >( Policy::input_ );
  }
}

//-----------------------------------------------------------------------------

template< typename Policy >
template< uint_fast16_t output >
auto R11Process< Policy >::GetOutput( int_fast8_t bufferNo )
-> decltype( std::get< output >( Policy::output_ ) )
{
  if( bufferNo >= 0 && bufferNo < _bufferCount )
  {
    return std::get< output >( _outputBuffers[ bufferNo ] );
  }
  else
  {
    return std::get< output >( Policy::output_ );
  }
}

//=============================================================================

template< typename Policy >
void R11Process< Policy >::_WaitForRelease( int_fast8_t bufferNo )
{
  _releaseMutexes[bufferNo].lock();
  if( !_gotReleases[bufferNo] )
  {
    _releaseCondts[bufferNo].wait( _releaseMutexes[bufferNo] ); // wait for resume
  }
  _gotReleases[bufferNo] = false; // reset the release flag
  _releaseMutexes[bufferNo].unlock();
}

//=============================================================================

template< typename Policy >
void R11Process< Policy >::_ReleaseBuffer( int_fast8_t bufferNo )
{
  if( _bufferCount > 0 )
  {
    ++bufferNo %= _bufferCount;
  }

  _releaseMutexes[bufferNo].lock();
  _gotReleases[bufferNo] = true;
  _releaseCondts[bufferNo].notify_all();
  _releaseMutexes[bufferNo].unlock();
}

}

//=============================================================================

#endif // R11PROCESS_H
