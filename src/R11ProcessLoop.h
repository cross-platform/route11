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

#ifndef R11PROCESSLOOP_H
#define R11PROCESSLOOP_H

//-----------------------------------------------------------------------------

#include <cstdint>

//=============================================================================

namespace Route11
{

// this class defines a feedback loop from a Process' outputs to its own inputs

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
    // set internal _process buffer count
    _process.SetBufferCount( bufferCount );
  }

  //-----------------------------------------------------------------------------

  void Tick( int_fast8_t threadNo = -1 )
  {
    // 1. tick _process
    _process.Tick( threadNo );

    // 2. transfer feedback signals
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

    // transfer signals recursively until all signals are satisfied
    _TransferSignals< nextOutput, nextInput... >( threadNo );
  }

  //-----------------------------------------------------------------------------

  template< uint_fast16_t output, uint_fast16_t input >
  void _TransferSignals( int_fast8_t threadNo = -1 )
  {
    // transfer last signal
    _process.template SetInput< input >( _process.template GetOutput< output >( threadNo ), threadNo );
  }

  //-----------------------------------------------------------------------------
};

}

//=============================================================================

#endif // R11PROCESSLOOP_H
