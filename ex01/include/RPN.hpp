#pragma once

#include <iostream>
#include <stack>
#include <string>

class RPN
{

private:
  std::stack<int> _rpnStack;

public:
  RPN () {}
  ~RPN () {}
  RPN (const RPN &copy) { *this = copy; }
  RPN &
  operator= (const RPN &other)
  {
    if (this != &other)
      this->_rpnStack = other._rpnStack;
    return (*this);
  }
  void rpnCalculation (std::string av);
  class rpnError : public std::exception
  {
  private:
    const char *_message;

  public:
    rpnError (const char *message) : _message (message) {}
    virtual const char *
    what () const throw ()
    {
      return (_message);
    }
    virtual ~rpnError () throw () {}
  };
};
