#ifndef HEADER_GUARD_42e8b55ec8c2ea03149dca15b7358c3c
#define HEADER_GUARD_42e8b55ec8c2ea03149dca15b7358c3c

namespace jbms {

/**
 * \brief Returns a pointer to the element that matches
 *
 * This overload is only defined for "container" types with a find member.
 **/
template <class Container, class Key>
auto find_ptr(Container &&container, Key &&key) -> decltype(&*container.find(std::forward<Key>(key))) {
  auto it = container.find(std::forward<Key>(key));
  using Result = decltype(&*it);
  if (it == container.end())
    return Result(nullptr);
  return &*it;
}

}

#endif /* HEADER GUARD */
