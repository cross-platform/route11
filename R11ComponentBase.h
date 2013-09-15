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

public:
  void ThreadTick()
  {
    threads_[currentThread_].Tick();
    ++currentThread_ %= threadCount_;
  }
};

}

//-----------------------------------------------------------------------------

#endif // R11COMPONENTBASE_H
