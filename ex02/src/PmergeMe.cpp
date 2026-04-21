#include "PmergeMe.hpp"

bool
isValidArg (char *av)
{
  if (!av || !av[0])
    return (false);
  for (int i = 0; av[i]; i++)
    {
      if (!std::isdigit (av[i]))
        return (false);
    }
  return (true);
}

void
PmergeMe::makePairs (char **av)
{
  int i;
  for (i = 1; av[i + 1] && isValidArg (av[i]) && isValidArg (av[i + 1]);
       i += 2)
    {
      int a = std::atoi (av[i]);
      int b = std::atoi (av[i + 1]);
      if (a < b)
        _paires.push_back (std::make_pair (a, b));
      else
        _paires.push_back (std::make_pair (b, a));
    }
  if (i % 2 != 0 && av[i] && isValidArg (av[i]))
    {
      int unpaired = std::atoi (av[i]);
      _paires.push_back (std::make_pair (unpaired, -1));
    }
}

void
PmergeMe::addMaxtoMain ()
{
  std::vector<std::pair<int, int>>::iterator it = _paires.begin ();
  while (it->second)
    {
      _main.push_back (it->second);
      it++;
    }
}

void
PmergeMe::addMintoPend ()
{
  std::vector<std::pair<int, int>>::iterator it = _paires.begin ();
  while (it->second)
    {
      _pend.push_back (it->second);
      it++;
    }
}

void
PmergeMe::mergeSort ()
{
  if (_main.size () == 0)
    return;
  if (_main.size () <= 1)
    return;
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
  std::vector<std::pair<int, int>>::iterator pit = _paires.begin ();
  while (pit != _paires.end ())
    {
      std::cout << "  (" << pit->first << ", " << pit->second << ")"
                << std::endl;
      ++pit;
    }
}
