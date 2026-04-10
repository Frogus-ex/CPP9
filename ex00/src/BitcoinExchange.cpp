#include "BitcoinExchange.hpp"

std::map<std::string, float> Bitcoin::_database;

bool
Bitcoin::isLeapYear (int year)
{
  return ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0));
}

int
Bitcoin::getDaysInMonth (int month, int year)
{
  if (month == 2)
    return (Bitcoin::isLeapYear (year) ? 29 : 28);
  if (month == 4 || month == 6 || month == 9 || month == 11)
    return (30);
  return (31);
}

bool
Bitcoin::validValue (std::string value)
{
  float valVal = static_cast<float> (std::atof (value.c_str ()));
  if (valVal < 0)
    {
      std::cerr << "Error: not a positive number." << std::endl;
      return (false);
    }
  if (valVal > 1000)
    {
      std::cerr << "Error: too large a number." << std::endl;
      return (false);
    }
  return (true);
}

bool
Bitcoin::validDate (std::string date)
{
  if (date.length () != 10 || date[4] != '-' || date[7] != '-')
    {
      std::cout << "Error: wrong date format.\n";
      return (false);
    }
  for (size_t n = 0; n < 10; n++)
    {
      if (n == 4 || n == 7)
        continue;
      if (!std::isdigit (static_cast<unsigned char> (date[n])))
        {
          std::cout << "Error: wrong date format.\n";
          return (false);
        }
    }
  int year = std::atoi (date.substr (0, 4).c_str ());
  int month = std::atoi (date.substr (5, 2).c_str ());
  int day = std::atoi (date.substr (8, 2).c_str ());
  if (year > 2026 || year < 0)
    {
      std::cout << "Error: this month doesn t exist\n";
      return (false);
    }
  if (month > 12 || month < 1)
    {
      std::cout << "Error: this month doesn t exist\n";
      return (false);
    }
  if (day > Bitcoin::getDaysInMonth (month, year) || day < 1)
    {
      std::cout << "Error: this day doesn t exist\n";
      return (false);
    }
  return (true);
}

void
Bitcoin::addToCont (const char *filename)
{
  std::ifstream file (filename);

  if (!file.is_open ())
    {
      std::cerr << "Error: could not open file." << std::endl;
      return;
    }

  std::string line;
  bool firstLine = true;

  while (std::getline (file, line))
    {
      if (firstLine)
        {
          firstLine = false;
          continue;
        }

      if (line.empty ())
        continue;

      std::stringstream ss (line);
      std::string date, priceStr;

      if (!std::getline (ss, date, ',') || !std::getline (ss, priceStr))
        {
          continue;
        }

      float price = static_cast<float> (std::atof (priceStr.c_str ()));
      _database[date] = price;
    }

  file.close ();
}

void
Bitcoin::calculateBitcoinValue (const char *filename)
{
  std::ifstream file (filename);

  if (!file.is_open ())
    {
      std::cerr << "Error: could not open file." << std::endl;
      return;
    }

  std::string line;
  bool firstLine = true;
  std::string date;
  std::string value;
  while (std::getline (file, line))
    {
      if (firstLine)
        {
          firstLine = false;
          continue;
        }
      if (line.find ("|") == line.npos)
        {
          std::cout << "Error: bad input => " << line << std::endl;
          continue;
        }

      date = line.substr (0, line.find ("|") - 1);
      value = line.substr (line.find ("|") + 1);

      size_t start = date.find_first_not_of (" \t");
      size_t end = date.find_last_not_of (" \t");
      if (start != std::string::npos)
        date = date.substr (start, end - start + 1);

      start = value.find_first_not_of (" \t");
      end = value.find_last_not_of (" \t");
      if (start != std::string::npos)
        value = value.substr (start, end - start + 1);
      if (Bitcoin::validDate (date) == true
          && Bitcoin::validValue (value) == true)
        {
          std::map<std::string, float>::iterator it
              = _database.lower_bound (date);
          // verification si la date exact est dispo dans la base de donnee
          if (it != _database.end () && it->first == date)
            {
              float btcQuantity
                  = static_cast<float> (std::atof (value.c_str ()));
              float result = it->second * btcQuantity;
              std::cout << date << " => " << value << " = " << result
                        << std::endl;
            }
          // verification pour trouver la date a plus proche arrondie a l
          // inferieur
          else if (it != _database.end () && it->first != date)
            {
              if (it != _database.begin ())
                {
                  --it;
                  float btcQuantity
                      = static_cast<float> (std::atof (value.c_str ()));
                  float result = it->second * btcQuantity;
                  std::cout << date << " => " << value << " = " << result
                            << std::endl;
                }
              else
                std::cout << "Error: date too old for the database\n";
            }
          else if (it == _database.end ())
            std::cout << "Error: date too recent for the database\n";
        }
    }
}

void
Bitcoin::printContent ()
{
  std::map<std::string, float>::iterator it = _database.begin ();
  while (it != _database.end ())
    {
      std::cout << it->first << " => " << it->second << std::endl;
      ++it;
    }
}
