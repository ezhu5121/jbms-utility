#ifndef HEADER_GUARD_1a2214d4343bac558978c3ed40e2981d
#define HEADER_GUARD_1a2214d4343bac558978c3ed40e2981d

#include <stdexcept>
#include <exception>
#include <utility>
#include "jbms/enable_if.hpp"

namespace jbms {

/**
 * \brief Exception thrown when an \ref expected object has neither a value nor an exception.
 **/
class expected_error : public std::logic_error {
public:
  explicit expected_error() : std::logic_error("expected::value() called without value or exception") {}
};

struct expect_t {};
constexpr static expect_t expect;

struct unexpect_t {};
constexpr static unexpect_t unexpect;

namespace expected_detail {

[[noreturn]] inline void throw_error(std::exception_ptr const &e) {
  if (e)
    std::rethrow_exception(e);
  throw expected_error();
}
}

template <class T>
class expected;

class unexpected {
  std::exception_ptr exn;

public:
  unexpected(std::exception_ptr exn) : exn(std::move(exn)) {}
  std::exception_ptr error() const & { return exn; }
  std::exception_ptr error() && { return std::move(exn); }
};

template <class T>
auto make_expected(T &&x) {
  using Result = expected<std::remove_const_t<std::remove_reference_t<T>>>;
  return Result{ expect, std::forward<T>(x) };
}


/**
 * \brief Represents a discriminated union of a value or an exception (stored as an std::exception_ptr).
 **/
template <class T>
class expected {
  union Data {
    Data() {}
    ~Data() {}
    T value;
    std::exception_ptr exn;
  } data;
  bool has_value_;

public:

  using value_type = T;
  using error_type = std::exception_ptr;

  expected() : has_value_(false) { new (&data.exn) std::exception_ptr(); }

  template <class U, JBMS_ENABLE_IF_C(std::is_constructible<T, U const &>::value &&
                                      !std::is_convertible<U const &, T>::value)>
  explicit expected(expected<U> const &other)
    : has_value_(other.valid())
  {
    if (has_value_)
      new (&data.value) value_type(*other);
    else
      new (&data.exn) error_type(other.unchecked_error());
  }

  template <class U, JBMS_ENABLE_IF_C(std::is_constructible<T, U &&>::value &&
                                      !std::is_convertible<U &&, T>::value)>
  explicit expected(expected<U> &&other)
    : has_value_(other.valid())
  {
    if (has_value_)
      new (&data.value) value_type(std::move(*other));
    else
      new (&data.exn) error_type(std::move(other.unchecked_error()));
  }

  template <class U, JBMS_ENABLE_IF_C(std::is_convertible<U const &, T>::value)>
  expected(expected<U> const &other)
    : has_value_(other.valid())
  {
    if (has_value_)
      new (&data.value) value_type(*other);
    else
      new (&data.exn) error_type(other.unchecked_error());
  }

  template <class U, JBMS_ENABLE_IF_C(std::is_convertible<U &&, T>::value)>
  expected(expected<U> &&other)
    : has_value_(other.valid())
  {
    if (has_value_)
      new (&data.value) value_type(std::move(*other));
    else
      new (&data.exn) error_type(std::move(other.unchecked_error()));
  }

  template <class... U, JBMS_ENABLE_IF(std::is_constructible<T, U &&...>)>
  expected(expect_t, U &&... u) {
    has_value_ = true;
    new (&data.value) T(std::forward<U>(u)...);
  }

  expected(unexpect_t, std::exception_ptr exn) {
    has_value_ = false;
    new (&data.exn) std::exception_ptr(std::move(exn));
  }

  expected(unexpected e)
    : expected(unexpect, std::move(e).error()) {}

  template <JBMS_DISABLE_IF(std::is_same<std::decay_t<T>, std::exception_ptr>)>
  expected(std::exception_ptr e)
    : expected(unexpect, std::move(e))
  {}

  template <JBMS_ENABLE_IF(std::is_constructible<T, T const &>)>
  expected(T const &x)
      : expected(expect, x) {}

  template <JBMS_ENABLE_IF(std::is_constructible<T, T &&>)>
  expected(T &&x)
      : expected(expect, std::move(x)) {}

  bool valid() const { return has_value_; }

  explicit operator bool() const { return has_value_; }

  /**
   * \pre bool(*this) == true
   **/
  T const &operator*() const { return data.value; }

  /**
   * \pre bool(*this) == true
   **/
  T const *operator->() const { return &data.value; }

  /**
   * \pre bool(*this) == true
   **/
  T &operator*() { return data.value; }

  /**
   * \pre bool(*this) == true
   **/
  T *operator->() { return &data.value; }

  T const &value() const {
    if (has_value_)
      return data.value;
    expected_detail::throw_error(data.exn);
  }

  T &value() {
    if (has_value_)
      return data.value;
    expected_detail::throw_error(data.exn);
  }

  error_type error() const {
    return has_value_ ? error_type() : data.exn;
  }

  error_type const &unchecked_error() const {
    return data.exn;
  }

  error_type &unchecked_error() {
    return data.exn;
  }

  template <class F>
  auto map(F &&f) const -> decltype(make_expected(f(std::declval<value_type const &>()))) {
    if (!has_value_)
      return { unexpect, std::move(data.exn) };
    try {
      return make_expected(f(data.value));
    } catch (...) {
      return { unexpect, std::current_exception() };
    }
  }

