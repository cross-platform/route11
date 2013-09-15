#ifndef R11COMPONENTPAIR_H
#define R11COMPONENTPAIR_H

//-----------------------------------------------------------------------------

#include "R11ComponentBase.h"

//-----------------------------------------------------------------------------

namespace Route11
{

template< unsigned int C1inputCount, unsigned int C1outputCount, typename C1T,
          unsigned int C2inputCount, unsigned int C2outputCount, typename C2T,
          unsigned int C1fromOutput = 0, unsigned int... C2toInput >

class R11ComponentPair : public R11ComponentBase
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
  void _TransferSignals( char threadNo = -1 )
  {
    _TransferSignals< output, input >( threadNo );
    _TransferSignals< nextOutput, nextInput... >( threadNo );
  }

  template< int output, int input >
  void _TransferSignals( char threadNo = -1 )
  {
    _components.second.template SetInput< input >( _components.first.template GetOutput< output >( threadNo ), threadNo );
  }

  template< int oddIndex >
  void _TransferSignals( char threadNo = -1 ) {}

  //-----------------------------------------------------------------------------

public:
  void Tick( char threadNo = -1 )
  {
    _components.first.Tick( threadNo );

    _TransferSignals< C1fromOutput, C2toInput... >( threadNo );

    _components.second.Tick( threadNo );
  }

  template< int input, typename T >
  void SetInput( const T& value, char threadNo = -1 )
  {
    std::get< _ToComp< input >() >( _components ).template SetInput< _ToIndex< input >() >( value, threadNo );
  }

  template< int input >
  auto GetInput( char threadNo = -1 ) -> decltype( std::get< _ToComp< input >() >( _components ).template GetInput< _ToIndex< input >() >( threadNo ) )
  {
    return std::get< _ToComp< input >() >( _components ).template GetInput< _ToIndex< input >() >( threadNo );
  }

  template< int output >
  auto GetOutput( char threadNo = -1 ) -> decltype( std::get< _FromComp< output >() >( _components ).template GetOutput< _FromIndex< output >() >( threadNo ) )
  {
    return std::get< _FromComp< output >() >( _components ).template GetOutput< _FromIndex< output >() >( threadNo );
  }

  //-----------------------------------------------------------------------------

public:
  static const unsigned int inputCount = C1inputCount + C2inputCount;
  static const unsigned int outputCount = C1outputCount + C2outputCount;
};

}

//-----------------------------------------------------------------------------

#endif // R11COMPONENTPAIR_H
