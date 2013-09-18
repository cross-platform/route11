#ifndef R11DSPCOMPONENT_H
#define R11DSPCOMPONENT_H

//-----------------------------------------------------------------------------

#include "DSPatch.h"
#include "R11ComponentThread.h"

#include <cstdint>
#include <functional>

//=============================================================================

namespace Route11
{

template< typename CT >
class R11DspComponent final : public DspComponent
{
private:
  CT _component;

public:
  explicit R11DspComponent( int_fast8_t threadCount = 0, bool startRunning = false );
  explicit R11DspComponent( bool startRunning );
  R11DspComponent( ThreadConfig threadConfig, bool startRunning = false );

  void SetThreadCount( int_fast8_t threadCount );

  template< uint_fast16_t input, typename T >
  void SetInput( const T& value );

  template< uint_fast16_t input >
  auto GetInput() -> decltype( _component.template GetInput< input >() );

  template< uint_fast16_t output >
  auto GetOutput() -> decltype( _component.template GetOutput< output >() );

protected:
  virtual void Process_( DspSignalBus& inputs, DspSignalBus& outputs ) override;

private:
  // Fancy little trick to get around function template specialization within a class
  template< uint_fast16_t First, uint_fast16_t Size >
  struct _StaticLoop
  {
    void AddIos( std::function< void() > addMethod );
    void FillInputs( CT& component, DspSignalBus& inputs );
    void FillOutputs( CT& component, DspSignalBus& outputs );
  };

  template< uint_fast16_t N >
  struct _StaticLoop< N, N >
  {
    void AddIos( std::function< void() > addMethod ) {}
    void FillInputs( CT& component, DspSignalBus& inputs ) {}
    void FillOutputs( CT& component, DspSignalBus& inputs ) {}
  };

private:
  _StaticLoop< 0, CT::inputCount > _inputsLooper;
  _StaticLoop< 0, CT::outputCount > _outputsLooper;
};

//=============================================================================

template< typename CT >
R11DspComponent< CT >::R11DspComponent( int_fast8_t threadCount, bool startRunning )
{
  _inputsLooper.AddIos( [ this ]() { AddInput_(); } );
  _outputsLooper.AddIos( [ this ]() { AddOutput_(); } );

  SetThreadCount( threadCount );

  if( startRunning )
  {
    StartAutoTick();
  }
}

//-----------------------------------------------------------------------------

template< typename CT >
R11DspComponent< CT >::R11DspComponent( bool startRunning )
: R11DspComponent( 0, startRunning ){}

//-----------------------------------------------------------------------------

template< typename CT >
R11DspComponent< CT >::R11DspComponent( ThreadConfig threadConfig, bool startRunning )
: R11DspComponent( threadConfig == ThreadConfig::ThreadPerCore ?
                   std::thread::hardware_concurrency() : 0,
                   startRunning ) {}

//=============================================================================

template< typename CT >
void R11DspComponent< CT >::SetThreadCount( int_fast8_t threadCount )
{
  if( threadCount < 0 )
  {
    return;
  }

  PauseAutoTick();
  _component.SetThreadCount( threadCount );
  ResumeAutoTick();
}

//-----------------------------------------------------------------------------

template< typename CT >
template< uint_fast16_t input, typename T >
void R11DspComponent< CT >::SetInput( const T& value )
{
  PauseAutoTick();
  _component.template SetInput< input >( value );
  ResumeAutoTick();
}

//-----------------------------------------------------------------------------

template< typename CT >
template< uint_fast16_t input >
auto R11DspComponent< CT >::GetInput() -> decltype( _component.template GetInput< input >() )
{
  PauseAutoTick();
  auto returnValue = _component.template GetInput< input >();
  ResumeAutoTick();

  return returnValue;
}

//-----------------------------------------------------------------------------

template< typename CT >
template< uint_fast16_t output >
auto R11DspComponent< CT >::GetOutput() -> decltype( _component.template GetOutput< output >() )
{
  PauseAutoTick();
  auto returnValue = _component.template GetOutput< output >();
  ResumeAutoTick();

  return returnValue;
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
template< uint_fast16_t First, uint_fast16_t Size >
void R11DspComponent< CT >::_StaticLoop< First, Size >::AddIos( std::function< void() > addMethod )
{
  addMethod();

  _StaticLoop< First + 1, Size >().AddIos( addMethod );
}

//-----------------------------------------------------------------------------

template< typename CT >
template< uint_fast16_t First, uint_fast16_t Size >
void R11DspComponent< CT >::_StaticLoop< First, Size >::FillInputs( CT& component, DspSignalBus& inputs )
{
  auto input = component.template GetInput< First >();

  inputs.GetValue( First, input );
  component.template SetInput< First >( input );

  _StaticLoop< First + 1, Size >().FillInputs( component, inputs );
}

//-----------------------------------------------------------------------------

template< typename CT >
template< uint_fast16_t First, uint_fast16_t Size >
void R11DspComponent< CT >::_StaticLoop< First, Size >::FillOutputs( CT& component, DspSignalBus& outputs )
{
  outputs.SetValue( First, component.template GetOutput< First >() );

  _StaticLoop< First + 1, Size >().FillOutputs( component, outputs );
}

}

//=============================================================================

#endif // R11DSPCOMPONENT_H
