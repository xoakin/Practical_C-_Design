#include <catch2/catch_all.hpp>
#include "stack.h"
#include <array>
#include <algorithm>

SCENARIO("Modifying stack elements")
{
  auto &s = Stack::Instance();
  GIVEN("An array of elements")
  {
    std::array elements{ 1., 2., 3., 4. };
    WHEN("pushing all the array elements into the stack")
    {
      for (const auto e : elements) {
        s.push(e);
      }
      THEN("stack size must match array size"){
        REQUIRE(elements.size() == s.size());
      }
      THEN("N-first elements must be reversed"){
        const auto n_elems = s.getElements(elements.size());
        REQUIRE(std::equal(crbegin(elements), crend(elements), cbegin(n_elems)));
      }
    }
  }
}