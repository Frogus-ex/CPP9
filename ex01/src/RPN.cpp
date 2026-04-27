#include "RPN.hpp"
#include <sstream>

void
RPN::rpnCalculation (std::string av)
{
  _rpnList.clear ();
  int topElement;
  int backElement;
  long result;
  std::istringstream iss (av);
  std::string token;

  while (iss >> token)
    {
      if (token.size () != 1)
        throw rpnError ("Error : arg is not valid");
      char c = token[0];
      if (c >= '0' && c <= '9')
        _rpnList.push_back (c - '0');
      else if (c == '+' && _rpnList.size () >= 2)
        {
          topElement = _rpnList.back ();
          _rpnList.pop_back ();
          backElement = _rpnList.back ();
          _rpnList.pop_back ();
          result = static_cast<long>(backElement) + static_cast<long>(topElement);
          if (result < -2147483648 || result > 2147483647)
            throw rpnError ("Error : overflow");
          _rpnList.push_back (static_cast<int> (result));
        }
      else if (c == '-' && _rpnList.size () >= 2)
        {
          topElement = _rpnList.back ();
          _rpnList.pop_back ();
          backElement = _rpnList.back ();
          _rpnList.pop_back ();
          result = static_cast<long>(backElement) - static_cast<long>(topElement);
          if (result < -2147483648 || result > 2147483647)
            throw rpnError ("Error : overflow");
          _rpnList.push_back (static_cast<int> (result));
        }
      else if (c == '*' && _rpnList.size () >= 2)
        {
          topElement = _rpnList.back ();
          _rpnList.pop_back ();
          backElement = _rpnList.back ();
          _rpnList.pop_back ();
          result = static_cast<long>(backElement) * static_cast<long>(topElement);
          if (result < -2147483648 || result > 2147483647)
            throw rpnError ("Error : overflow");
          _rpnList.push_back (static_cast<int> (result));
        }
      else if (c == '/' && _rpnList.size () >= 2)
        {
          topElement = _rpnList.back ();
          _rpnList.pop_back ();
          if (topElement == 0)
            throw rpnError ("Error: it is impossible to divide by 0");
          backElement = _rpnList.back ();
          _rpnList.pop_back ();
          result = static_cast<long>(backElement) / static_cast<long>(topElement);
          if (result < -2147483648 || result > 2147483647)
            throw rpnError ("Error : overflow");
          _rpnList.push_back (static_cast<int> (result));
        }
      else
        throw rpnError ("Error : arg is not valid");
    }
  if (_rpnList.size () != 1)
    throw rpnError ("Error: arg doesn t exist");
  std::cout << _rpnList.back () << std::endl;
}
