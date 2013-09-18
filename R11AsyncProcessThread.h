#ifndef R11ASYNCPROCESSTHREAD_H
#define R11ASYNCPROCESSTHREAD_H

//-----------------------------------------------------------------------------

#include <functional>
#include <thread>
#include <condition_variable>

//=============================================================================

namespace Route11
{

enum class R11ThreadConfig
{
  SingleThreaded,
  ThreadPerCore
};

//=============================================================================

class R11AsyncProcessThread
{
public:
  R11AsyncProcessThread() = default;
  R11AsyncProcessThread( const R11AsyncProcessThread& other ) { unused( other ); }

  ~R11AsyncProcessThread();

  void Initialise( std::function< void( int_fast8_t ) > tickMethod );

  void Sync();

  void Resume();

private:
  void _Stop();

  void _ThreadTick();

private:
  std::function< void( int_fast8_t ) > _tickMethod = nullptr;

  bool _stop = false;
  bool _stopped = false;
  bool _gotResume = false;
  bool _gotSync = true;
  std::mutex _resumeMutex;
  std::condition_variable_any _resumeCondt;
  std::condition_variable_any _syncCondt;
  std::thread _thread = std::thread( &R11AsyncProcessThread::_ThreadTick, this );
};

//=============================================================================

R11AsyncProcessThread::~R11AsyncProcessThread()
{
  _Stop();
}

//=============================================================================

void R11AsyncProcessThread::Initialise( std::function< void( int_fast8_t ) > tickMethod )
{
  _tickMethod = tickMethod;
}

//-----------------------------------------------------------------------------

void R11AsyncProcessThread::Sync()
{
  _resumeMutex.lock();

  if( !_gotSync ) // if haven't already got sync
  {
    _syncCondt.wait( _resumeMutex ); // wait for sync
  }

  _resumeMutex.unlock();
}

//-----------------------------------------------------------------------------

void R11AsyncProcessThread::Resume()
{
  _resumeMutex.lock();

  _gotSync = false; // reset the sync flag

  _gotResume = true; // set the resume flag
  _resumeCondt.notify_all();

  _resumeMutex.unlock();
}

//=============================================================================

void R11AsyncProcessThread::_Stop()
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

  _stopped = true;
}

}

//=============================================================================

#endif // R11ASYNCPROCESSTHREAD_H
