#ifndef R11PROCESSLOOP_H
#define R11PROCESSLOOP_H

//-----------------------------------------------------------------------------

namespace Route11
{

template< unsigned int CinputCount, unsigned int CoutputCount, typename CT,
          unsigned int fromOutput, unsigned int... toInput >

class R11ProcessLoop
{
  static_assert( CinputCount <= CT::inputCount, "Input count provided is larger than available inputs" );
  static_assert( CoutputCount <= CT::outputCount, "Output count provided is larger than available outputs" );

private:
  CT _process;

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
    _process.template SetInput< input >( _process.template GetOutput< output >( threadNo ), threadNo );
  }

public:
  void SetBufferCount( char bufferCount )
  {
    _process.SetBufferCount( bufferCount );
  }

  void Tick( char threadNo = -1 )
  {
    _process.Tick( threadNo );

    _TransferSignals< fromOutput, toInput... >( threadNo );
  }

  template< int input, typename T >
  void SetInput( const T& value, char threadNo = -1 )
  {
    _process.template SetInput< input >( value, threadNo );
  }

  template< int input >
  auto GetInput( char threadNo = -1 ) -> decltype( _process.template GetInput< input >( threadNo ) )
  {
    return _process.template GetInput< input >( threadNo );
  }

  template< int output >
  auto GetOutput( char threadNo = -1 ) -> decltype( _process.template GetOutput< output >( threadNo ) )
  {
    return _process.template GetOutput< output >( threadNo );
  }

  static const unsigned int inputCount = CinputCount;
  static const unsigned int outputCount = CoutputCount;
};

}

//-----------------------------------------------------------------------------

#endif // R11PROCESSLOOP_H
