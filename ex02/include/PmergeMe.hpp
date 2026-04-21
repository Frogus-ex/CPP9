#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>
#include <deque>
#include <sstream>
#include <string>

class PmergeMe {
    private:
        std::vector<int> _main;
        std::vector<int> _main;
        std::deque<int> _sort;
        std::vector<std::pair<int, int> > _paires;

    public:
    void printContent();
    void makePairs(char **av);
    void addMaxtoMain();
    void addMintoPend();
    void mergeSort();
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
  void run (const char *databaseFile, const char *inputFile);
};
