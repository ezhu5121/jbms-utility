#ifndef HEADER_GUARD_8bf4355b29ef79684b82acc6cbf9f10b
#define HEADER_GUARD_8bf4355b29ef79684b82acc6cbf9f10b

#include <limits.h>

namespace jbms {

template <class T>
inline T rotate_left(T x, int r) {
  return (x << r) | (x >> (sizeof(T) * CHAR_BIT - r));
}

template <class T>
inline T rotate_right(T x, int r) {
  return (x >> r) | (x << (sizeof(T) * CHAR_BIT - r));
}

}  // namespace jbms

#endif /* HEADER GUARD */
