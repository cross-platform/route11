#ifndef R11COMPONENTTHREAD_H
#define R11COMPONENTTHREAD_H

//-----------------------------------------------------------------------------

#include <functional>
#include <thread>
#include <condition_variable>

//-----------------------------------------------------------------------------
namespace Route11
{

class R11ComponentThread
{
private:
  std::function< void( char ) > _function = nullptr;

  bool _stop = false;
  bool _stopped = false;
  bool _gotResume = false;
  bool _gotSync = false;
  std::mutex _resumeMutex;
  std::condition_variable_any _resumeCondt;
  std::condition_variable_any _syncCondt;
  std::thread _thread = std::thread( &R11ComponentThread::_ThreadTick, this );

private:
  void _Sync()
  {
    _resumeMutex.lock();

    // wait for sync
    if( !_gotSync ) // if haven't already got sync
    {
      _syncCondt.wait( _resumeMutex ); // wait for sync
    }
    _gotSync = false; // reset the sync flag

    // notify resume
    _gotResume = true; // set the resume flag
    _resumeCondt.notify_all();

    _resumeMutex.unlock();
  }

  void _ThreadTick()
  {
    while( !_stop )
    {
      _resumeMutex.lock();;

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

      if( !_stop && _function != nullptr )
      {
        _function( 0 );
      }
    }

    _stopped = true;
  }

public:
  R11ComponentThread() = default;
  R11ComponentThread( const R11ComponentThread& other ) {}

  void Initialise( std::function< void( char ) > function )
  {
    _function = function;
  }

  ~R11ComponentThread()
  {
    _stop = true;

    while( !_stopped )
    {
      _resumeCondt.notify_all();
      std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
    }

    _thread.join();
  }

  void Tick()
  {
    _Sync();
  }
};

}

//-----------------------------------------------------------------------------

#endif // R11COMPONENTTHREAD_H
