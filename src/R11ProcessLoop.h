#ifndef R11PROCESSLOOP_H
#define R11PROCESSLOOP_H

//-----------------------------------------------------------------------------

#include <cstdint>

//=============================================================================

namespace Route11
{

template< uint_fast16_t PinputCount, uint_fast16_t PoutputCount, typename PT,
          uint_fast16_t fromOutput, uint_fast16_t... toInput >

class R11ProcessLoop
{
  static_assert( PinputCount <= PT::inputCount, "Input count provided is larger than available inputs" );
  static_assert( PoutputCount <= PT::outputCount, "Output count provided is larger than available outputs" );

private:
  PT _process;

public:
  //-----------------------------------------------------------------------------

  void SetBufferCount( int_fast8_t bufferCount )
  {
    _process.SetBufferCount( bufferCount );
  }

  //-----------------------------------------------------------------------------

  void Tick( int_fast8_t threadNo = -1 )
  {
    _process.Tick( threadNo );

    _TransferSignals< fromOutput, toInput... >( threadNo );
  }

  //-----------------------------------------------------------------------------

  template< uint_fast16_t input, typename T >
  void SetInput( const T& value, int_fast8_t threadNo = -1 )
  {
    _process.template SetInput< input >( value, threadNo );
  }

  //-----------------------------------------------------------------------------

  template< uint_fast16_t input >
  auto GetInput( int_fast8_t threadNo = -1 ) -> decltype( _process.template GetInput< input >( threadNo ) )
  {
    return _process.template GetInput< input >( threadNo );
  }

  //-----------------------------------------------------------------------------

  template< uint_fast16_t output >
  auto GetOutput( int_fast8_t threadNo = -1 ) -> decltype( _process.template GetOutput< output >( threadNo ) )
  {
    return _process.template GetOutput< output >( threadNo );
  }

  //-----------------------------------------------------------------------------

public:
  static const uint_fast16_t inputCount = PinputCount;
  static const uint_fast16_t outputCount = PoutputCount;

private:
  //-----------------------------------------------------------------------------

  template< uint_fast16_t output, uint_fast16_t input, uint_fast16_t nextOutput, uint_fast16_t... nextInput >
  void _TransferSignals( int_fast8_t threadNo = -1 )
  {
    _TransferSignals< output, input >( threadNo );
    _TransferSignals< nextOutput, nextInput... >( threadNo );
  }

  //-----------------------------------------------------------------------------

  template< uint_fast16_t output, uint_fast16_t input >
  void _TransferSignals( int_fast8_t threadNo = -1 )
  {
    _process.template SetInput< input >( _process.template GetOutput< output >( threadNo ), threadNo );
  }

  //-----------------------------------------------------------------------------
};

}

//=============================================================================

#endif // R11PROCESSLOOP_H
