#ifndef R11COMPONENTPAIR_H
#define R11COMPONENTPAIR_H

//-----------------------------------------------------------------------------

namespace Route11
{
template< unsigned int C1inputCount, unsigned int C1outputCount, typename C1T,
          unsigned int C2inputCount, unsigned int C2outputCount, typename C2T,
          unsigned int C1fromOutput = 0, unsigned int... C2toInput >

class R11ComponentPair
{
  static_assert( C1inputCount <= C1T::inputCount, "Input count provided for C1 is larger than available inputs" );
  static_assert( C1outputCount <= C1T::outputCount, "Output count provided for C1 is larger than available outputs" );
  static_assert( C2inputCount <= C2T::inputCount, "Input count provided for C2 is larger than available inputs" );
  static_assert( C2outputCount <= C2T::outputCount, "Output count provided for C2 is larger than available outputs" );

  //-----------------------------------------------------------------------------

private:
  std::pair< C1T, C2T > _components;

  //-----------------------------------------------------------------------------

private:
  template< int input >
  static constexpr int _ToComp()
  {
    return input < C1inputCount ? 0 : 1;
  }

  template< int input >
  static constexpr int _ToIndex()
  {
    return input - ( input < C1inputCount ? 0 : C1inputCount );
  }

  template< int output >
  static constexpr int _FromComp()
  {
    return output < C1outputCount ? 0 : 1;
  }

  template< int output >
  static constexpr int _FromIndex()
  {
    return output - ( output < C1outputCount ? 0 : C1outputCount );
  }

  //-----------------------------------------------------------------------------

  template< int output, int input, int nextOutput, int... nextInput >
  void _TransferSignals()
  {
    _TransferSignals< output, input >();
    _TransferSignals< nextOutput, nextInput... >();
  }

  template< int output, int input >
  void _TransferSignals()
  {
    _components.second.template SetInput< input >( _components.first.template GetOutput< output >() );
  }

  template< int oddIndex >
  void _TransferSignals() {}

  //-----------------------------------------------------------------------------

public:
  void Tick()
  {
    _components.first.Tick();

    _TransferSignals< C1fromOutput, C2toInput... >();

    _components.second.Tick();
  }

  void Reset()
  {
    _components.first.Reset();
    _components.second.Reset();
  }

  template< int input, typename T >
  void SetInput( const T& value )
  {
    std::get< _ToComp< input >() >( _components ).template SetInput< _ToIndex< input >() >( value );
  }

  template< int input >
  auto GetInput() -> decltype( std::get< _ToComp< input >() >( _components ).template GetInput< _ToIndex< input >() >() )
  {
    return std::get< _ToComp< input >() >( _components ).template GetInput< _ToIndex< input >() >();
  }

  template< int output >
  auto GetOutput() -> decltype( std::get< _FromComp< output >() >( _components ).template GetOutput< _FromIndex< output >() >() )
  {
    return std::get< _FromComp< output >() >( _components ).template GetOutput< _FromIndex< output >() >();
  }

  //-----------------------------------------------------------------------------

public:
  static const unsigned int inputCount = C1inputCount + C2inputCount;
  static const unsigned int outputCount = C1outputCount + C2outputCount;
};
}

//-----------------------------------------------------------------------------

#endif // R11COMPONENTPAIR_H
