#include "PmergeMe.hpp"

int
PmergeMe::addToCont (const char *av)
{
  int i = 0;
  while (av[i])
    {
      if (!std::isdigit (av[i]))
        {
          std::cout << "Error: wrong arg not a number\n";
          return (1);
        }
      i++;
    }
  _main.push_back (std::atoi (av));
  return (0);
}

void
PmergeMe::makePairs ()
{
  std::vector<int>::iterator it = _main.begin ();

  while (it != _main.end ())
    {
      if (it + 1 != _main.end ())
      {
        if (it + 1 < it)
          _paires.push_back (std::make_pair (*(it + 1), *it));
        else
          _paires.push_back (std::make_pair (*it, *(it + 1)));
        }
      else
          _paires.push_back (std::make_pair (*it, -1));
      it += 2;
    }
}

void
PmergeMe::printContent ()
{
  std::vector<int>::iterator it = _main.begin ();
  while (it != _main.end ())
    {
      std::cout << *it << std::endl;
      ++it;
    }
}
