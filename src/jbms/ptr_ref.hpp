#ifndef HEADER_GUARD_9632e4b62c015672f8baba214ef80f72
#define HEADER_GUARD_9632e4b62c015672f8baba214ef80f72

#include "./enable_if.hpp"
#include <type_traits>
#include <memory>

namespace jbms {

template <class T>
class ptr_ref;

template <class Ptr>
struct pointer_element_type {};

template <class U>
struct pointer_element_type<U *> {
  using type = U;
};

template <class U>
struct pointer_element_type<ptr_ref<U>> {
  using type = U;
};

template <class U>
struct pointer_element_type<std::shared_ptr<U>> {
  using type = U;
};

template <class U, class Deleter>
struct pointer_element_type<std::unique_ptr<U,Deleter>> {
  using type = U;
};

/**
 * A non-owning pointer implicitly constructible from a bare pointer or a shared_ptr.
 *
 * Care regarding pointer lifetime is necessary when using this.
 **/
template <class T>
class ptr_ref {
  T *ptr_;

  // template <class Ptr, class U = typename pointer_element_type<Ptr>::type>
  // struct is_compatible {
  //   constexpr static bool value = std::is_same<U, T>::value || std::is_same<U const, T>::value;
  // };

  template <class Ptr, class U = typename pointer_element_type<Ptr>::type>
  using is_compatible = std::is_convertible<U *, T *>;

public:
  using element_type = T;

  ptr_ref() : ptr_(nullptr) {}
  ptr_ref(ptr_ref const &) = default;

  template <class Ptr, JBMS_ENABLE_IF(is_compatible<Ptr>)>
  ptr_ref(Ptr const &x)
      : ptr_(std::addressof(*x)) {}

  ptr_ref &operator=(ptr_ref const &) = default;

  template <class Ptr, JBMS_ENABLE_IF(is_compatible<Ptr>)>
  ptr_ref &operator=(Ptr const &x) {
    ptr_ = std::addressof(*x);
    return *this;
  }

  T *get() const { return ptr_; }
  operator T *() const { return ptr_; }

  T &operator*() const { return *ptr_; }
  T *operator->() const { return ptr_; }
};

}

#endif /* HEADER GUARD */
