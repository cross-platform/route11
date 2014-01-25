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

#include <R11AsyncProcessThread.h>
#include <Route11.h>

using namespace Route11;

R11AsyncProcessThread::~R11AsyncProcessThread()
{
  _Stop();
}

R11AsyncProcessThread::R11AsyncProcessThread( const R11AsyncProcessThread& other )
{
  unused( other );
}

void R11AsyncProcessThread::Initialise( std::function< void( int_fast8_t ) > tickMethod )
{
  // store callback method for use in _ThreadTick
  _tickMethod = tickMethod;
}

void R11AsyncProcessThread::Sync()
{
  _resumeMutex.lock();

  if( !_gotSync ) // if haven't already got sync
  {
    _syncCondt.wait( _resumeMutex ); // wait for sync
  }

  _resumeMutex.unlock();
}

void R11AsyncProcessThread::Resume()
{
  _resumeMutex.lock();

  _gotSync = false; // reset the sync flag

  _gotResume = true; // set the resume flag
  _resumeCondt.notify_all();

  _resumeMutex.unlock();
}

void R11AsyncProcessThread::_Stop()
{
  // set _stop flag (notify _ThreadTick to exit)
  _stop = true;

  // wait until _ThreadTick exits
  while( !_stopped )
  {
    _syncCondt.notify_all();
    _resumeCondt.notify_all();
    std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
  }

  // join with thread
  _thread.join();
}

void R11AsyncProcessThread::_ThreadTick()
{
  while( !_stop )
  {
    _resumeMutex.lock();

    // notify sync
    _gotSync = true; // set the sync flag
    _syncCondt.notify_all();

    // wait for resume
    if( !_gotResume ) // if haven't already got resume
    {
      _resumeCondt.wait( _resumeMutex ); // wait for resume
    }
    _gotResume = false; // reset the resume flag

    _resumeMutex.unlock();

    if( !_stop && _tickMethod != nullptr )
    {
      _tickMethod( 0 );
    }
  }

  // set _stopped flag (notify _Stop that the thread has exited)
  _stopped = true;
}
