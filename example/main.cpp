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
