#ifndef R11COMPONENTLOOP_H
#define R11COMPONENTLOOP_H

//-----------------------------------------------------------------------------

template< typename CT, int fromOutput, int... toInput >
class R11ComponentLoop
{
private:
  CT _component;

private:
  template< int output, int input, int nextOutput, int... nextInput >
  void _TransferSignals()
  {
    _TransferSignals< output, input >();
    _TransferSignals< nextOutput, nextInput... >();
  }

  template< int output, int input >
  void _TransferSignals()
  {
    _component.template SetInput< input >( _component.template GetOutput< output >() );
  }

public:
  void Tick()
  {
    _component.Tick();

    _TransferSignals< fromOutput, toInput... >();
  }

  void Reset()
  {
    _component.Reset();
  }

  template< int input, typename T >
  void SetInput( const T& value )
  {
    _component.template SetInput< input >( value );
  }

  template< int output >
  auto GetOutput() -> decltype( _component.template GetOutput< output >() )
  {
    return _component.template GetOutput< output >();
  }

  static const unsigned int inputCount = CT::inputCount;
  static const unsigned int outputCount = CT::outputCount;
};

//-----------------------------------------------------------------------------

#endif // R11COMPONENTLOOP_H
