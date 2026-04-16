#include "RPN.hpp"

void
RPN::rpnCalculation (std::string av)
{
  int topElement;
  int backElement;
  long overflow;
  for (size_t i = 0; i < av.size () && av.size () > 1; i++)
    {
      if (av[i] >= '0' && av[i] <= '9')
        _rpnList.push_back (av[i] - '0');
      else if (av[i] == '+' && _rpnList.size () >= 2)
        {
          topElement = _rpnList.back ();
          _rpnList.pop_back ();
          backElement = _rpnList.back ();
          _rpnList.pop_back ();
          overflow = static_cast<long>(backElement) + static_cast<long>(topElement);
          if (overflow < -2147483648 || overflow > 2147483647)
            throw rpnError ("Error : overflow");
          _rpnList.push_back (backElement + topElement);
        }
      else if (av[i] == '-' && _rpnList.size () >= 2)
        {
          topElement = _rpnList.back ();
          _rpnList.pop_back ();
          backElement = _rpnList.back ();
          _rpnList.pop_back ();
          overflow = static_cast<long>(backElement) - static_cast<long>(topElement);
          if (overflow < -2147483648 || overflow > 2147483647)
            throw rpnError ("Error : overflow");
          _rpnList.push_back (backElement - topElement);
        }
      else if (av[i] == '*' && _rpnList.size () >= 2)
        {
          topElement = _rpnList.back ();
          _rpnList.pop_back ();
          backElement = _rpnList.back ();
          _rpnList.pop_back ();
          overflow = static_cast<long>(backElement) * static_cast<long>(topElement);
          if (overflow < -2147483648 || overflow > 2147483647)
            throw rpnError ("Error : overflow");
          _rpnList.push_back (backElement * topElement);
        }
      else if (av[i] == '/' && _rpnList.size () >= 2)
        {
          topElement = _rpnList.back ();
          _rpnList.pop_back ();
          if (topElement == 0)
            throw rpnError ("Error: it is impossible to divide by 0");
          backElement = _rpnList.back ();
          _rpnList.pop_back ();
          overflow = static_cast<long>(backElement) / static_cast<long>(topElement);
          if (overflow < -2147483648 || overflow > 2147483647)
            throw rpnError ("Error : overflow");
          _rpnList.push_back (backElement / topElement);
        }
      else if (av[i] == 32)
        continue;
      else
        throw rpnError ("Error : arg is not valid");
    }
  if (_rpnList.size () != 1)
    throw rpnError ("Error: arg doesn t exist");
  std::cout << _rpnList.back () << std::endl;
}
