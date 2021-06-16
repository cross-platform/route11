/************************************************************************
 Route11 - C++ Flow-Based Metaprogramming Library
 Copyright (c) 2021 Marcus Tomlinson

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

#include <R11PsMicWaveMixer.h>

/// this application instantiates a Route11 "Mic/Wave Mixer" process system,
/// sets the input wave file and crossfade levels, then ticks the system 2500
/// times before exiting.

template <auto inComp_t,
          auto outComp_t,
          int output_t,
          int input_t,
          auto nextOutComp_t = nullptr,
          int nextOutput_t = 0,
          auto... nextInput_t>
struct R11ProcessChain
{
    //    static auto outComp = outComp_t;
    //    static const int output = output_t;
    //    static auto inComp = inComp_t;
    //    static const int input = input_t;
    void Tick() const
    {
    }
};

template <auto chain1, auto chain2 = nullptr, auto chain3 = nullptr, auto... chains>
struct R11Circuit
{
    void Tick()
    {
        TickChain<chain1, chain2, chain3, chains...>();
    }

    template <auto chain_1, auto chain_2, auto chain_3, auto... chain_s>
    void TickChain()
    {
        if constexpr ( chain_1 != nullptr )
            chain_1->Tick();
        if constexpr ( chain_2 != nullptr )
            chain_2->Tick();
        TickChain<chain_3, chain_s...>();
    }

    template <auto chain_1, auto chain_2>
    void TickChain()
    {
        if constexpr ( chain_1 != nullptr )
            chain_1->Tick();
        if constexpr ( chain_2 != nullptr )
            chain_2->Tick();
    }

    template <auto chain_1>
    void TickChain()
    {
        if constexpr ( chain_1 != nullptr )
            chain_1->Tick();
    }
};

struct MyCircuit
{
    static inline R11Process<R11PpWaveStreamer> WS;
    static inline R11Process<R11PpCrossfader> CF1;
    static inline R11Process<R11PpCrossfader> CF2;
    static inline R11Process<R11PpAudioDevice> AD;

    static inline R11ProcessChain<&CF1, &AD, 0, 0, &WS, 0, 1> ch1;
    static inline R11ProcessChain<&CF2, &AD, 1, 0, &WS, 1, 1> ch2;
    static inline R11ProcessChain<&AD, &CF1, 0, 0, &CF2, 0, 1> ch3;

    R11Circuit<&ch1, &ch2, &ch3> circuit;
};

int main()
{
    // create a "Mic/Wave Mixer" process system
    //    R11PsMicWaveMixer mixer( Route11::R11ThreadConfig::ThreadPerCore );

    //    // set mixer input values
    //    mixer.SetInput<R11PsMicWaveMixer::File>( EXAMPLE_WAV_FILE );
    //    mixer.SetInput<R11PsMicWaveMixer::Play>( true );
    //    mixer.SetInput<R11PsMicWaveMixer::Fade1>( 0.75 );
    //    mixer.SetInput<R11PsMicWaveMixer::Fade2>( 0.75 );

    //    // tick mixer system
    //    for ( int i = 0; i < 2500; ++i )
    //    {
    //        mixer.Tick();
    //    }

    MyCircuit c;
    // tick mixer system
    for ( int i = 0; i < 2500; ++i )
    {
        c.circuit.Tick();
    }

    return 0;
}
