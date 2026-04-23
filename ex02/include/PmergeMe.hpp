#pragma once

#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <sys/time.h>

class PmergeMe
{
private:
  std::vector<int> _main;
  std::vector<int> _pend;
  std::vector<int> _pendPartner;
  bool _hasStraggler;
  int _straggler;
  std::deque<int> _deque_main;
  std::deque<int> _deque_pend;
  std::deque<int> _deque_pendPartner;
  bool _hasDequeStraggler;
  int _dequeStraggler;

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
  void merge (int left, int mid, int right);
  void mergeSort (int left, int right);
  void insertionSort ();
  void sort ();
  void makePairsDeque (char **av);
  void mergeDeque (int left, int mid, int right);
  void mergeSortDeque (int left, int right);
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
