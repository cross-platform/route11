#ifndef COMPONENTS_H
#define COMPONENTS_H

//-----------------------------------------------------------------------------

class BoolGenerator
{
protected:
  ~BoolGenerator() = default;

  void Process()
  {
    std::get<0>( output_ ) = true;
    std::get<1>( output_ ) = false;
  }

  std::tuple<> input_;
  std::tuple< bool, bool > output_ { false, false };
};

//-----------------------------------------------------------------------------

class BoolInverter
{
protected:
  ~BoolInverter() = default;

  void Process()
  {
    std::get<0>( output_ ) = !std::get<0>( input_ );
  }

  std::tuple< bool > output_ { false };
  std::tuple< bool > input_ { false };
};

//-----------------------------------------------------------------------------

class BoolPrinter
{
protected:
  ~BoolPrinter() = default;

  void Process()
  {
    std::cout << std::get<0>( input_ ) << '\n';
  }

  std::tuple< bool > input_ { false };
  std::tuple<> output_;
};

//-----------------------------------------------------------------------------

#endif // COMPONENTS_H
