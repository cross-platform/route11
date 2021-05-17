# Route11

C++ flow-based metaprogramming library

Route11 is a powerful C++ flow-based metaprogramming library that allows you to create and route complex dataflow systems at compile-time.

Route11 uses Template Metaprogramming to construct complex "process systems" defined by static relationships between "process primitives". A process primitive is a generic host class that inherits a "process policy". The process policy defines the specific behavior of a process primitive. The general term: "process" can refer to either one of these abstractions (I.e. "2 processes connected in series" may refer to 2 primitives, 2 systems, or one of each)

## Build & Run

```
git clone https://github.com/cross-platform/route11.git
cd route11
mkdir build
cd build
cmake ..
make

./example/Route11Example
```

- *`cmake ..` will auto-detect your IDE / compiler. To manually select one, use `cmake -G`.*
- *When building for an IDE, instead of `make`, simply open the cmake generated project file.*

The source code for the `Route11Example` application run above can be found in the "example" folder. This was written to assist developers in understanding the Route11 API as well as to demonstrate how it could be used to build an audio process chain.

**Note:** This project uses the open-source library RtAudio in order to stream audio to your computer's audio device. Linux users will need to install "libasound2-dev" (ALSA) before attempting to build.
