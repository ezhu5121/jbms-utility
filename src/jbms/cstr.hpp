#ifndef HEADER_GUARD_ae667c7c64738f7e18dac1143bde1a74
#define HEADER_GUARD_ae667c7c64738f7e18dac1143bde1a74

#include <string>
#include <cstring>

namespace jbms {

/**
 * Represents a reference to a NUL-terminated C string.
 *
 * This is useful as a function parameter type, in order to allow either std::string or const char * values to be given.  While array_view<char> is more general, some APIs require NUL termination.
 **/
class cstr {
  const char *ptr_ = nullptr;
public:
  cstr(const char *ptr) : ptr_(ptr) {}
  cstr(std::string const &str) : ptr_(str.c_str()) {}
  const char *data() const { return ptr_; }
  const char *c_str() const { return ptr_; }
  operator const char *() const { return ptr_; }
  std::string str() const { return ptr_ ? std::string(ptr_) : std::string(); }

  bool empty() const { return !ptr_ || ptr_[0] == 0; }

  // Warning: This is not constant-time
  size_t size() const { return ptr_ ? std::strlen(ptr_) : 0; }

  // These overloads are necessary because of the way operator+ is defined for std::basic_string.
  friend std::string operator+(std::string const &x, cstr y) { return y ? x + y.data() : x; }

  friend std::string operator+(std::string &&x, cstr y) { return y ? std::move(x) + y.data() : std::move(x); }

  friend std::string operator+(cstr y, std::string const &x) { return y ? y.data() + x : x; }

  friend std::string operator+(cstr y, std::string &&x) { return y ? y.data() + std::move(x) : std::move(x); }

  friend std::string operator+(cstr x, cstr y) {
    if (x) {
      if (y)
        return x.str() + y.data();
      else
        return x.str();
    } else
      return y.str();
  }

  friend std::string operator+(cstr x, const char *y) { return x + cstr(y); }

  friend std::string operator+(const char *x, cstr y) { return cstr(x) + y; }
};


}

#endif /* HEADER GUARD */
