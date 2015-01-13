#ifndef HEADER_GUARD_9632e4b62c015672f8baba214ef80f72
#define HEADER_GUARD_9632e4b62c015672f8baba214ef80f72

#include "./enable_if.hpp"
#include <type_traits>
#include <memory>

namespace jbms {

template <class T>
class ptr_ref;

/**
 * A non-owning pointer implicitly constructible from a bare pointer or a shared_ptr.
 *
 * Care regarding pointer lifetime is necessary when using this.
 **/
template <class T>
class ptr_ref {
  T *ptr_;

public:
  ptr_ref() : ptr_(nullptr) {}
  ptr_ref(ptr_ref const &) = default;

  template <class U, JBMS_ENABLE_IF_C(std::is_same<U, T>::value || std::is_same<const U, T>::value)>
  ptr_ref(std::shared_ptr<U> const &x)
      : ptr_(x.get()) {}

  template <class U, JBMS_ENABLE_IF_C(std::is_same<U, T>::value || std::is_same<const U, T>::value)>
  ptr_ref(U *ptr)
      : ptr_(ptr) {}

  template <class U, JBMS_ENABLE_IF_C(std::is_same<U, T>::value || std::is_same<const U, T>::value)>
  ptr_ref(ptr_ref<U> const &x)
      : ptr_(x.get()) {}

  ptr_ref &operator=(ptr_ref const &) = default;

  template <class U, JBMS_ENABLE_IF_C(std::is_same<U, T>::value || std::is_same<const U, T>::value)>
  ptr_ref &operator=(U *ptr) {
    ptr_ = ptr;
    return *this;
  }

  template <class U, JBMS_ENABLE_IF_C(std::is_same<U, T>::value || std::is_same<const U, T>::value)>
  ptr_ref &operator=(std::shared_ptr<U> const &x) {
    ptr_ = x.get();
    return *this;
  }

  template <class U, JBMS_ENABLE_IF_C(std::is_same<U, T>::value || std::is_same<const U, T>::value)>
  ptr_ref &operator=(ptr_ref<U> const &x) {
    ptr_ = x.get();
    return *this;
  }

  T *get() const { return ptr_; }
  operator T *() const { return ptr_; }

  T &operator*() const { return *ptr_; }
  T *operator->() const { return ptr_; }
};
}

#endif /* HEADER GUARD */
