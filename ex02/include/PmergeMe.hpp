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
        std::deque<int> _pend;
        std::vector<std::pair<int, int> > _paires;
    public:
    int addToCont(const char *av);
    void printContent();
    void makePairs();

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
