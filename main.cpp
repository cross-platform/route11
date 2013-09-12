#include <iostream>
#include <tuple>

#include "Circuits.h"
#include "Components.h"
#include "Route11.h"
#include "R11DspComponent.h"

// USED 16
// in-class initializers, override, final, static_assert, type traits, auto, decltype,
// std::function, lambdas, tuples, uniform init, alias, = default / delete, constexpr,
// variadic templates, inheriting constr

// NOT USED 12
// nullptr, int24_t, delegating constr, enum classes, user-defined literals,
// range-for / begin / end, safe pointers, std::array, expicit conv, STL algorithms,
// std::threads, chrono

// NOT GOING TO USE 4
// raw string literals, char types, unrestricted unions, inline namespace

//-----------------------------------------------------------------------------

using Route11::R11DspComponent;

int main()
{
  BoolsPrinter x;
  R11DspComponent< BoolsPrinter > y;

  x.Tick();
  x.Reset();
  x.Tick();
  x.Reset();

  std::cout << x.GetOutput< BoolsPrinter::outGen2 >() << '\n';

  getchar();
  y.StartAutoTick();
  getchar();

  DSPatch::Finalize();
  return 0;
}

//-----------------------------------------------------------------------------
