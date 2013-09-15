#ifndef R11COMPONENTLOOP_H
#define R11COMPONENTLOOP_H

//-----------------------------------------------------------------------------

namespace Route11
{

template< typename CT, int fromOutput, int... toInput >
class R11ComponentLoop
{
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
    _component.Tick( threadNo );

    _TransferSignals< fromOutput, toInput... >( threadNo );
  }

  void Reset( char threadNo = -1 )
  {
    _component.Reset( threadNo );
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

  static const unsigned int inputCount = CT::inputCount;
  static const unsigned int outputCount = CT::outputCount;
};

}

//-----------------------------------------------------------------------------

#endif // R11COMPONENTLOOP_H
