#pragma once

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

class Bitcoin
{
private:
  std::map<std::string, float> _database;
  void addToCont (const char *filename);
  void calculateBitcoinValue (const char *filename);
  bool validValue (const std::string &value);
  bool validDate (const std::string &date);
  int getDaysInMonth (int month, int year);
  bool isLeapYear (int year);
  void printContent ();

public:
  Bitcoin () {}
  ~Bitcoin () {}
  Bitcoin (const Bitcoin &copy) { *this = copy; }
  Bitcoin &
  operator= (const Bitcoin &other)
  {
    if (this != &other)
      {
        this->_database = other._database;
      }
    return (*this);
  }

  class BitcoinError : public std::exception
  {
  private:
    const char *_message;

  public:
    BitcoinError (const char *message) : _message (message) {}
    virtual const char *
    what () const throw ()
    {
      return (_message);
    }
    virtual ~BitcoinError () throw () {}
  };
  void run (const char *databaseFile, const char *inputFile);
};
