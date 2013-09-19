/************************************************************************
Route 11 - C++11 Flow-Based Template Metaprogramming Library
Copyright (c) 2013 Marcus Tomlinson

This file is part of Route 11.

The BSD 2-Clause License:
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

#ifndef R11CROSSFADER_H
#define R11CROSSFADER_H

//=============================================================================

class R11Crossfader
{
protected:
  ~R11Crossfader() = default;

  //-----------------------------------------------------------------------------

  void Process_()
  {
    auto bufferSize = std::get< 0 >( input_ ).size();
    auto crossfade = std::get< 2 >( input_ );

    if( bufferSize != std::get< 1 >( input_ ).size() ||
        crossfade < 0 || crossfade > 1 )
    {
      std::get< 0 >( output_ ) = {};
      return;
    }

    if( _bufferSize != bufferSize )
    {
      std::get< 0 >( output_ ).resize( bufferSize );
      _bufferSize = bufferSize;
    }

    for( uint_fast32_t i = 0; i < bufferSize; ++i )
    {
      std::get< 0 >( output_ )[ i ] = ( std::get< 0 >( input_ )[ i ] * ( 1 - crossfade ) ) +
                                      ( std::get< 1 >( input_ )[ i ] * crossfade );
    }
  }

  //-----------------------------------------------------------------------------

protected:
  std::tuple< std::vector< float >, std::vector< float >, float > input_;
  std::tuple< std::vector< float > > output_;

private:
  uint_fast32_t _bufferSize = 0;
};

//=============================================================================

#endif // R11CROSSFADER_H
