#include "PmergeMe.hpp"
#include <cerrno>
#include <cstdlib>
#include <limits>

PmergeMe::PmergeMe ()
    : _hasLoneWolf (false), _LoneWolf (0), _hasDequeLoneWolf (false),
      _dequeLoneWolf (0)
{
}

PmergeMe::PmergeMe (const PmergeMe &other) { *this = other; }

PmergeMe &
PmergeMe::operator= (const PmergeMe &other)
{
  if (this != &other)
    {
      _main = other._main;
      _pend = other._pend;
      _hasLoneWolf = other._hasLoneWolf;
      _LoneWolf = other._LoneWolf;
      _deque_main = other._deque_main;
      _deque_pend = other._deque_pend;
      _hasDequeLoneWolf = other._hasDequeLoneWolf;
      _dequeLoneWolf = other._dequeLoneWolf;
    }
  return (*this);
}

PmergeMe::~PmergeMe () {}

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
      || parsed > std::numeric_limits<int>::max ())
    return (false);

  value = static_cast<int> (parsed);
  return (true);
}

// premiere etape de l exercice qui cree des paires de 2 et les pretrie une
// premiere fois avec le min et le max pour pouvoir ensuite faire notre merge
// sort sur les max uniquement. je garde pour les min leurs partenaire max pour
// l insertion plus tard tkt (larmes abondantes)

void
PmergeMe::makePairs (char **av)
{
  _main.clear ();
  _pend.clear ();
  _hasLoneWolf = false;

  int i;
  for (i = 1; av[i] && av[i + 1]; i += 2)
    {
      int a;
      int b;
      if (!parsePositiveInt (av[i], a) || !parsePositiveInt (av[i + 1], b))
        throw PmergeError ("Error");
      if (a < b)
        {
          _pend.push_back (std::make_pair (a, b));
          _main.push_back (b);
        }
      else
        {
          _pend.push_back (std::make_pair (b, a));
          _main.push_back (a);
        }
    }
  if (av[i])
    {
      int unpaired;
      if (!parsePositiveInt (av[i], unpaired))
        throw PmergeError ("Error");
      _hasLoneWolf = true;
      _LoneWolf = unpaired;
    }
}

// premiere etape du merge insertion sort :
// 1- Ajout de mes elements de main dans mon left et right scinde par mid qui
// est le juste milieu des elements 2- reinsertion dans main en trian les
// elements "si l1 <= r1 alors _main = l1 sinon r1 et ++ sur l ou r" 3-
// parcours tout les elements restant soit dans l1 soit r1 pour venir les
// placer a la fin du main comme que dans l ou r les elements restant ils
// seront deja trier.

void
PmergeMe::merge (size_t left, size_t mid, size_t right)
{
  size_t n1 = mid - left + 1;
  size_t n2 = right - mid;

  std::vector<int> L (n1), R (n2);

  for (size_t i = 0; i < n1; i++)
    L[i] = _main[left + i];
  for (size_t j = 0; j < n2; j++)
    R[j] = _main[mid + 1 + j];

  size_t i = 0, j = 0;
  size_t k = left;

  while (i < n1 && j < n2)
    {
      if (L[i] <= R[j])
        {
          _main[k] = L[i];
          i++;
        }
      else
        {
          _main[k] = R[j];
          j++;
        }
      k++;
    }

  while (i < n1)
    {
      _main[k] = L[i];
      i++;
      k++;
    }

  while (j < n2)
    {
      _main[k] = R[j];
      j++;
      k++;
    }
}

// recursion sur le merge afin de trier bien scinder l entiereter des element
// en elements individuel reintroduit dans main en checkant left et right
// seulement les triant et passe a l element suivant

void
PmergeMe::mergeSort (size_t left, size_t right)
{
  if (left >= right)
    return;

  size_t mid = left + (right - left) / 2;
  mergeSort (left, mid);
  mergeSort (mid + 1, right);
  merge (left, mid, right);
}

// genere la suite du jacob donc on a nos constant premier index 0 et 1 puis
// ensuite on vient appliquer la formule et push l element puis on avance au
// next element pour le set selon la formule et on s arrete quand le dernier
// element de la suite est <= a la taille de pend

std::vector<int>
PmergeMe::generateJacobsthal (int size)
{
  std::vector<int> result;
  if (size <= 0)
    return (result);
  result.push_back (1);
  if (size == 1)
    return (result);

  int previous = 1;
  int current = 3;
  while (current <= size)
    {
      result.push_back (current);
      int next = current + (2 * previous);
      previous = current;
      current = next;
    }
  return (result);
}

// on utilise notre suite de jacob pour cree le reel ordre d insertion de notre
// pend on check d abord que notre pend ne soit pas vide ou ne contient pas qu
// un element puis on parcours notre suite de jacob jusqu a sa fin et on vient
// insere du plus grand au plus petit dans notre vector order qui definira le
// reel ordre d insertion qui suis minutieusement la logique de ford tant que
// current est superieur a previous alors current decremente pour ajouter tout
// les les element du plus grand au plus petit. puis si il reste des elements
// ou reboucle sur tout ceux ci pour les ajouter a l ordre toujours du plus
// grand ou plus petit.

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

// fonction qui porte bien son nom. elle cherche le partenaire de l element
// actuel si l element partner

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

// binary search regarde l element du milieu si c est plus grand que l element
// a insere alors il degage tout les element de droite qui seront forcement
// plus grand et regarde au milieu de left etc

