#ifndef R11COMPONENT_H
#define R11COMPONENT_H

//-----------------------------------------------------------------------------

template< typename Policy >
class R11Component : public Policy
{
  //static_assert( !std::is_destructible< Policy >::value, "Component policy should not be destructible" );

private:
  bool _ticked = false;

public:
  //using Policy::Policy;

  void Tick()
  {
    if( !_ticked )
    {
      _ticked = true;
      Policy::Process();
    }
  }

  void Reset()
  {
    _ticked = false;
  }

  template< int input, typename T >
  void SetInput( const T& value )
  {
    std::get< input >( Policy::input_ ) = value;
  }

  template< int input >
  auto GetInput() -> decltype( std::get< input >( Policy::input_ ) )
  {
    return std::get< input >( Policy::input_ );
  }

  template< int output >
  auto GetOutput() -> decltype( std::get< output >( Policy::output_ ) )
  {
    return std::get< output >( Policy::output_ );
  }

  static const unsigned int inputCount = std::tuple_size< decltype( R11Component::input_ ) >::value;
  static const unsigned int outputCount = std::tuple_size< decltype( R11Component::output_ ) >::value;
};

//-----------------------------------------------------------------------------

#endif // R11COMPONENT_H
