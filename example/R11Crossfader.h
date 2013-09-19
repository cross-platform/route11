#ifndef R11CROSSFADER_H
#define R11CROSSFADER_H

//=============================================================================

class R11Crossfader
{
protected:
  ~R11Crossfader() = default;

  //-----------------------------------------------------------------------------

  void Process_()
  {
    auto bufferSize = std::get< 0 >( input_ ).size();
    auto crossfade = std::get< 2 >( input_ );

    if( bufferSize != std::get< 1 >( input_ ).size() ||
        crossfade < 0 || crossfade > 1 )
    {
      std::get< 0 >( output_ ) = {};
      return;
    }

    if( _bufferSize != bufferSize )
    {
      std::get< 0 >( output_ ).resize( bufferSize );
      _bufferSize = bufferSize;
    }

    for( uint_fast32_t i = 0; i < bufferSize; ++i )
    {
      std::get< 0 >( output_ )[ i ] = ( std::get< 0 >( input_ )[ i ] * ( 1 - crossfade ) ) +
                                      ( std::get< 1 >( input_ )[ i ] * crossfade );
    }
  }

  //-----------------------------------------------------------------------------

protected:
  std::tuple< std::vector< float >, std::vector< float >, float > input_;
  std::tuple< std::vector< float > > output_;

private:
  uint_fast32_t _bufferSize = 0;
};

//=============================================================================

#endif // R11CROSSFADER_H
