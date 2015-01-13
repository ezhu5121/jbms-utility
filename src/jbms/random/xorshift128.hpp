#ifndef HEADER_GUARD_d4d4c3ab1ce8410793df53f33d456798
#define HEADER_GUARD_d4d4c3ab1ce8410793df53f33d456798

#include <stdint.h>

namespace jbms {
namespace random {

/**
 * Implements the xorshift128 random number generator.
 *
 * This random number generator is known for being very fast.
 *
 * It should not be used for cryptographic purposes.
 **/
struct xorshift128 {
  uint32_t x = 123456789;
  uint32_t y = 362436069;
  uint32_t z = 521288629;
  uint32_t w = 88675123;

  using result_type = uint32_t;

  uint32_t operator()() {
    uint32_t t;

    t = x ^ (x << 11);
    x = y;
    y = z;
    z = w;
    return w = w ^ (w >> 19) ^ (t ^ (t >> 8));
  }

  static constexpr uint32_t min() { return 0; }
  static constexpr uint32_t max() { return (uint32_t)-1; }
};


}
}

#endif /* HEADER GUARD */
