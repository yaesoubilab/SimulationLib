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

    REQUIRE_THROWS(  n += 1 ); // this would value n at 6, not cool!
    REQUIRE_NOTHROW( n -= 5 ); // n should now be 0
    REQUIRE(         n == 0 );

    n = BoundInt<0,6>{1}; // n should now be 1

    REQUIRE( n   == 1 );
    REQUIRE( n() == 1 );
}

// Need to write tests for:
//
// - Trying to construct invalid values
//
// - Trying to copy-construct invalid values
//
// - Prefix and postfix increment and decrement
//
// - +=, -=, *=, /=, %/ operators on Bound objects, and on numbers
//    - explore behavior of numbers of different types from that of Bound object
//
// - All comparison operators on Bound objects and on numbers
//    - explore behavior of numbers of different types from that of Bound object
//
// - Assignment from Bound object and from numeric types
//    - explore behavior of numbers of different types from that of Bound object
//
// - All arithmetic operators on a mixture of Bound objects and numeric types
//    - explore behavior of numbers of different types from that of Bound object
