#include "RPN.hpp"

int
main (int ac, char **av)
{
  if (ac != 2)
    {
      std::cerr << "wrong nbr of args\n";
      return 1;
    }
  RPN calculator;
  try
    {
      calculator.rpnCalculation (av[1]);
    }
  catch (const std::exception &e)
    {
      std::cerr << e.what () << std::endl;
      return 1;
    }
  return 0;
}
