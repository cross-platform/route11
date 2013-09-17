#ifndef R11PROCESSPAIR_H
#define R11PROCESSPAIR_H

//=============================================================================

namespace Route11
{

template< unsigned int P1inputCount, unsigned int P1outputCount, typename P1T,
          unsigned int P2inputCount, unsigned int P2outputCount, typename P2T,
          unsigned int P1fromOutput = 0, unsigned int... P2ToInput >

class R11ProcessPair
{
  static_assert( P1inputCount <= P1T::inputCount, "Input count provided for P1 is larger than available inputs" );
  static_assert( P1outputCount <= P1T::outputCount, "Output count provided for P1 is larger than available outputs" );
  static_assert( P2inputCount <= P2T::inputCount, "Input count provided for P2 is larger than available inputs" );
  static_assert( P2outputCount <= P2T::outputCount, "Output count provided for P2 is larger than available outputs" );

private:
  //-----------------------------------------------------------------------------

  template< int input >
  static constexpr int _ToComp()
  {
    return input < P1inputCount ? 0 : 1;
  }

  //-----------------------------------------------------------------------------

  template< int input >
  static constexpr int _ToIndex()
  {
    return input - ( input < P1inputCount ? 0 : P1inputCount );
  }

  //-----------------------------------------------------------------------------

  template< int output >
  static constexpr int _FromComp()
  {
    return output < P1outputCount ? 0 : 1;
  }

  //-----------------------------------------------------------------------------

  template< int output >
  static constexpr int _FromIndex()
  {
    return output - ( output < P1outputCount ? 0 : P1outputCount );
  }

  //-----------------------------------------------------------------------------

private:
  std::pair< P1T, P2T > _processes;

public:
  //-----------------------------------------------------------------------------

  void SetBufferCount( char bufferCount )
  {
    _processes.first.SetBufferCount( bufferCount );
    _processes.second.SetBufferCount( bufferCount );
  }

  //-----------------------------------------------------------------------------

  void Tick( char threadNo = -1 )
  {
    _processes.first.Tick( threadNo );

    _TransferSignals< P1fromOutput, P2ToInput... >( threadNo );

    _processes.second.Tick( threadNo );
  }

  //-----------------------------------------------------------------------------

  template< int input, typename T >
  void SetInput( const T& value, char threadNo = -1 )
  {
    std::get< _ToComp< input >() >( _processes ).template SetInput< _ToIndex< input >() >( value, threadNo );
  }

  //-----------------------------------------------------------------------------

  template< int input >
  auto GetInput( char threadNo = -1 )
  -> decltype( std::get< _ToComp< input >() >( _processes ).template GetInput< _ToIndex< input >() >( threadNo ) )
  {
    return std::get< _ToComp< input >() >( _processes ).template GetInput< _ToIndex< input >() >( threadNo );
  }

  //-----------------------------------------------------------------------------

  template< int output >
  auto GetOutput( char threadNo = -1 )
  -> decltype( std::get< _FromComp< output >() >( _processes ).template GetOutput< _FromIndex< output >() >( threadNo ) )
  {
    return std::get< _FromComp< output >() >( _processes ).template GetOutput< _FromIndex< output >() >( threadNo );
  }

  //-----------------------------------------------------------------------------

public:
  static const unsigned int inputCount = P1inputCount + P2inputCount;
  static const unsigned int outputCount = P1outputCount + P2outputCount;

private:
  //-----------------------------------------------------------------------------

  template< int output, int input, int nextOutput, int... nextInput >
  void _TransferSignals( char threadNo = -1 )
  {
    _TransferSignals< output, input >( threadNo );
    _TransferSignals< nextOutput, nextInput... >( threadNo );
  }

  //-----------------------------------------------------------------------------

  template< int output, int input >
  void _TransferSignals( char threadNo = -1 )
  {
    _processes.second.template SetInput< input >( _processes.first.template GetOutput< output >( threadNo ), threadNo );
  }

  //-----------------------------------------------------------------------------

  template< int oddIndex >
  void _TransferSignals( char threadNo = -1 ) {}

  //-----------------------------------------------------------------------------
};

}

//=============================================================================

#endif // R11PROCESSPAIR_H
