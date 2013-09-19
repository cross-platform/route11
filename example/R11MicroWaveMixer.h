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
