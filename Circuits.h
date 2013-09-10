#ifndef CIRCUITS_H
#define CIRCUITS_H

//-----------------------------------------------------------------------------

#include "Components.h"
#include "Route11.h"
#include "C:\Projects\DSP\DSPatch\include\DSPatch.h"

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

template <unsigned short N> struct Box {};

template< typename CT >
class R11DspComponent : public DspComponent
{
private:
  CT _component;

  template< int index >
  void _FillInputs( DspSignalBus& inputs, Box<index> x = Box<index>() )
  {
    _FillInputs< index - 1 >( inputs );
  }

  void _FillInputs( DspSignalBus& inputs, Box<0> x = Box<0>() )
  {
    //index != 0 ? _FillInputs< index - 1 >( inputs ) : int i = 0;
  }

public:
  R11DspComponent()
  {
    AddInput_();
    AddOutput_();
  }

  void Process_( DspSignalBus& inputs, DspSignalBus& outputs ) override
  {
    _FillInputs< CT::inputCount >( inputs );
  };
};

//-----------------------------------------------------------------------------

#endif // CIRCUITS_H
