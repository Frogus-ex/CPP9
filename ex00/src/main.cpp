#include "BitcoinExchange.hpp"

// void
// isValid (const char *filename)
// {
//   std::ifstream fp (filename);
//   if (!fp.is_open ())
//     {
//       std::cerr << "Error: could not open file.";
//       return;
//     }
//   std::string line;
//   while (std::getline (fp, line))
//     {
//       int flag = 0;
//       if (line.find ("|") == line.npos)
//         {
//           flag = 1;
//           std::cout << "Error: bad input => " << line << "\n";
//         }
//       size_t n = line.find ("|");
//       std::string test = line;
//       while (test[n] && flag == 0)
//         {
//           if (test[n] == '-')
//             {
//               flag = 1;
//               std::cout << "Error: not a positive number.\n";
//             }
//           n++;
//         }
//       if (flag == 0)
//         std::cout << line << "\n";
//     }
// }

void
formatString (const char *filename)
{
  std::ifstream fp (filename);
  if (!fp.is_open ())
    {
      std::cerr << "Error: could not open file.";
      return;
    }
  std::string line;
  std::string date;
  std::string value;
  bool dateIsValidFormat = false;
  while (std::getline (fp, line))
    {
      if (line.find ("|") != line.npos)
        {
          date = line.substr (0, line.find ("|"));
          date = date.substr (date.find_first_not_of (" "), date.npos);
          if (date.length () == 10 && date[4] == '-' && date[7] == '-')
            dateIsValidFormat = true;
          else
            dateIsValidFormat = false;
        }
      if (line.find ("|") != line.npos)
        {
          value = line.substr (line.find ("|") + 2, line.npos);
          value = value.substr (value.find_first_not_of (" "), value.npos);
        }
      std::cout << date << "\n";
    }
}

int
main (int ac, char **av)
{
  if (ac != 2)
    std::cerr << "Error: wrong nbr of arguments\n";
  //   isValid (av[1]);
  formatString (av[1]);
}
