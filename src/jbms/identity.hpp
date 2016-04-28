#ifndef HEADER_GUARD_5490600df0c2bf25df122c935a523cd9
#define HEADER_GUARD_5490600df0c2bf25df122c935a523cd9

namespace jbms {

/**
 * Warning: This can return dangling references.  Do not bind the result with auto &&.
 **/
struct identity {
  template <class T>
  constexpr T&& operator()(T &&x) const { return static_cast<T&&>(x); }
};

}

#endif /* HEADER GUARD */