int
PmergeMe::binarySearch (int value, int rightExclusive)
{
  int left = 0;
  int right = rightExclusive;

  while (left < right)
    {
      int mid = left + (right - left) / 2;
      if (_main[static_cast<size_t> (mid)] < value)
        left = mid + 1;
      else
        right = mid;
    }
  return (left);
}

// le fameux insertion sort qui vient insere les elements dans le main
// en suivant l ordre de jacob qu on a defini precedemment

void
PmergeMe::insertionSort ()
{
  if (_pend.empty () && !_hasLoneWolf)
    return;

  std::vector<int> jacobsthal
      = generateJacobsthal (static_cast<int> (_pend.size ()));

  std::vector<size_t> order = buildInsertionOrder (_pend.size (), jacobsthal);
  for (size_t i = 0; i < order.size (); ++i)
    {
      size_t idx = order[i];
      int value = _pend[idx].first;
      int partner = _pend[idx].second;
      int rightExclusive = findPartnerBoundVector (_main, partner);
      int pos = binarySearch (value, rightExclusive);
      _main.insert (_main.begin () + pos, value);
    }

  if (_hasLoneWolf)
    {
      int pos = binarySearch (_LoneWolf, static_cast<int> (_main.size ()));
      _main.insert (_main.begin () + pos, _LoneWolf);
      _hasLoneWolf = false;
    }
}

void
PmergeMe::sort ()
{
  if (_main.empty () && _pend.empty () && !_hasLoneWolf)
    return;

  if (!_main.empty ())
    mergeSort (0, static_cast<size_t> (_main.size ()) - 1);
  insertionSort ();
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
// a partir de la c est pareil ma avec utilisation de deque*
/*********************************************************/

void
PmergeMe::makePairsDeque (char **av)
{
  _deque_main.clear ();
  _deque_pend.clear ();
  _hasDequeLoneWolf = false;

  int i;
  for (i = 1; av[i] && av[i + 1]; i += 2)
    {
      int a;
      int b;
      if (!parsePositiveInt (av[i], a) || !parsePositiveInt (av[i + 1], b))
        throw PmergeError ("Error");
      if (a < b)
        {
          _deque_pend.push_back (std::make_pair (a, b));
          _deque_main.push_back (b);
        }
      else
        {
          _deque_pend.push_back (std::make_pair (b, a));
          _deque_main.push_back (a);
        }
    }
  if (av[i])
    {
      int unpaired;
      if (!parsePositiveInt (av[i], unpaired))
        throw PmergeError ("Error");
      _hasDequeLoneWolf = true;
      _dequeLoneWolf = unpaired;
    }
}

void
PmergeMe::mergeDeque (size_t left, size_t mid, size_t right)
{
  size_t n1 = mid - left + 1;
  size_t n2 = right - mid;

  std::deque<int> L (n1), R (n2);

  for (size_t i = 0; i < n1; i++)
    L[i] = _deque_main[left + i];
  for (size_t j = 0; j < n2; j++)
    R[j] = _deque_main[mid + 1 + j];

  size_t i = 0, j = 0;
  size_t k = left;

  while (i < n1 && j < n2)
    {
      if (L[i] <= R[j])
        {
          _deque_main[k] = L[i];
          i++;
        }
      else
        {
          _deque_main[k] = R[j];
          j++;
        }
      k++;
    }

  while (i < n1)
    {
      _deque_main[k] = L[i];
      i++;
      k++;
    }

  while (j < n2)
    {
      _deque_main[k] = R[j];
      j++;
      k++;
    }
}

void
PmergeMe::mergeSortDeque (size_t left, size_t right)
{
  if (left >= right)
    return;

  size_t mid = left + (right - left) / 2;
  mergeSortDeque (left, mid);
  mergeSortDeque (mid + 1, right);
  mergeDeque (left, mid, right);
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

int
PmergeMe::binarySearchDeque (int value, int rightExclusive)
{
  int left = 0;
  int right = rightExclusive;

  while (left < right)
    {
      int mid = left + (right - left) / 2;
      if (_deque_main[static_cast<size_t> (mid)] < value)
        left = mid + 1;
      else
        right = mid;
    }
  return (left);
}

void
PmergeMe::insertionSortDeque ()
{
  if (_deque_pend.empty () && !_hasDequeLoneWolf)
    return;

  std::vector<int> jacobsthal
      = generateJacobsthal (static_cast<int> (_deque_pend.size ()));

  std::vector<size_t> order
      = buildInsertionOrder (_deque_pend.size (), jacobsthal);
  for (size_t i = 0; i < order.size (); ++i)
    {
      size_t idx = order[i];
      int value = _deque_pend[idx].first;
      int partner = _deque_pend[idx].second;
      int rightExclusive = findPartnerBoundDeque (_deque_main, partner);
      int pos = binarySearchDeque (value, rightExclusive);
      _deque_main.insert (_deque_main.begin () + pos, value);
    }

  if (_hasDequeLoneWolf)
    {
      int pos = binarySearchDeque (_dequeLoneWolf,
                                   static_cast<int> (_deque_main.size ()));
      _deque_main.insert (_deque_main.begin () + pos, _dequeLoneWolf);
      _hasDequeLoneWolf = false;
    }
}

void
PmergeMe::sortDeque ()
{
  if (_deque_main.empty () && _deque_pend.empty () && !_hasDequeLoneWolf)
    return;

  if (!_deque_main.empty ())
    mergeSortDeque (0, static_cast<size_t> (_deque_main.size ()) - 1);
  insertionSortDeque ();
}
