#ifndef R11DSPCOMPONENT_H
#define R11DSPCOMPONENT_H

//-----------------------------------------------------------------------------

#include <functional>
#include "C:\Projects\DSP\DSPatch\include\DSPatch.h"

//-----------------------------------------------------------------------------

namespace Route11
{
template< typename CT >
class R11DspComponent final : public DspComponent
{
private:
  CT _component;

  // this here is a fancy little trick to get around function template specialization within a class
  template< int First, int Size >
  struct _StaticLoop
  {
    void AddIos( std::function< void() > addMethod )
    {
      addMethod();

      _StaticLoop< First + 1, Size >().AddIos( addMethod );
    }

    void FillInputs( CT& component, DspSignalBus& inputs )
    {
      auto input = component.template GetInput< First >();

      inputs.GetValue( First, input );
      component.template SetInput< First >( input );

      _StaticLoop< First + 1, Size >().FillInputs( component, inputs );
    }

    void FillOutputs( CT& component, DspSignalBus& outputs )
    {
      outputs.SetValue( First, component.template GetOutput< First >() );

      _StaticLoop< First + 1, Size >().FillOutputs( component, outputs );
    }
  };

  template< int N >
  struct _StaticLoop< N, N >
  {
    void AddIos( std::function< void() > addMethod ) {}
    void FillInputs( CT& component, DspSignalBus& inputs ) {}
    void FillOutputs( CT& component, DspSignalBus& inputs ) {}
  };

private:
  _StaticLoop< 0, CT::inputCount > _inputsLooper;
  _StaticLoop< 0, CT::outputCount > _outputsLooper;

public:
  R11DspComponent()
  {
    _inputsLooper.AddIos( [ this ]() { AddInput_(); } );
    _outputsLooper.AddIos( [ this ]() { AddOutput_(); } );
  }

  void Process_( DspSignalBus& inputs, DspSignalBus& outputs ) override
  {
    _component.Reset();

    _inputsLooper.FillInputs( _component, inputs );

    _component.Tick();

    _outputsLooper.FillOutputs( _component, outputs );
  };
};
}

//-----------------------------------------------------------------------------

#endif // R11DSPCOMPONENT_H
