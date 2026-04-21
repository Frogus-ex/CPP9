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
  coco.makePairs (av);
  coco.addMaxtoMain ();
  coco.printContent ();
  coco.recursion (1);
  coco.printContent ();
  return (0);
}
