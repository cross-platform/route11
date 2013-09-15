#include <iostream>
#include <tuple>

#include "Circuits.h"
#include "Components.h"
#include "Route11.h"
#include "R11DspComponent.h"

// USED 20
// in-class initializers, override, final, static_assert, type traits, auto, decltype,
// std::function, lambdas, tuples, uniform init, alias, = default / delete, constexpr,
// variadic templates, inheriting constr, nullptr, std::threads, chrono, range-for

// NOT USED 9
// int24_t, delegating constr, enum classes, user-defined literals,
// safe pointers, std::array, expicit conv, STL algorithms, nonmember begin/end

// NOT GOING TO USE 4
// raw string literals, char types, unrestricted unions, inline namespace

//-----------------------------------------------------------------------------

using Route11::R11DspComponent;

int main()
{
  BoolsPrinter x;
  R11DspComponent< BoolsPrinter > y;

  x.SetThreadCount( 2 );
  y.SetThreadCount( 2 );

  x.Tick();
  x.Tick();
  x.Tick();
  x.Tick();
  x.Tick();
  x.Tick();
  x.Tick();
  x.Tick();
  x.Tick();
  x.Tick();
  x.Tick();
  x.Tick();
  x.Tick();
  x.Tick();

  getchar();
  y.StartAutoTick();
  getchar();

  DSPatch::Finalize();
  return 0;
}

//-----------------------------------------------------------------------------
