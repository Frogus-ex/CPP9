#include "PmergeMe.hpp"
#include <cerrno>
#include <climits>
#include <cstdlib>

static bool
parsePositiveInt (const char *arg, int &value)
{
  if (!arg || !arg[0])
    return (false);

  for (int i = 0; arg[i]; ++i)
    {
      if (!std::isdigit (static_cast<unsigned char> (arg[i])))
        return (false);
    }

  errno = 0;
  char *endptr = 0;
  long parsed = std::strtol (arg, &endptr, 10);

  if (errno == ERANGE || *endptr != '\0' || parsed <= 0
      || parsed > static_cast<long> (INT_MAX))
    return (false);

  value = static_cast<int> (parsed);
  return (true);
}

static std::vector<size_t>
buildInsertionOrder (size_t pendCount, const std::vector<int> &jacobsthal)
{
  std::vector<size_t> order;
  if (pendCount == 0)
    return (order);

  order.push_back (0);
  if (pendCount == 1)
    return (order);

  size_t previous = 1;
  for (size_t k = 1; k < jacobsthal.size (); ++k)
    {
      size_t current = static_cast<size_t> (jacobsthal[k]);
      if (current > pendCount)
        current = pendCount;
      for (size_t idx = current; idx > previous; --idx)
        order.push_back (idx - 1);
      previous = current;
    }

  for (size_t idx = pendCount; idx > previous; --idx)
    order.push_back (idx - 1);
  return (order);
}

static int
findPartnerBoundVector (const std::vector<int> &chain, int partner)
{
  int i = 0;
  int size = static_cast<int> (chain.size ());
  while (i < size && chain[static_cast<size_t> (i)] < partner)
    ++i;
  if (i < size && chain[static_cast<size_t> (i)] == partner)
    return (i + 1);
  return (size);
}

static int
findPartnerBoundDeque (const std::deque<int> &chain, int partner)
{
  int i = 0;
  int size = static_cast<int> (chain.size ());
  while (i < size && chain[static_cast<size_t> (i)] < partner)
    ++i;
  if (i < size && chain[static_cast<size_t> (i)] == partner)
    return (i + 1);
  return (size);
}

//premiere etape de l exercice qui cree des paires de 2 et les pretrie une premiere fois avec le min et le max
//pour pouvoir ensuite faire notre merge sort sur les max uniquement.
//je garde pour les min leurs partenaire max pour l insertion plus tard tkt (larmes abondantes)

void
PmergeMe::makePairs (char **av)
{
  _main.clear ();
  _pend.clear ();
  _pendPartner.clear ();
  _hasStraggler = false;

  int i;
  for (i = 1; av[i] && av[i + 1]; i += 2)
    {
      int a;
      int b;
      if (!parsePositiveInt (av[i], a) || !parsePositiveInt (av[i + 1], b))
        throw PmergeError ("Error");
      if (a < b)
        {
          _pend.push_back (a);
          _pendPartner.push_back (b);
          _main.push_back (b);
        }
      else
        {
          _pend.push_back (b);
          _pendPartner.push_back (a);
          _main.push_back (a);
        }
    }
  if (av[i])
    {
      int unpaired;
      if (!parsePositiveInt (av[i], unpaired))
        throw PmergeError ("Error");
      _hasStraggler = true;
      _straggler = unpaired;
    }
}

//premiere etape du merge insertion sort :
//1- Ajout de mes elements de main dans mon left et right scinde par mid qui est le juste milieu des elements
//2- reinsertion dans main en trian les elements "si l1 <= r1 alors _main = l1 sinon r1 et ++ sur l ou r"
//3- parcours tout les elements restant soit dans l1 soit r1 pour venir les placer a la fin du main comme
//que dans l ou r les elements restant ils seront deja trier.

