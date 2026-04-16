#include "PmergeMe.hpp"

int
main (int ac, char **av)
{
  (void)av;
  if (ac < 2)
    {
      std::cout << "wrong nbr of args\n";
      return (1);
    }
  PmergeMe coco;
  for (int i = 1; i < ac; i++)
    if (coco.addToCont (av[i]) == 1)
      return 1;
  coco.makePairs();
  coco.printContent ();
  return (0);
}
