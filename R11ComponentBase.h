#ifndef R11COMPONENTBASE_H
#define R11COMPONENTBASE_H

//-----------------------------------------------------------------------------

#include <vector>

//-----------------------------------------------------------------------------

namespace Route11
{

class R11ComponentBase
{
private:
  std::vector< int > _threads;
  char _threadCount = 0;
  char _currentThread = -1;

protected:
  R11ComponentBase() = default;
  ~R11ComponentBase() = default;

public:
  void SetThreadCount( char threadCount )
  {
    _threads.resize( threadCount );
    _threadCount = threadCount;
  }
};

}

//-----------------------------------------------------------------------------

#endif // R11COMPONENTBASE_H
