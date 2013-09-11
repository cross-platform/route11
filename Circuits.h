#ifndef CIRCUITS_H
#define CIRCUITS_H

//-----------------------------------------------------------------------------

#include "Components.h"
#include "Route11.h"

using Route11::R11Component;
using Route11::R11ComponentLoop;
using Route11::R11ComponentPair;

//-----------------------------------------------------------------------------

class _BoolsPrinter
{
private:
  using BoolInverterPair = R11ComponentPair< 1, 1, R11Component< BoolInverter >, 1, 1, R11Component< BoolInverter > >;

  using BoolInvertedGen = R11ComponentPair< 0, 2, R11Component< BoolGenerator >, 2, 2, BoolInverterPair, 0, 0, 1, 1 >;

  using BoolPrinterPair = R11ComponentPair< 1, 0, R11Component< BoolPrinter >, 1, 0, R11Component< BoolPrinter > >;

public:
  using T = R11ComponentPair< 2, 4, BoolInvertedGen, 2, 0, BoolPrinterPair, 2, 0, 3, 1 >;
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

#endif // CIRCUITS_H
