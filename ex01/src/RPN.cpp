#include "RPN.hpp"

void
RPN::rpnCalculation (std::string av)
{
  int topElement;
  int backElement;
  for (size_t i = 0; i < av.size () && av.size() > 1; i++)
    {
      if (av[i] >= '0' && av[i] <= '9')
        _rpnStack.push_back(av[i] - '0');
      else if (av[i] == '+' && _rpnStack.size () >= 2)
        {
          topElement = _rpnStack.back ();
          _rpnStack.pop_back ();
          backElement = _rpnStack.back ();
          _rpnStack.pop_back ();
          _rpnStack.push_back (backElement + topElement);
        }
      else if (av[i] == '-' && _rpnStack.size () >= 2)
        {
          topElement = _rpnStack.back ();
          _rpnStack.pop_back ();
          backElement = _rpnStack.back ();
          _rpnStack.pop_back ();
          _rpnStack.push_back (backElement - topElement);
        }
      else if (av[i] == '*' && _rpnStack.size () >= 2)
        {
          topElement = _rpnStack.back ();
          _rpnStack.pop_back ();
          backElement = _rpnStack.back ();
          _rpnStack.pop_back ();
          _rpnStack.push_back (backElement * topElement);
        }
      else if (av[i] == '/' && _rpnStack.size () >= 2)
        {
          topElement = _rpnStack.back ();
          _rpnStack.pop_back ();
          if (topElement == 0)
            throw std::runtime_error ("Error");
          backElement = _rpnStack.back ();
          _rpnStack.pop_back ();
          _rpnStack.push_back (backElement / topElement);
        }
      else if (av[i] == 32)
        continue;
      else
        throw std::runtime_error ("Error");
    }
  if (_rpnStack.size () != 1)
    throw std::runtime_error ("Error");
  std::cout << _rpnStack.back () << std::endl;
}
