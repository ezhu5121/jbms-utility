#ifndef HEADER_GUARD_0c213cae237e36b80dbe23f595c19d82
#define HEADER_GUARD_0c213cae237e36b80dbe23f595c19d82

#include <boost/test/predicate_result.hpp>

#include "jbms/print.hpp"
#include <boost/range/algorithm/equal.hpp>

namespace jbms {

namespace test {
template <class RangeA, class RangeB>
boost::test_tools::predicate_result sequences_equal(RangeA const &range_a, RangeB const &range_b) {
  if (!boost::equal(range_a, range_b)) {
    boost::test_tools::predicate_result result(false);
    result.message() << "\n" << jbms::streamable(range_a) << "\nvs\n" << jbms::streamable(range_b) << "\n";
    return result;
  }
  return true;
}
}

}

#endif /* HEADER GUARD */
