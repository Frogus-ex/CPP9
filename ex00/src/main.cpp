#include "BitcoinExchange.hpp"

int
main (int ac, char **av)
{
  if (ac < 3)
    {
      std::cerr << "Error: could not open file." << std::endl;
      return (1);
    }
  Bitcoin::addToCont (av[1]);
  Bitcoin::calculateBitcoinValue (av[2]);
  // Bitcoin::printContent ();
}
