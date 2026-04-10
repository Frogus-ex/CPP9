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
  static std::map<std::string, float> _database;

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
  static std::map<std::string, float> &
  getDates ()
  {
    return (_database);
  }
  static void addToCont (const char *filename);
  static void calculateBitcoinValue (const char *filename);
  static bool validValue (std::string value);
  static bool validDate (std::string date);
  static int getDaysInMonth(int month, int year);
  static bool isLeapYear (int year);
  static void printContent ();
};