void
PmergeMe::merge (int left, int mid, int right)
{
  int n1 = mid - left + 1;
  int n2 = right - mid;

  std::vector<int> L (static_cast<size_t>(n1)), R (static_cast<size_t>(n2));

  for (int i = 0; i < n1; i++)
    L[static_cast<size_t>(i)] = _main[static_cast<size_t>(left + i)];
  for (int j = 0; j < n2; j++)
    R[static_cast<size_t>(j)] = _main[static_cast<size_t>(mid + 1 + j)];

  int i = 0, j = 0;
  int k = left;

  while (i < n1 && j < n2)
    {
      if (L[static_cast<size_t>(i)] <= R[static_cast<size_t>(j)])
        {
          _main[static_cast<size_t>(k)] = L[static_cast<size_t>(i)];
          i++;
        }
      else
        {
          _main[static_cast<size_t>(k)] = R[static_cast<size_t>(j)];
          j++;
        }
      k++;
    }

  while (i < n1)
    {
      _main[static_cast<size_t>(k)] = L[static_cast<size_t>(i)];
      i++;
      k++;
    }

  while (j < n2)
    {
      _main[static_cast<size_t>(k)] = R[static_cast<size_t>(j)];
      j++;
      k++;
    }
}

//recursion sur le merge afin de trier bien scinder l entiereter des element en
//elements individuel reintroduit dans main en checkant left et right seulement les triant 
//et passe a l element suivant

void
PmergeMe::mergeSort (int left, int right)
{
  if (left >= right)
    return;

  int mid = left + (right - left) / 2;
  mergeSort (left, mid);
  mergeSort (mid + 1, right);
  merge (left, mid, right);
}

void 
PmergeMe::generateJacobsthalVector(int size)
{
  int previous = 0, current = 1, next = 0;
  std::vector<int> result, insertion;

  while (current <= size) {
    next = current + (2 * (previous));
    if (next <= size)
      result.push_back(next);
    previous = current;
    current = next;
  }
}

//binary search regarde l element du milieu si c est plus grand que l element a insere
//alors il degage tout les element de droite qui seront forcement plus grand et regarde au milieu de left
//etc

int
PmergeMe::binarySearch (int value)
{
  return (binarySearch (value, static_cast<int> (_main.size ())));
}

int
PmergeMe::binarySearch (int value, int rightExclusive)
{
  int left = 0;
  int right = rightExclusive;

  while (left < right)
    {
      int mid = left + (right - left) / 2;
      if (_main[static_cast<size_t>(mid)] < value)
        left = mid + 1;
      else
        right = mid;
    }
  return (left);
}

std::vector<int>
PmergeMe::generateJacobsthal (int size)
{
  std::vector<int> result;
  if (size <= 0)
    return (result);
  
  int j0 = 0, j1 = 1;
  result.push_back (j1);
  
  while (j1 < size)
    {
      int j_next = j1 + 2 * j0;
      if (j_next < size)
        result.push_back (j_next);
      j0 = j1;
      j1 = j_next;
    }
  return (result);
}

//insertion sort optimise avec Jacobsthal
//Au lieu d'inserer lineairement, on suit la sequence de Jacobsthal
//puis on insere les elements "gaps" restants en ordre normal

void
PmergeMe::insertionSort ()
{
  if (_pend.empty () && !_hasStraggler)
    return;

  std::vector<int> jacobsthal = generateJacobsthal (static_cast<int>(_pend.size ()));

  std::vector<size_t> order = buildInsertionOrder (_pend.size (), jacobsthal);
  for (size_t i = 0; i < order.size (); ++i)
    {
      size_t idx = order[i];
      int value = _pend[idx];
      int partner = _pendPartner[idx];
      int rightExclusive = findPartnerBoundVector (_main, partner);
      int pos = binarySearch (value, rightExclusive);
      _main.insert (_main.begin () + pos, value);
    }

  if (_hasStraggler)
    {
      int pos = binarySearch (_straggler);
      _main.insert (_main.begin () + pos, _straggler);
      _hasStraggler = false;
    }
}

//fonction pour appeler le fameux merge insertion sort du petit ford

void
PmergeMe::sort ()
{
  if (_main.empty () && _pend.empty () && !_hasStraggler)
    return;

  if (!_main.empty ())
    mergeSort (0, static_cast<int> (_main.size ()) - 1);
  insertionSort ();
}

//fonction qui permet d obtenir precisement le temps ecouler

double getElapsedTime(clock_t start, clock_t end) {
    return static_cast<double>(end - start) / CLOCKS_PER_SEC;
}

void
PmergeMe::printContent ()
{
  std::vector<int>::iterator it = _main.begin ();
  while (it != _main.end ())
    {
      std::cout << *it << " ";
      ++it;
    }
  std::cout << std::endl;
}
/*********************************************************/
//a partir de la c est pareil ma avec utilisation de deque*
/*********************************************************/
int
PmergeMe::binarySearchDeque (int value)
{
  return (binarySearchDeque (value, static_cast<int> (_deque_main.size ())));
}

