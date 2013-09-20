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

#include "R11PsMicWaveMixer.h"

//=============================================================================

// this application instantiates a Route11 "Mic/Wave Mixer" Process System,
// sets the input wave file and crossfade levels, then ticks the system 1500
// times before exiting.

int main()
{
  // create a "Mic/Wave Mixer" Process System
  R11PsMicWaveMixer mixer( Route11::R11ThreadConfig::ThreadPerCore );

  // set mixer input values
  mixer.SetInput< R11PsMicWaveMixer::File >( "../Tchaikovski-Swan-Lake-Scene.wav" );
  mixer.SetInput< R11PsMicWaveMixer::Play >( true );
  mixer.SetInput< R11PsMicWaveMixer::Fade1 >( 0.75 );
  mixer.SetInput< R11PsMicWaveMixer::Fade2 >( 0.75 );

  // tick mixer system
  for( int i = 0; i < 1500; ++i )
  {
    mixer.Tick();
  }

  return 0;
}

//=============================================================================