  template <class F>
  auto map(F &&f) && -> decltype(make_expected(f(std::declval<value_type &&>()))) {
    if (!has_value_)
      return { unexpect, std::move(data.exn) };
    try {
      return make_expected(f(std::move(data.value)));
    } catch (...) {
      return { unexpect, std::current_exception() };
    }
  }

  expected(expected const &other)
    : has_value_(other.has_value_) {

    if (has_value_)
      new (&data.value) value_type(other.data.value);
    else
      new (&data.exn) error_type(other.data.exn);
  }

  expected(expected &&other)
    : has_value_(other.has_value_) {

    if (has_value_)
      new (&data.value) value_type(std::move(other.data.value));
    else
      new (&data.exn) error_type(std::move(other.data.exn));
  }

  template <class... U>
  void emplace(U &&...u) {
    expected(expect, std::forward<U>(u)...).swap(*this);
  }

  void swap(expected &other) {
    using std::swap;
    // \pre: v.valid() && !e.valid()
    // \post: !v.valid() && e.valid()
    auto do_swap_value_error = [](expected &v, expected &e) {
      error_type t = std::move(e.data.exn);
      e.data.exn.~error_type();
      try {
        new (&e.data.value) value_type(std::move(v.data.value));
        e.has_value_ = true;
      }
      catch (...) {
        new (&e.data.exn) error_type(std::move(t));
        throw;
      }
      v.data.value.~value_type();
      new (&v.data.exn) error_type(std::move(t));
      v.has_value_ = false;
    };

    if (has_value_) {
      if (other.has_value_) {
        swap(data.value, other.data.value);
      } else {
        do_swap_value_error(*this, other);
      }
    } else if (other.has_value_) {
      do_swap_value_error(other, *this);
    } else {
      swap(data.exn, other.data.exn);
    }
  }

  expected &operator=(expected const &other) {
    expected(other).swap(*this);
    return *this;
  }

  expected &operator=(expected &&other) {
    other.swap(*this);
    return *this;
  }

  ~expected() {
    if (!has_value_)
      data.exn.~exception_ptr();
    else
      data.value.~T();
  }
};

template <>
class expected<void> {
  union Data {
    Data() {}
    ~Data() {}
    std::exception_ptr exn;
  } data;
  bool has_value_;

public:
  using value_type = void;
  using error_type = std::exception_ptr;

  expected() : has_value_(false) { new (&data.exn) std::exception_ptr(); }

  expected(expect_t) : has_value_(true) {}

  expected(unexpect_t, std::exception_ptr exn) : has_value_(false) { new (&data.exn) std::exception_ptr(std::move(exn)); }

  expected(std::exception_ptr e)
    : expected(unexpect, std::move(e))
  {}

  expected(unexpected e)
    : expected(unexpect, std::move(e).error()) {}

  bool valid() const { return has_value_; }

  explicit operator bool() const { return has_value_; }

  void value() const {
    if (!has_value_)
      expected_detail::throw_error(data.exn);
  }

  template <class F>
  auto map(F &&f) const & -> decltype(make_expected(f())) {
    if (!has_value_)
      return { unexpect, std::move(data.exn) };
    try {
      return make_expected(f());
    } catch (...) {
      return { unexpect, std::current_exception() };
    }
  }

  template <class F>
  auto map(F &&f) && -> decltype(make_expected(f())) {
    if (!has_value_)
      return { unexpect, std::move(data.exn) };
    try {
      return make_expected(f());
    } catch (...) {
      return { unexpect, std::current_exception() };
    }
  }

  expected(expected const &other)
    : has_value_(other.has_value_) {

    if (!has_value_)
      new (&data.exn) error_type(other.data.exn);
  }

  expected(expected &&other)
    : has_value_(other.has_value_) {

    if (!has_value_)
      new (&data.exn) error_type(std::move(other.data.exn));
  }

  void emplace() {
    if (!has_value_)
      data.exn.~error_type();
    has_value_ = true;
  }

  void swap(expected &other) {
    using std::swap;
    auto do_swap_value_error = [](expected &v, expected &e) {
      new (&v.data.exn) error_type(std::move(e.data.exn));
      v.has_value_ = false;
      e.data.exn.~error_type();
      e.has_value_ = true;
    };

    if (has_value_) {
      if (other.has_value_) {
        return;
      } else {
        do_swap_value_error(*this, other);
      }
    } else if (other.has_value_) {
      do_swap_value_error(other, *this);
    } else {
      swap(data.exn, other.data.exn);
    }
  }

  expected &operator=(expected const &other) {
    expected(other).swap(*this);
    return *this;
  }

  expected &operator=(expected &&other) {
    other.swap(*this);
    return *this;
  }

  error_type error() const {
    return has_value_ ? error_type() : data.exn;
  }


  error_type const &unchecked_error() const {
    return data.exn;
  }

  error_type &unchecked_error() {
    return data.exn;
  }

  ~expected() {
    if (!has_value_)
      data.exn.~exception_ptr();
  }
};

inline expected<void> make_expected() { return { expect }; }

} // namespace jbms

#endif /* HEADER GUARD */