int
PmergeMe::binarySearchDeque (int value, int rightExclusive)
{
  int left = 0;
  int right = rightExclusive;

  while (left < right)
    {
      int mid = left + (right - left) / 2;
      if (_deque_main[static_cast<size_t>(mid)] < value)
        left = mid + 1;
      else
        right = mid;
    }
  return (left);
}

void
PmergeMe::mergeDeque (int left, int mid, int right)
{
  int n1 = mid - left + 1;
  int n2 = right - mid;

  std::deque<int> L (static_cast<size_t>(n1)), R (static_cast<size_t>(n2));

  for (int i = 0; i < n1; i++)
    L[static_cast<size_t>(i)] = _deque_main[static_cast<size_t>(left + i)];
  for (int j = 0; j < n2; j++)
    R[static_cast<size_t>(j)] = _deque_main[static_cast<size_t>(mid + 1 + j)];

  int i = 0, j = 0;
  int k = left;

  while (i < n1 && j < n2)
    {
      if (L[static_cast<size_t>(i)] <= R[static_cast<size_t>(j)])
        {
          _deque_main[static_cast<size_t>(k)] = L[static_cast<size_t>(i)];
          i++;
        }
      else
        {
          _deque_main[static_cast<size_t>(k)] = R[static_cast<size_t>(j)];
          j++;
        }
      k++;
    }

  while (i < n1)
    {
      _deque_main[static_cast<size_t>(k)] = L[static_cast<size_t>(i)];
      i++;
      k++;
    }

  while (j < n2)
    {
      _deque_main[static_cast<size_t>(k)] = R[static_cast<size_t>(j)];
      j++;
      k++;
    }
}

void
PmergeMe::mergeSortDeque (int left, int right)
{
  if (left >= right)
    return;

  int mid = left + (right - left) / 2;
  mergeSortDeque (left, mid);
  mergeSortDeque (mid + 1, right);
  mergeDeque (left, mid, right);
}

void
PmergeMe::insertionSortDeque ()
{
  if (_deque_pend.empty () && !_hasDequeStraggler)
    return;

  std::vector<int> jacobsthal = generateJacobsthal (static_cast<int>(_deque_pend.size ()));

  std::vector<size_t> order = buildInsertionOrder (_deque_pend.size (), jacobsthal);
  for (size_t i = 0; i < order.size (); ++i)
    {
      size_t idx = order[i];
      int value = _deque_pend[idx];
      int partner = _deque_pendPartner[idx];
      int rightExclusive = findPartnerBoundDeque (_deque_main, partner);
      int pos = binarySearchDeque (value, rightExclusive);
      _deque_main.insert (_deque_main.begin () + pos, value);
    }

  if (_hasDequeStraggler)
    {
      int pos = binarySearchDeque (_dequeStraggler);
      _deque_main.insert (_deque_main.begin () + pos, _dequeStraggler);
      _hasDequeStraggler = false;
    }
}

void
PmergeMe::makePairsDeque (char **av)
{
  _deque_main.clear ();
  _deque_pend.clear ();
  _deque_pendPartner.clear ();
  _hasDequeStraggler = false;

  int i;
  for (i = 1; av[i] && av[i + 1]; i += 2)
    {
      int a;
      int b;
      if (!parsePositiveInt (av[i], a) || !parsePositiveInt (av[i + 1], b))
        throw PmergeError ("Error");
      if (a < b)
        {
          _deque_pend.push_back (a);
          _deque_pendPartner.push_back (b);
          _deque_main.push_back (b);
        }
      else
        {
          _deque_pend.push_back (b);
          _deque_pendPartner.push_back (a);
          _deque_main.push_back (a);
        }
    }
  if (av[i])
    {
      int unpaired;
      if (!parsePositiveInt (av[i], unpaired))
        throw PmergeError ("Error");
      _hasDequeStraggler = true;
      _dequeStraggler = unpaired;
    }
}

void
PmergeMe::sortDeque ()
{
  if (_deque_main.empty () && _deque_pend.empty () && !_hasDequeStraggler)
    return;

  if (!_deque_main.empty ())
    mergeSortDeque (0, static_cast<int> (_deque_main.size ()) - 1);
  insertionSortDeque ();
}
