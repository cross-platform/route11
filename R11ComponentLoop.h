#ifndef R11COMPONENTLOOP_H
#define R11COMPONENTLOOP_H

//-----------------------------------------------------------------------------

#include "R11ComponentBase.h"

#include <thread>

//-----------------------------------------------------------------------------

namespace Route11
{

template< unsigned int CinputCount, unsigned int CoutputCount, typename CT,
          unsigned int fromOutput, unsigned int... toInput >

class R11ComponentLoop : public R11ComponentBase
{
  static_assert( CinputCount <= CT::inputCount, "Input count provided is larger than available inputs" );
  static_assert( CoutputCount <= CT::outputCount, "Output count provided is larger than available outputs" );

private:
  CT _component;

private:
  template< int output, int input, int nextOutput, int... nextInput >
  void _TransferSignals( char threadNo = -1 )
  {
    _TransferSignals< output, input >( threadNo );
    _TransferSignals< nextOutput, nextInput... >( threadNo );
  }

  template< int output, int input >
  void _TransferSignals( char threadNo = -1 )
  {
    _component.template SetInput< input >( _component.template GetOutput< output >( threadNo ), threadNo );
  }

public:
  void Tick( char threadNo = -1 )
  {
    if( threadNo == -1 && threadCount_ > 0 )
    {
      std::thread t1( [](){} );
      t1.join();
    }

    _component.Tick( threadNo );

    _TransferSignals< fromOutput, toInput... >( threadNo );
  }

  template< int input, typename T >
  void SetInput( const T& value, char threadNo = -1 )
  {
    _component.template SetInput< input >( value, threadNo );
  }

  template< int input >
  auto GetInput( char threadNo = -1 ) -> decltype( _component.template GetInput< input >( threadNo ) )
  {
    return _component.template GetInput< input >( threadNo );
  }

  template< int output >
  auto GetOutput( char threadNo = -1 ) -> decltype( _component.template GetOutput< output >( threadNo ) )
  {
    return _component.template GetOutput< output >( threadNo );
  }

  static const unsigned int inputCount = CinputCount;
  static const unsigned int outputCount = CoutputCount;
};

}

//-----------------------------------------------------------------------------

#endif // R11COMPONENTLOOP_H
