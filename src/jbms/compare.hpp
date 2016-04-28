#ifndef HEADER_GUARD_baea50dda679f7bfcadc55509ce5c804
#define HEADER_GUARD_baea50dda679f7bfcadc55509ce5c804

#include <functional>

namespace jbms {

template <class Pred, class KeyA, class KeyB>
struct CompareWithKey2 {
  Pred pred;
  KeyA keyA;
  KeyB keyB;

  CompareWithKey2(Pred pred, KeyA keyA, KeyB keyB)
    : pred(pred), keyA(keyA), keyB(keyB)
  {}

  template <class A, class B>
  auto operator()(A &&a, B &&b) -> decltype(pred(keyA(static_cast<A&&>(a)), keyB(static_cast<B&&>(b)))) {
    return pred(keyA(static_cast<A&&>(a)), keyB(static_cast<B&&>(b)));
  }
};

template <class Pred, class Key>
struct CompareWithKey {
  Pred pred;
  Key key;

  CompareWithKey(Pred pred, Key key)
    : pred(pred), key(key)
  {}

  template <class A, class B>
  auto operator()(A &&a, B &&b) -> decltype(pred(key(static_cast<A&&>(a)), key(static_cast<B&&>(b)))) {
    return pred(key(static_cast<A&&>(a)), key(static_cast<B&&>(b)));
  }
};

template <class Pred, class Key>
CompareWithKey<std::decay_t<Pred>, std::decay_t<Key>> compare_with_key(Pred &&pred, Key &&key) {
  return { std::forward<Pred>(pred), std::forward<Key>(key) };
}

template <class Pred, class KeyA, class KeyB>
CompareWithKey2<std::decay_t<Pred>, std::decay_t<KeyA>, std::decay_t<KeyB>> compare_with_key(Pred &&pred, KeyA &&keyA, KeyB &&keyB) {
  return { std::forward<Pred>(pred), std::forward<KeyA>(keyA), std::forward<KeyB>(keyB) };
}

template <class Pred = std::less<>>
auto three_way_compare(Pred &&pred = {}) {
  return [pred](auto const &a, auto const &b) -> int {
    if (pred(a,b))
      return -1;
    if (pred(b,a))
      return 1;
    return 0;
  };
}

}

#endif /* HEADER GUARD */
