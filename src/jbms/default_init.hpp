#ifndef HEADER_GUARD_d3b523a830b3e472a0d071c142553383
#define HEADER_GUARD_d3b523a830b3e472a0d071c142553383

#include <memory>
#include <vector>

namespace jbms {

/**
 * @brief Allocator adaptor that interposes construct() calls to
 * convert value initialization into default initialization.
 *
 * This is useful for making std::vector not value-initialize (zero) primitive types.
 *
 * See http://stackoverflow.com/questions/21028299/is-this-behavior-of-vectorresizesize-type-n-under-c11-and-boost-container/21028912#21028912
 **/
template <typename T, typename A = std::allocator<T>>
class default_init_allocator : public A {
  typedef std::allocator_traits<A> a_t;

public:
  template <typename U>
  struct rebind {
    using other = default_init_allocator<U, typename a_t::template rebind_alloc<U>>;
  };

  using A::A;

  template <typename U>
  void construct(U *ptr) {
    ::new (static_cast<void *>(ptr)) U;
  }
  template <typename U, typename... Args>
  void construct(U *ptr, Args &&... args) {
    a_t::construct(static_cast<A &>(*this), ptr, std::forward<Args>(args)...);
  }
};

/**
 * @brief Vector type that uses default_init_allocator.
 **/
template <class T,class Allocator=std::allocator<T>>
using default_init_vector = std::vector<T,default_init_allocator<T>>;

/**
 * @brief Resizes a vector, using default rather than value initialization to construct any additional elements.
 *
 * This relies on code that is not legal C++, but should work in practice:
 * We reinterpret std::vector<T,Alloc> as default_init_vector<T,Alloc>.
 **/
template <class T, class Alloc>
void default_init_resize(std::vector<T,Alloc> &x, size_t new_size) {
  using orig_vec = std::vector<T,Alloc>;
  using default_init_vec_t = default_init_vector<T,Alloc>;
  static_assert(sizeof(orig_vec) == sizeof(default_init_vec) &&
                alignof(orig_vec) == alignof(default_init_vec),
                "Size of original and default_init vectors are unexpectedly not the same or differ in alignment.");
  union U {
    orig_vec orig;
    default_init_vec temp;
  };
  reinterpret_cast<U&>(x).temp.resize(new_size);
}


}

#endif /* HEADER GUARD */
