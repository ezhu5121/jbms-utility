#ifndef HEADER_GUARD_b4575f1018fb99529178559f68d6040d
#define HEADER_GUARD_b4575f1018fb99529178559f68d6040d

#include <string>
#include "./is_byte.hpp"
#include "./enable_if.hpp"

namespace jbms {

template <class Container, JBMS_ENABLE_IF(is_byte_range<Container>)>
void append_hex(Container &c, unsigned char x) {
  constexpr const char *letters = "0123456789abcdef";
  c += letters[x / 16];
  c += letters[x % 16];
}

template <class Range, JBMS_ENABLE_IF(is_byte_range<Range>)>
std::string hex_string(Range const &range) {
  std::string result;
  result.reserve(2*std::distance(std::begin(range),std::end(range)));
  for (unsigned char x : range) {
    append_hex(result, x);
  }
  return result;
}

}

#endif /* HEADER GUARD */
