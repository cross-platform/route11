#ifndef R11DSPCOMPONENT_H
#define R11DSPCOMPONENT_H

//-----------------------------------------------------------------------------

#include "DSPatch.h"

#include <functional>

//=============================================================================

namespace Route11
{

template< typename CT >
class R11DspComponent : public DspComponent
{
public:
  R11DspComponent();

  void SetThreadCount( char threadCount );

protected:
  virtual void Process_( DspSignalBus& inputs, DspSignalBus& outputs ) override;

private:
  // Fancy little trick to get around function template specialization within a class
  template< int First, int Size >
  struct _StaticLoop
  {
    void AddIos( std::function< void() > addMethod );
    void FillInputs( CT& component, DspSignalBus& inputs );
    void FillOutputs( CT& component, DspSignalBus& outputs );
  };

  template< int N >
  struct _StaticLoop< N, N >
  {
    void AddIos( std::function< void() > addMethod ) {}
    void FillInputs( CT& component, DspSignalBus& inputs ) {}
    void FillOutputs( CT& component, DspSignalBus& inputs ) {}
  };

private:
  CT _component;

  _StaticLoop< 0, CT::inputCount > _inputsLooper;
  _StaticLoop< 0, CT::outputCount > _outputsLooper;
};

//=============================================================================

template< typename CT >
R11DspComponent< CT >::R11DspComponent()
{
  _inputsLooper.AddIos( [ this ]() { AddInput_(); } );
  _outputsLooper.AddIos( [ this ]() { AddOutput_(); } );
}

//=============================================================================

template< typename CT >
void R11DspComponent< CT >::SetThreadCount( char threadCount )
{
  PauseAutoTick();
  _component.SetThreadCount( threadCount );
  ResumeAutoTick();
}

//=============================================================================

template< typename CT >
void R11DspComponent< CT >::Process_( DspSignalBus& inputs, DspSignalBus& outputs )
{
  _inputsLooper.FillInputs( _component, inputs );

  _component.Tick();

  _outputsLooper.FillOutputs( _component, outputs );
}

//=============================================================================

template< typename CT >
template< int First, int Size >
void R11DspComponent< CT >::_StaticLoop< First, Size >::AddIos( std::function< void() > addMethod )
{
  addMethod();

  _StaticLoop< First + 1, Size >().AddIos( addMethod );
}

//-----------------------------------------------------------------------------

template< typename CT >
template< int First, int Size >
void R11DspComponent< CT >::_StaticLoop< First, Size >::FillInputs( CT& component, DspSignalBus& inputs )
{
  auto input = component.template GetInput< First >();

  inputs.GetValue( First, input );
  component.template SetInput< First >( input );

  _StaticLoop< First + 1, Size >().FillInputs( component, inputs );
}

//-----------------------------------------------------------------------------

template< typename CT >
template< int First, int Size >
void R11DspComponent< CT >::_StaticLoop< First, Size >::FillOutputs( CT& component, DspSignalBus& outputs )
{
  outputs.SetValue( First, component.template GetOutput< First >() );

  _StaticLoop< First + 1, Size >().FillOutputs( component, outputs );
}

}

//=============================================================================

#endif // R11DSPCOMPONENT_H
