#include <map>
#include <utility>

// Used in the SurfaceConnector to store the surface residuals.
// Essentially a symmetric matrix where the indicies could be
// arbitrary data types.

template <class T1, class T2>
class SymmetricDualMap
{
public:
  SymmetricDualMap() {}
  virtual ~SymmetricDualMap() {}

  T2 & operator()(const T1 & item1, const T1 & item2);
  bool contains(const T1 & item1, const T1 & item2);

protected:
  using key_t = std::pair<T1, T1>;

  key_t getKey(const T1 & item1, const T1 & item2) const;
  std::map<key_t, T2> _map;
};

template <class T1, class T2>
inline typename SymmetricDualMap<T1, T2>::key_t
SymmetricDualMap<T1, T2>::getKey(const T1 & item1, const T1 & item2) const
{
  if (item1 < item2)
    return key_t(item1, item2);
  else
    return key_t(item2, item1);
}

template <class T1, class T2>
T2 &
SymmetricDualMap<T1, T2>::operator()(const T1 & item1, const T1 & item2)
{
  return _map[getKey(item1, item2)];
}

template <class T1, class T2>
bool
SymmetricDualMap<T1, T2>::contains(const T1 & item1, const T1 & item2)
{
  return _map.find(getKey(item1, item2)) != _map.end();
}