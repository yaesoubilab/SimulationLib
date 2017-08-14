#include "catch.hpp"
#include "../include/SimulationLib/utils/Bound.h"

template <int lower, int upper>
using BoundInt = Bound<int, lower, upper>;

TEST_CASE("Hello_world_bounds", "[bound]") {
    BoundInt<0, 5> n{5};

    REQUIRE( n     == 5 );
    REQUIRE( n()   == 5 );
    REQUIRE( n + 0 == 5 );
    REQUIRE( n - 0 == 5 );
    REQUIRE( n * 1 == 5 );
    REQUIRE( n / 1 == 5 );

    REQUIRE_THROWS(  n += 1 );
    REQUIRE_NOTHROW( n -= 5 ); // n should now be 0
    REQUIRE(         n == 0 );

    n = BoundInt<0,6>{1}; // n should now be 1

    REQUIRE( n   == 1 );
    REQUIRE( n() == 1 );
}
