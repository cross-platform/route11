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

#pragma once

#include <route11/R11AsyncProcess.h>
#include <route11/R11Process.h>
#include <route11/R11ProcessLoop.h>
#include <route11/R11ProcessPair.h>

/**
 \mainpage Welcome!

 \section intro_sec Introduction
 Route11 is a powerful C++ flow-based metaprogramming library that allows you
 to create and route complex dataflow systems at compile-time.

 Route11 uses Template Metaprogramming to construct complex "process systems"
 defined by static relationships between "process primitives". A process
 primitive is a generic host class that inherits a "process policy". The
 process policy defines the specific behavior of a process primitive. The
 general term: "process" can refer to either one of these abstractions (I.e.
 "2 processes connected in series" may refer to 2 primitives, 2 systems, or
 one of each)

 <b>NOTE</b>: Route11 was written using many new C++11 features, so it is
 required that a C++11 compatible compiler (e.g Clang or GCC) is used when
 building this project.

 \n

 \section features_sec Features
 - <b>Automatic branch synchronization</b> - The result of data diverging across parallel branches is guaranteed to arrive synchronized
 at a converging point.
 - <b>Compile-time system routing</b> - Route complex process systems at compile-time to avoid the overhead of construction at
 run-time.
 - <b>Compile-time type safety</b> - Processes only accept inputs of statically specified value types, resulting in stable, type safe
 process systems.
 - <b>Dynamic thread count adjustment</b> - Specify at run-time, the number of threads in which you require a process system to run.
 - <b>Feedback loops</b> - Create true closed-circuit systems by feeding process outputs back into previous process inputs.
 - <b>High performance parallel processing</b> - Process systems use advanced multi-threaded scheduling to maximize data flow
 efficiency.
 - <b>Non-threaded platform support</b> - Develop for (or port to) platforms without multi-threading support and execute complex
 process systems in a single thread.

 \n

 \section install_sec Getting Started

 \subsection step1 Step 1: Download Route11
 The Route11 library can be downloaded from the
 <a href="https://launchpad.net/route11/"><b>Launchpad project page</b></a>.
 Download the project archive and extract the contents anywhere you like.

 \subsection step2 Step 2: Check out the example project
 In the "example" folder (in the Route11 root directory) you will find a Route11 demo project,
 written to assist developers in understanding the Route11 API as well as to demonstrate how it
 could be used to build an audio process chain. This project uses the open-source library RtAudio
 in order to stream sound to your computer's audio device -Linux users will need to install
 "libasound2-dev" (ALSA) before attempting to build.

 \subsection step3 Step 3: Make your own process policies
 A fast way to create your own process policy could be to copy an existing one to another
 destination, rename it, and edit the contents to satisfy your process' required behavior.
 You can find a few process policy examples in the "example" folder (from Step 2) prefixed with
 "R11Pp".

 \subsection step4 Step 4: Roll your own Route11 projects
 As Route11 is not reliant on any non-standard 3rd party sources, getting a Route11 project to
 compile and run is really easy. All you need to do from your project is #include <Route11.h> from
 the "include" folder (in the Route11 root directory) and you're ready to go.

 \subsection step5 Step 5: Refer to the documentation
 Between the example project and the documentation found here, you should have enough resources
 to get started with Route11 straight away. If you have any questions or wish to report a bug,
 feel free to email me at themarcustomlinson@gmail.com.

 \n

 \section release_notes Release Notes

 \subsection v02 v.0.2 (15 October 2013)
 - Replaced Qt project with more flexible CMakeLists.
 - Adjusted code for GCC compiler.

 \subsection v01 v.0.1 (20 September 2013)
 - Initial release.
 */
