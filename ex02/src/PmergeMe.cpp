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
          int a = *it;
          int b = *(it + 1);
          if (a <= b)
            _paires.push_back (std::make_pair (a, b));
          else
            _paires.push_back (std::make_pair (b, a));
          it += 2;
        }
      else
        {
          _paires.push_back (std::make_pair (*it, -1));
          it++;
        }
    }
}

void  PmergeMe::sortMaxElements()
{
  std::vector<std::pair<int, int> >::iterator it = _paires.begin();
  int first;
  int second;
  int tmp;
  for (; it != _paires.end(); it += 2)
  {
    if (it + 1 != _paires.end())
    {
      it->second = first;
      it++;
      it->second = second;
      if (first > second)
      {
        it->second = first;
        it--;
        it->second = second;
      }
      else
        it += 2;
    }
  }
}

void
PmergeMe::printContent ()
{
  std::cout << "Main: ";
  std::vector<int>::iterator it = _main.begin ();
  while (it != _main.end ())
    {
      std::cout << *it << " ";
      ++it;
    }
  std::cout << std::endl;
  std::cout << "Paires:" << std::endl;
  std::vector<std::pair<int, int> >::iterator pit = _paires.begin();
  while (pit != _paires.end ())
    {
      std::cout << "  (" << pit->first << ", " << pit->second << ")" << std::endl;
      ++pit;
    }
}
