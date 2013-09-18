#ifndef COMPONENTS_H
#define COMPONENTS_H

//-----------------------------------------------------------------------------

#include "Processes.h"
#include "Route11.h"

using Route11::R11Process;
using Route11::R11ProcessLoop;
using Route11::R11ProcessPair;
using Route11::R11AsyncProcess;

//-----------------------------------------------------------------------------

class _BoolsPrinter
{
private:
  using BoolInverterPair = R11ProcessPair<
  1, 1, R11Process< BoolInverter >,
  1, 1, R11Process< BoolInverter > >;

  using BoolInverterLoop = R11ProcessLoop<
  2, 2, BoolInverterPair,
  1, 1 >;

  using BoolInvertedGen = R11ProcessPair<
  0, 2, R11Process< BoolGenerator >,
  2, 2, BoolInverterLoop,
  0, 0 >;

  using BoolPrinterPair = R11ProcessPair<
  1, 0, R11Process< BoolPrinter >,
  1, 0, R11Process< BoolPrinter > >;

  using BoolsPrinter = R11ProcessPair<
  2, 4, BoolInvertedGen,
  2, 0, BoolPrinterPair,
  2, 0,
  3, 1 >;

public:
  using T = R11AsyncProcess< BoolsPrinter >;
};

//-----------------------------------------------------------------------------

class BoolsPrinter : public _BoolsPrinter::T
{
public:
  enum Inputs
  {
    inInv1,
    inInv2,
    inPrn1,
    inPrn2
  };

  enum Outputs
  {
    outGen1,
    outGen2,
    outInv1,
    outInv2
  };
};

//-----------------------------------------------------------------------------

#endif // COMPONENTS_H
