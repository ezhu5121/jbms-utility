// This is based on code from https://github.com/viboes/tags/blob/master/include/yafpl/v1/functional/overload.hpp

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef HEADER_GUARD_ecf332f7fd4595b28920796094e7ebe0
#define HEADER_GUARD_ecf332f7fd4595b28920796094e7ebe0

#include <functional>

namespace jbms {

namespace detail {


/**
 * Simple wrapper around a function object or function pointer (including member function) that is inheritable.
 **/
template <class F>
struct inheritable_function_type {
  using type = F;
};

template <class R, class ...X>
struct inheritable_function_type<R(*)(X...)> {
  struct type {
    using fun = R(*)(X...);
    fun f;
    constexpr type(fun f) : f(f) {}
    constexpr R operator()(X... x) const {
      return f(static_cast<X>(x)...);
    }
  };
};

template <class Tp, class X>
struct inheritable_function_type<Tp X::*> {
  using mem_t = Tp X::*;
  using mem_fn_t = decltype(std::mem_fn(std::declval<mem_t>()));

  struct type : mem_fn_t {
    constexpr type(mem_t mp) : mem_fn_t(std::mem_fn(mp)) {}
  };
};

template <class F>
using inheritable_function = typename inheritable_function_type<F>::type;

template <class F, class G>
struct overloaded : inheritable_function<F>, inheritable_function<G> {
  using F::operator();
  using G::operator();

  template <class F_, class G_>
  overloaded(F_ &&f, G_ &&g)
      : inheritable_function<F>(std::forward<F_>(f)), inheritable_function<G>(std::forward<G_>(g)) {}
};

}

template <class F>
auto overload(F&& f) {
  return f;
}

template <class F1, class F2, class ...Fs>
auto overload(F1 &&f1, F2 &&f2, Fs &&... fs) {
  using F12 = detail::overloaded<std::decay_t<F1>,std::decay_t<F2>>;
  return overload(F12(std::forward<F1>(f1), std::forward<F2>(f2)), std::forward<Fs>(fs)...);
}

}

#endif /* HEADER GUARD */
