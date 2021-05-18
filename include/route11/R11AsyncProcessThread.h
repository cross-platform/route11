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

#include <condition_variable>
#include <functional>
#include <thread>

namespace Route11
{

enum class R11ThreadConfig
{
    SingleThreaded,  // use main application thread to do all processing
    ThreadPerCore    // use one thread per processor core for processing
};

/// This class is used by R11AsyncProcess to control each process thread

class R11AsyncProcessThread
{
public:
    R11AsyncProcessThread() = default;
    R11AsyncProcessThread( const R11AsyncProcessThread& other );

    ~R11AsyncProcessThread();

    void Initialise( std::function<void( int_fast8_t )> tickMethod );

    void Sync();

    void Resume();

private:
    void _Stop();

    void _ThreadTick();

private:
    std::function<void( int_fast8_t )> _tickMethod = nullptr;

    bool _stop = false;
    bool _stopped = false;
    bool _gotResume = false;
    bool _gotSync = true;
    std::mutex _resumeMutex;
    std::condition_variable_any _resumeCondt;
    std::condition_variable_any _syncCondt;
    std::thread _thread = std::thread( &R11AsyncProcessThread::_ThreadTick, this );
};

}  // namespace Route11
