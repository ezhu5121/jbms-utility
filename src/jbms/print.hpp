#ifndef HEADER_GUARD_d633187a0f19b3bb7e8d950eb860c300
#define HEADER_GUARD_d633187a0f19b3bb7e8d950eb860c300

#include "./print_fwd.hpp"
#include "./string_view.hpp"
#include <sstream>
#include <string>
#include <functional>
#include <tuple>
#include <array>
#include <utility>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

/**
 * \file
 *
 * Facilities for printing arbitrary types (for debugging purposes primarily)
 *
 **/

namespace jbms {

template <class... T>
std::string to_string(T const &... a) {
  std::ostringstream ostr;
  auto l = { ((void)print(ostr, a),nullptr)... };
  (void)l;
  return ostr.str();
}

template <class T>
struct printer_specialization<std::reference_wrapper<T>> {
  static void do_print(std::ostream &os, T const &x) {
    printer_specialization<std::decay_t<T>>::print(os, x.get());
  }
};

/**
 * Wrapper that prints a python-like repr string when streamed to an ostream.
 **/
template <class T>
struct Repr {
  T value;
  Repr(T value) : value(value) {}

  operator std::string () const {
    return to_string(*this);
  }

  std::string str() const {
    return to_string(*this);
  }
};

template <class T>
Repr<T const &> repr(T const &s) { return Repr<T const &>(s); }

/**
 * Note: We can't use Repr<string_view const &> because we need to make a temporary string_view
 **/
inline Repr<string_view> repr(std::string const &s) { return Repr<string_view>(s); }
inline Repr<string_view> repr(string_view const &s) { return Repr<string_view>(s); }
inline Repr<string_view> repr(const char *s) { return Repr<string_view>(s); }

template <class T>
std::ostream &operator<<(std::ostream &os, Repr<T> r) {
  print(os, r.value);
  return os;
}

inline std::ostream &operator<<(std::ostream &os, Repr<string_view> r) {
  os << '\"';

  for (auto x : r.value) {
    switch (x) {
    case '\0':
      os << "\\0";
      break;
    case '\'':
      os << "\\'";
      break;
    case '\\':
      os << "\\\\";
      break;
    case '\a':
      os << "\\a";
      break;
    case '\b':
      os << "\\b";
      break;
    case '\f':
      os << "\\f";
      break;
    case '\n':
      os << "\\n";
      break;
    case '\r':
      os << "\\r";
      break;
    case '\t':
      os << "\\t";
      break;
    case '\v':
      os << "\\v";
      break;
    case '"':
      os << "\\\"";
      break;
    default:
      if (std::isprint(x))
        os << x;
      else {
        os << "\\x";
        constexpr const char *letters = "0123456789abcdef";
        os << letters[x / 16];
        os << letters[x % 16];
      }
      break;
    }
  }

  os << '\"';
  return os;
}

template <class T>
std::string repr_string(T const &x) {
  return repr(x);
}

template <class T, class U>
struct printer_specialization<std::pair<T,U>> {
  static void do_print(std::ostream &os, std::pair<T,U> const &x) {
    os << "{ " << jbms::streamable(x.first) << " , " << jbms::streamable(x.second) << " }";
  }
};

template <class... T>
struct printer_specialization<std::tuple<T...>> {
  template <size_t... Is>
  static void helper(std::ostream &os, std::tuple<T...> const &x, std::index_sequence<Is...>) {
    os << "{ ";
    auto l = {
      ((os << (Is == 0 ? "" : ", ") << jbms::streamable(std::get<Is>(x))), 0)...
    };
    os << " }";
    (void)l;
  }
  static void do_print(std::ostream &os, std::tuple<T...> const &x) {
    helper(os, x, std::index_sequence_for<T...>{});
  }
};

template <class T, size_t N>
JBMS_RANGE_PRINTER_SPECIALIZATION(std::array<T,N>);

template <class T, class Alloc>
JBMS_RANGE_PRINTER_SPECIALIZATION(std::list<T,Alloc>);

template <class T, class Alloc>
JBMS_RANGE_PRINTER_SPECIALIZATION(std::vector<T,Alloc>);

template <class K, class V, class Compare, class Alloc>
JBMS_RANGE_PRINTER_SPECIALIZATION(std::map<K,V,Compare,Alloc>);

template <class K, class V, class Compare, class Alloc>
JBMS_RANGE_PRINTER_SPECIALIZATION(std::multimap<K,V,Compare,Alloc>);

template <class K, class Compare, class Alloc>
JBMS_RANGE_PRINTER_SPECIALIZATION(std::set<K,Compare,Alloc>);

template <class K, class Compare, class Alloc>
JBMS_RANGE_PRINTER_SPECIALIZATION(std::multiset<K,Compare,Alloc>);

template <class K, class V, class Hash, class Compare, class Alloc>
JBMS_RANGE_PRINTER_SPECIALIZATION(std::unordered_map<K,V,Hash,Compare,Alloc>);

template <class K, class V, class Hash, class Compare, class Alloc>
JBMS_RANGE_PRINTER_SPECIALIZATION(std::unordered_multimap<K,V,Hash,Compare,Alloc>);

template <class K, class Hash, class Compare, class Alloc>
JBMS_RANGE_PRINTER_SPECIALIZATION(std::unordered_set<K,Hash,Compare,Alloc>);

template <class K, class Hash, class Compare, class Alloc>
JBMS_RANGE_PRINTER_SPECIALIZATION(std::unordered_multiset<K,Hash,Compare,Alloc>);

}

#endif /* HEADER GUARD */
