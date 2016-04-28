#ifndef HEADER_GUARD_0cd5bfda218af99198a2344268454da8
#define HEADER_GUARD_0cd5bfda218af99198a2344268454da8

namespace jbms {

template <class A, class B, class ThreeWayComp, class CallbackA, class CallbackB, class CallbackAB>
void generate_merge(A &&a,
                    B &&b,
                    ThreeWayComp &&comp,
                    CallbackA &&callbackA,
                    CallbackB &&callbackB,
                    CallbackAB &&callbackAB) {
  auto a_it = a.begin();
  auto b_it = b.begin();

  while (a_it != a.end() && b_it != b.end()) {
    auto &&a_t = *a_it;
    auto &&b_t = *b_it;

    auto result = comp(a_t, b_t);

    if (result < 0) {
      callbackA(a_t);
      ++a_it;
    } else if (result > 0) {
      callbackB(b_t);
      ++b_it;
    } else {
      callbackAB(a_t, b_t);
      ++a_it;
      ++b_it;
    }
  }
  while (a_it != a.end()) {
    callbackA(*a_it);
    ++a_it;
  }
  while (b_it != b.end()) {
    callbackB(*b_it);
    ++b_it;
  }
}


} // namespace jbms


#endif /* HEADER GUARD */
