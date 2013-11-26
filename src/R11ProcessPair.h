/************************************************************************
 Route11 - C++ Flow-Based Metaprogramming Library
 Copyright (c) 2013 Marcus Tomlinson

 This file is part of Route11.

 Simplified BSD Licence:
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are
 met:

 Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 ************************************************************************/

#ifndef R11PROCESSPAIR_H
#define R11PROCESSPAIR_H

#include <cstdint>

namespace Route11
{

/// This class defines either a parallel or serial connected pair of processes

template< uint_fast16_t P1inputCount, uint_fast16_t P1outputCount, typename P1T, uint_fast16_t P2inputCount,
    uint_fast16_t P2outputCount, typename P2T, uint_fast16_t P1fromOutput = 0, uint_fast16_t ... P2ToInput >

class R11ProcessPair
{
  static_assert( P1inputCount <= P1T::inputCount, "Input count provided for P1 is larger than available inputs" );
  static_assert( P1outputCount <= P1T::outputCount, "Output count provided for P1 is larger than available outputs" );
  static_assert( P2inputCount <= P2T::inputCount, "Input count provided for P2 is larger than available inputs" );
  static_assert( P2outputCount <= P2T::outputCount, "Output count provided for P2 is larger than available outputs" );

private:
  template< uint_fast16_t input >
  static constexpr uint_fast16_t _ToComp()
  {
    return input < P1inputCount ? 0 : 1;
  }

  template< uint_fast16_t input >
  static constexpr uint_fast16_t _ToIndex()
  {
    return input - ( input < P1inputCount ? 0 : P1inputCount );
  }

  template< uint_fast16_t output >
  static constexpr uint_fast16_t _FromComp()
  {
    return output < P1outputCount ? 0 : 1;
  }

  template< uint_fast16_t output >
  static constexpr uint_fast16_t _FromIndex()
  {
    return output - ( output < P1outputCount ? 0 : P1outputCount );
  }

private:
  std::pair< P1T, P2T > _processes;

public:
  void SetBufferCount( int_fast8_t bufferCount )
  {
    // set both internal processes' buffer counts
    _processes.first.SetBufferCount( bufferCount );
    _processes.second.SetBufferCount( bufferCount );
  }

  void Tick( int_fast8_t threadNo = -1 )
  {
    // 1. tick first process
    _processes.first.Tick( threadNo );

    // 2. transfer signals from first process to second process
    _TransferSignals< P1fromOutput, P2ToInput... >( threadNo );

    // 3. tick second process
    _processes.second.Tick( threadNo );
  }

  template< uint_fast16_t input, typename T >
  void SetInput( const T& value, int_fast8_t threadNo = -1 )
  {
    std::get < _ToComp< input >() > ( _processes ).template SetInput< _ToIndex< input >() >( value, threadNo );
  }

  template< uint_fast16_t input >
  auto GetInput(
      int_fast8_t threadNo = -1 ) -> decltype( std::get< _ToComp< input >() >( _processes ).template GetInput< _ToIndex< input >() >( threadNo ) )
  {
    return std::get < _ToComp< input >() > ( _processes ).template GetInput< _ToIndex< input >() >( threadNo );
  }

  template< uint_fast16_t output >
  auto GetOutput(
      int_fast8_t threadNo = -1 ) -> decltype( std::get< _FromComp< output >() >( _processes ).template GetOutput< _FromIndex< output >() >( threadNo ) )
  {
    return std::get < _FromComp< output >() > ( _processes ).template GetOutput< _FromIndex< output >() >( threadNo );
  }

public:
  static const uint_fast16_t inputCount = P1inputCount + P2inputCount;
  static const uint_fast16_t outputCount = P1outputCount + P2outputCount;

private:
  template< uint_fast16_t output, uint_fast16_t input, uint_fast16_t nextOutput, uint_fast16_t ... nextInput >
  void _TransferSignals( int_fast8_t threadNo = -1 )
  {
    _TransferSignals< output, input >( threadNo );

    // transfer signals recursively until all signals are satisfied
    _TransferSignals< nextOutput, nextInput... >( threadNo );
  }

  template< uint_fast16_t output, uint_fast16_t input >
  void _TransferSignals( int_fast8_t threadNo = -1 )
  {
    // transfer last signal
    _processes.second.template SetInput< input >( _processes.first.template GetOutput< output >( threadNo ), threadNo );
  }

  template< uint_fast16_t oddIndex >
  void _TransferSignals( int_fast8_t threadNo = -1 )
  {
    unused( threadNo );
  }
};

} // namespace Route11

#endif // R11PROCESSPAIR_H
