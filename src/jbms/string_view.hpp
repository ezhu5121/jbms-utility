#ifndef HEADER_GUARD_f1b800462f93116ce9062008a64e03c1
#define HEADER_GUARD_f1b800462f93116ce9062008a64e03c1

#include <experimental/string_view>

namespace jbms {

using std::experimental::basic_string_view;
using std::experimental::string_view;
using std::experimental::wstring_view;

namespace literals {
namespace string_view_literals = std::experimental::string_view_literals;
using namespace string_view_literals;
}

}

#endif /* HEADER GUARD */
