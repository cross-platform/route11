#ifndef R11COMPONENTBASE_H
#define R11COMPONENTBASE_H

//-----------------------------------------------------------------------------

#include <vector>
#include <thread>

#include "R11ComponentThread.h"

//-----------------------------------------------------------------------------

namespace Route11
{

class R11ComponentBase
{
protected:
  R11ComponentBase() = default;
  ~R11ComponentBase() = default;

  std::vector< R11ComponentThread > threads_;
  char threadCount_ = 0;
  char currentThread_ = -1;

  virtual std::function< void( char ) > GetTickMethod( char threadIndex ) = 0;

public:
  void SetThreadCount( char threadCount )
  {
    threads_.resize( threadCount );

    char i = 0;
    for( auto& thread : threads_ )
    {
      thread.Initialise( GetTickMethod( i ) );
      ++i;
    }

    threadCount_ = threadCount;
    currentThread_ = threadCount - 1;
  }

  void ThreadTick()
  {
    threads_[currentThread_].Tick();
    ++currentThread_ %= threadCount_;
  }
};

}

//-----------------------------------------------------------------------------

#endif // R11COMPONENTBASE_H
