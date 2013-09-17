#ifndef R11COMPONENTTHREAD_H
#define R11COMPONENTTHREAD_H

//-----------------------------------------------------------------------------

#include <functional>
#include <thread>
#include <condition_variable>

//=============================================================================

namespace Route11
{

class R11ComponentThread
{
public:
  R11ComponentThread() = default;
  R11ComponentThread( const R11ComponentThread& other );

  ~R11ComponentThread();

  void Initialise( std::function< void( char ) > tickMethod );

  void Sync();

  void Resume();

private:
  void _Stop();

  void _ThreadTick();

private:
  std::function< void( char ) > _tickMethod = nullptr;

  bool _stop = false;
  bool _stopped = false;
  bool _gotResume = false;
  bool _gotSync = true;
  std::mutex _resumeMutex;
  std::condition_variable_any _resumeCondt;
  std::condition_variable_any _syncCondt;
  std::thread _thread = std::thread( &R11ComponentThread::_ThreadTick, this );
};

//=============================================================================

R11ComponentThread::R11ComponentThread( const R11ComponentThread& other ) {}

//-----------------------------------------------------------------------------

R11ComponentThread::~R11ComponentThread()
{
  _Stop();
}

//=============================================================================

void R11ComponentThread::Initialise( std::function< void( char ) > tickMethod )
{
  _tickMethod = tickMethod;
}

//-----------------------------------------------------------------------------

void R11ComponentThread::Sync()
{
  _resumeMutex.lock();

  if( !_gotSync ) // if haven't already got sync
  {
    _syncCondt.wait( _resumeMutex ); // wait for sync
  }

  _resumeMutex.unlock();
}

//-----------------------------------------------------------------------------

void R11ComponentThread::Resume()
{
  _resumeMutex.lock();

  _gotSync = false; // reset the sync flag

  _gotResume = true; // set the resume flag
  _resumeCondt.notify_all();

  _resumeMutex.unlock();
}

//=============================================================================

void R11ComponentThread::_Stop()
{
  _stop = true;

  while( !_stopped )
  {
    _syncCondt.notify_all();
    _resumeCondt.notify_all();
    std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
  }

  _thread.join();
}

//-----------------------------------------------------------------------------

void R11ComponentThread::_ThreadTick()
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

  _stopped = true;
}

}

//=============================================================================

#endif // R11COMPONENTTHREAD_H
