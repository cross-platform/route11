#include <iostream>
#include <tuple>

#include "Circuits.h"
#include "Components.h"
#include "Route11.h"

// USED 16
// in-class initializers, override, final, static_assert, type traits, auto, decltype,
// std::function, lambdas, tuples, uniform init, alias, = default / delete, constexpr,
// variadic templates, inheriting constr

// NOT USED 16
// nullptr, int24_t, delegating constr, enum classes, user-defined literals,
// range-for / begin / end, safe pointers, std::array, chrono, STL algorithms,
// std::threads, raw string literals, expicit conversion, inline namespace,
// char types, unrestricted unions

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
