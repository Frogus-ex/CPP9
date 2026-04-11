#include "RPN.hpp"

void
RPN::rpnCalculation (std::string av)
{
  int topElement;
  int backElement;
  for (size_t i = 0; i < av.size () && av.size() > 1; i++)
    {
      if (av[i] >= '0' && av[i] <= '9')
        _rpnStack.push (av[i] - '0');
      else if (av[i] == '+' && _rpnStack.size () >= 2)
        {
          topElement = _rpnStack.top ();
          _rpnStack.pop ();
          backElement = _rpnStack.top ();
          _rpnStack.pop ();
          _rpnStack.push (backElement + topElement);
        }
      else if (av[i] == '-' && _rpnStack.size () >= 2)
        {
          topElement = _rpnStack.top ();
          _rpnStack.pop ();
          backElement = _rpnStack.top ();
          _rpnStack.pop ();
          _rpnStack.push (backElement - topElement);
        }
      else if (av[i] == '*' && _rpnStack.size () >= 2)
        {
          topElement = _rpnStack.top ();
          _rpnStack.pop ();
          backElement = _rpnStack.top ();
          _rpnStack.pop ();
          _rpnStack.push (backElement * topElement);
        }
      else if (av[i] == '/' && _rpnStack.size () >= 2)
        {
          topElement = _rpnStack.top ();
          _rpnStack.pop ();
          if (topElement == 0)
            throw std::runtime_error ("Error");
          backElement = _rpnStack.top ();
          _rpnStack.pop ();
          _rpnStack.push (backElement / topElement);
        }
      else if (av[i] == 32)
        continue;
      else
        throw std::runtime_error ("Error");
    }
  if (_rpnStack.size () != 1)
    throw std::runtime_error ("Error");
  std::cout << _rpnStack.top () << std::endl;
}
