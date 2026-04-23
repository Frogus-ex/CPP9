#include "PmergeMe.hpp"

int
main (int ac, char **av)
{
  if (ac < 2)
    {
      std::cerr << "Usage: " << av[0] << " <positive integers>" << std::endl;
      return (1);
    }

  try
    {
      struct timeval start_vector, end_vector, start_deque, end_deque;
      
      // VECTOR SORT
      gettimeofday(&start_vector, 0);
      
      PmergeMe sorter;
      sorter.makePairs (av);
      sorter.sort ();
      
      gettimeofday(&end_vector, 0);
      
      // DEQUE SORT
      gettimeofday(&start_deque, 0);
      sorter.makePairsDeque(av);
      sorter.sortDeque();
      gettimeofday(&end_deque, 0);

      // Print results
      std::cout << "Before: ";
      for (int i = 1; i != ac; i++)
        std::cout << av[i] << " ";
      std::cout << "\n";
      std::cout << "After: ";
      sorter.printContent ();
      
      std::cout << "Time to process a range of " << (ac - 1)
                << " elements with std::vector : "
                << std::fixed << std::setprecision(5)
                << ((static_cast<double>(end_vector.tv_sec - start_vector.tv_sec) * 1000000.0) +
                    static_cast<double>(end_vector.tv_usec - start_vector.tv_usec))
                << " us\n";
      std::cout << "Time to process a range of " << (ac - 1)
                << " elements with std::deque : "
                << std::fixed << std::setprecision(5)
                << ((static_cast<double>(end_deque.tv_sec - start_deque.tv_sec) * 1000000.0) +
                    static_cast<double>(end_deque.tv_usec - start_deque.tv_usec))
                << " us\n";
    }
  catch (const std::exception &e)
    {
      std::cerr << "Error" << std::endl;
      return (1);
    }

  return (0);
}
