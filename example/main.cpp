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

#include "R11MicroWaveMixer.h"

//=============================================================================

int main()
{
  R11MicroWaveMixer mixer( Route11::R11ThreadConfig::ThreadPerCore );

  mixer.SetInput< R11MicroWaveMixer::File >( "../Tchaikovski-Swan-Lake-Scene.wav" );
  mixer.SetInput< R11MicroWaveMixer::Play >( true );
  mixer.SetInput< R11MicroWaveMixer::Fade1 >( 0.75 );
  mixer.SetInput< R11MicroWaveMixer::Fade2 >( 0.75 );

  for( int i = 0; i < 1500; ++i )
  {
    mixer.Tick();
  }

  return 0;
}

//=============================================================================
