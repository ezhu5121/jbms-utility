#ifndef HEADER_GUARD_kz92f53acf49c0fd51cb67bf2ab1e958
#define HEADER_GUARD_kz92f53acf49c0fd51cb67bf2ab1e958

#include <stdexcept>

namespace jbms {

template <class F> class move_only_wrapper {
public:
  F target;
  template <class... T> move_only_wrapper(T &&... t) : target(std::forward<T>(t)...) {}

  move_only_wrapper(move_only_wrapper const &)
    : target(false ? (F && ) * static_cast<F *>(nullptr) : throw std::logic_error("Cannot copy move_only_wrapper")) {}

  move_only_wrapper(move_only_wrapper &)
    : target(false ? (F && ) * static_cast<F *>(nullptr) : throw std::logic_error("Cannot copy move_only_wrapper")) {}

  move_only_wrapper(move_only_wrapper &&x) = default;

  move_only_wrapper &operator=(move_only_wrapper const &) { throw std::logic_error("Cannot copy move_only_wrapper"); }

  move_only_wrapper &operator=(move_only_wrapper &&x) = default;

  template <class... T> auto operator()(T &&... x) const -> decltype(target(std::forward<T>(x)...)) {
    return target(std::forward<T>(x)...);
  }

  template <class... T> auto operator()(T &&... x) -> decltype(target(std::forward<T>(x)...)) {
    return target(std::forward<T>(x)...);
  }
};

template <class T> move_only_wrapper<typename std::decay<T>::type> move_only(T &&x) {
  return move_only_wrapper<typename std::decay<T>::type>(std::forward<T>(x));
}

template <class T, class... U> move_only_wrapper<T> move_only(U &&... x) { return move_only_wrapper<T>(std::forward<U>(x)...); }
}

#endif /* HEADER GUARD */
