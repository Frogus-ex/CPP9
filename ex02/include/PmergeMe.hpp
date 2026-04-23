#pragma once

#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <iomanip>
#include <ctime>
#include <sys/time.h>

class PmergeMe
{
private:
  std::vector<int> _main;
  std::vector<std::pair<int, int> > _pend;
  bool _hasLoneWolf;
  int _LoneWolf;
  std::deque<int> _deque_main;
  std::deque<std::pair<int, int> > _deque_pend;
  bool _hasDequeLoneWolf;
  int _dequeLoneWolf;

private:
  int binarySearch (int value, int rightExclusive);
  int binarySearchDeque (int value, int rightExclusive);
  std::vector<int> generateJacobsthal (int size);

public:
  PmergeMe ();
  PmergeMe (const PmergeMe &other);
  PmergeMe &operator= (const PmergeMe &other);
  ~PmergeMe ();
  void printContent ();
  void makePairs (char **av);
  void merge (size_t left, size_t mid, size_t right);
  void mergeSort (size_t left, size_t right);
  void insertionSort ();
  void sort ();
  void makePairsDeque (char **av);
  void mergeDeque (size_t left, size_t mid, size_t right);
  void mergeSortDeque (size_t left, size_t right);
  void insertionSortDeque ();
  void sortDeque ();
  class PmergeError : public std::exception
  {
  private:
    const char *_message;

  public:
    PmergeError (const char *message) : _message (message) {}
    virtual const char *
    what () const throw ()
    {
      return (_message);
    }
    virtual ~PmergeError () throw () {}
  };
};
