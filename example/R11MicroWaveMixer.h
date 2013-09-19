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

#ifndef R11MICROWAVEMIXER_H
#define R11MICROWAVEMIXER_H

//-----------------------------------------------------------------------------

#include "../include/Route11.h"

#include "R11AudioDevice.h"
#include "R11Crossfader.h"
#include "R11WaveStreamer.h"

using Route11::R11AsyncProcess;
using Route11::R11Process;
using Route11::R11ProcessLoop;
using Route11::R11ProcessPair;

//=============================================================================

class _R11MicroWaveMixer
{
private:
  using Ws = R11Process< R11WaveStreamer >;
  using Cf = R11Process< R11Crossfader >;
  using Ad = R11Process< R11AudioDevice >;

  typedef R11ProcessPair
  < 3, 1, Cf,
    3, 1, Cf
  > CfCf;

  typedef R11ProcessPair
  < 6, 2, CfCf,
    2, 2, Ad,
    0, 0,
    1, 1
  > CfCfAd;

  typedef R11ProcessLoop
  < 8, 4, CfCfAd,
    2, 0,
    3, 3
  > _CfCfAd_;

  typedef R11ProcessPair
  < 2, 2, Ws,
    8, 4, _CfCfAd_,
    0, 1,
    1, 4
  > Ws_CfCfAd_;

public:
  using T = R11AsyncProcess< Ws_CfCfAd_ >;
};

//-----------------------------------------------------------------------------

class R11MicroWaveMixer : public _R11MicroWaveMixer::T
{
public:
  using _R11MicroWaveMixer::T::T;

  enum Inputs
  {
    File = 0,
    Play = 1,
    Fade1 = 4,
    Fade2 = 7
  };
};

//=============================================================================

#endif // R11MICROWAVEMIXER_H
