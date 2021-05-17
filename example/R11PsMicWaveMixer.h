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

#ifndef R11PSMICWAVEMIXER_H
#define R11PSMICWAVEMIXER_H

#include <Route11.h>

#include <R11PpAudioDevice.h>
#include <R11PpCrossfader.h>
#include <R11PpWaveStreamer.h>

using Route11::R11AsyncProcess;
using Route11::R11Process;
using Route11::R11ProcessLoop;
using Route11::R11ProcessPair;

/// this class is used to route a "Mic/Wave Mixer" process system
/// (refer to the "doc" folder for detailed diagrams of each routing step)

class _R11PsMicWaveMixer
{
private:
  // step 1 : define aliases for processes required
  // (R11PsMicWaveMixer - Step 1.png)
  using Ws = R11Process< R11PpWaveStreamer >;
  using Cf = R11Process< R11PpCrossfader >;
  using Ad = R11Process< R11PpAudioDevice >;

  // step 2 : pair 2 crossfaders
  // (R11PsMicWaveMixer - Step 2.png)
  typedef R11ProcessPair< 3, 1, Cf, 3, 1, Cf > CfCf;

  // step 3 : pair and connect the crossfader pair to an audio device
  // (R11PsMicWaveMixer - Step 3.png)
  typedef R11ProcessPair< 6, 2, CfCf, 2, 2, Ad, 0, 0, 1, 1 > CfCfAd;

  // step 4 : feed audio device outputs back into the crossfader pair
  // (R11PsMicWaveMixer - Step 4.png)
  typedef R11ProcessLoop< 8, 4, CfCfAd, 2, 0, 3, 3 > _CfCfAd_;

  // step 5 : pair and connect a wave streamer to the crossfader inputs
  // (R11PsMicWaveMixer - Step 5.png)
  typedef R11ProcessPair< 2, 2, Ws, 8, 4, _CfCfAd_, 0, 1, 1, 4 > Ws_CfCfAd_;

public:
  // define public alias "T" as system type
  // (async allows system to be multi-threaded)
  using T = R11AsyncProcess< Ws_CfCfAd_ >;
};

// this class is used to instantiate a "Mic/Wave Mixer" process system

class R11PsMicWaveMixer : public _R11PsMicWaveMixer::T
{
public:
  // inherit base constructors
  using _R11PsMicWaveMixer::T::T;

  // define user-friendly input enumeration for end user
  enum Inputs
  {
    File = 0, Play = 1, Fade1 = 4, Fade2 = 7
  };
};

#endif // R11PSMICWAVEMIXER_H
