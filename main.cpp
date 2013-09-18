#include "Processes.h"
#include "Components.h"
#include "Route11.h"
#include "R11DspComponent.h"

#include <iostream>

// USED 25
// =======
// in-class initializers
// override
// final
// static_assert
// type traits
// auto
// decltype
// std::function
// lambdas
// tuples
// uniform init
// alias
// = default / delete
// constexpr
// variadic templates
// inheriting constr
// nullptr
// std::threads
// chrono
// range-for
// integer types
// safe pointers
// expicit conv
// nonmember begin / end
// move semantics

// NOT USED 9
// ===========
// delegating constr
// enum classes
// user-defined literals
// std::array
// STL algorithms
// raw string literals
// char types
// unrestricted unions
// inline namespace

//-----------------------------------------------------------------------------

using Route11::R11DspComponent;

int main()
{
  BoolsPrinter x;
  R11DspComponent< BoolsPrinter > y;

  x.SetThreadCount( 10 );
  y.SetThreadCount( 10 );

  DspCircuit circuit;
  circuit.AddComponent( y );
  circuit.SetThreadCount( 12 );
  circuit.StartAutoTick();

  x.Tick();
  x.SetThreadCount( 12 );
  x.Tick();
  x.Tick();
  x.Tick();

  getchar();
  y.StartAutoTick();
  getchar();
  y.SetThreadCount( 3 );
  getchar();
  y.SetThreadCount( 1 );
  y.SetThreadCount( 0 );
  y.SetThreadCount( 3 );
  circuit.SetThreadCount( 1 );
  y.SetThreadCount( 100 );
  y.SetThreadCount( 0 );
  getchar();
  y.SetThreadCount( 1 );
  y.SetThreadCount( 13 );
  y.SetThreadCount( 1 );
  y.SetThreadCount( 1 );
  getchar();
  y.SetThreadCount( 30 );
  getchar();

  std::cout << "DIE";
  DSPatch::Finalize();
  return 0;
}

//-----------------------------------------------------------------------------
