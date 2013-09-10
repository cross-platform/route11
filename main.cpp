#include <iostream>
#include <tuple>

#include "Circuits.h"
#include "Components.h"
#include "Route11.h"

// nullptr, int24_t, delegating constr, override, final, std::function, lambdas,
// std::tie (tuples), enum classes, user-defined literals, range-for / begin / end,
// safe pointers, std::array, chrono, STL algorithms, std::threads

//-----------------------------------------------------------------------------

int main()
{
  BoolsPrinter x;

  R11DspComponent< BoolsPrinter > y;

  x.Tick();
  x.Reset();
  x.Tick();
  x.Reset();

  std::cout << x.GetOutput< BoolsPrinter::outGen2 >() << '\n';

  return 0;
}

//-----------------------------------------------------------------------------
