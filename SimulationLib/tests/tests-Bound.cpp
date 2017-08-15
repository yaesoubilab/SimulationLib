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
TEST_CASE("Construct invalid values", "[bound]") {
    bool threw;

    SECTION( "Invalid int values" ) {
        threw = false;

        SECTION( "Greater than upper bound" ) {
            try {
                BoundInt<0,5> n{6};
            } catch(...) {
                threw = true;
            }

            REQUIRE( threw == true );
        }

        SECTION( "Less than lower bound" ) {
            try {
                BoundInt<0,5> n{-1};
            } catch(...) {
                threw = true;
            }

            REQUIRE( threw == true );
        }

        SECTION( "Lower and upper bound are the same, less than lower bound" ) {
            try {
                BoundInt<0,0> n{-1};
            } catch(...) {
                threw = true;
            }

            REQUIRE( threw == true );
        }

        SECTION( "Lower and upper bound are the same, greater than upper bound" ) {
            try {
                BoundInt<0,0> n{1};
            } catch(...) {
                threw = true;
            }

            REQUIRE( threw == true );
        }
    }
    // other types?
}


//
// - Trying to copy-construct invalid values
//
// - Prefix and postfix increment and decrement

TEST_CASE("Prefix and postfix increment and decrement", "[bound]") {
    SECTION( "Legal behaviors" ) {

        SECTION( "Basic increment, decrement" ) {
            BoundInt<0, 5> n{3};

            REQUIRE( n == 3 );

            n++;
            REQUIRE( n == 4 );

            ++n;
            REQUIRE( n == 5 );

            n--;
            REQUIRE( n == 4 );

            --n;
            REQUIRE( n == 3 );
        }

        SECTION( "Check return values of increment/decrement" ) {
            BoundInt<0, 5> n{3};

            REQUIRE( n++ == 3 );
            REQUIRE( ++n == 5 );

            REQUIRE( n-- == 5 );
            REQUIRE( --n == 3 );
        }
    }

    SECTION( "Illegal behaviors" ) {
        SECTION( "Basic increment past bounds" ) {
            BoundInt<0, 5> n{5};

            REQUIRE_THROWS(n++);
            REQUIRE_THROWS(++n);
        }

        SECTION( "Basic decrement past bounds" ) {
            BoundInt<0, 5> n{0};

            REQUIRE_THROWS(n--);
            REQUIRE_THROWS(--n);
        }
    }
}

//
// - +=, -=, *=, /=, %= operators on Bound objects, and on numbers
//    - explore behavior of numbers of different types from that of Bound object

TEST_CASE("+=, -=, *=, /=, %= operators on Bound objects, and on numbers", "[bound]") {
    SECTION( "Legal behaviors on BoundInts" ) {

        SECTION( "Basic +=, -= with same bounds" ) {
            BoundInt<0, 5> n{3};
            BoundInt<0, 5> m{2};

            REQUIRE( n == 3 );
            REQUIRE( m == 2 );

            REQUIRE_NOTHROW( n -= m );
            REQUIRE( n == 1 );

            REQUIRE_NOTHROW( n += n );
            REQUIRE( n == 2 );
        }

        SECTION( "Basic +=, -= with different bounds" ) {
            BoundInt<0, 5> n{3};
            BoundInt<0, 10> m{2};

            REQUIRE( n == 3 );
            REQUIRE( m == 2 );

            REQUIRE_NOTHROW( n -= m );
            REQUIRE( n == 1 );

            REQUIRE_NOTHROW( n += n );
            REQUIRE( n == 2 );

            REQUIRE_NOTHROW( m += n );
            REQUIRE_NOTHROW( m += n );
            REQUIRE( m == 6 );
        }

        SECTION( "Basic *=, /= with same bounds" ) {
            BoundInt<0, 5> n{2};
            BoundInt<0, 5> m{2};
            BoundInt<0, 5> l{0};

            REQUIRE( n == 2 );
            REQUIRE( m == 2 );
            REQUIRE( l == 0 );

            REQUIRE_NOTHROW( n *= m );
            REQUIRE( n == 4 );

            REQUIRE_NOTHROW( n /= n );
            REQUIRE( n == 1 );

            REQUIRE_NOTHROW( n *= l );
            REQUIRE( n == 0 );
        }

        SECTION( "Basic *=, /= with different bounds" ) {
            BoundInt<0, 5> n{2};
            BoundInt<0, 10> m{2};
            BoundInt<0, 0> l{0};

            REQUIRE( n == 2 );
            REQUIRE( m == 2 );
            REQUIRE( l == 0 );

            REQUIRE_NOTHROW( n *= m );
            REQUIRE( n == 4 );

            REQUIRE_NOTHROW( m *= n );
            REQUIRE( n == 10 );

            REQUIRE_NOTHROW( n /= n );
            REQUIRE( n == 1 );

            REQUIRE_NOTHROW( n *= l );
            REQUIRE( n == 0 );
        }

        SECTION( "Basic %= with same bounds" ) {
            BoundInt<0, 5> n{5};
            BoundInt<0, 5> m{3};
            BoundInt<0, 5> l{2};
            BoundInt<0, 5> k{1};

            REQUIRE( n == 5 );
            REQUIRE( m == 3 );
            REQUIRE( l == 2 );
            REQUIRE( k == 1 );

            REQUIRE_NOTHROW( n %= m );
            REQUIRE( n == 2 );

            REQUIRE_NOTHROW( n %= l );
            REQUIRE( n == 0 );

            REQUIRE_NOTHROW( n %= k );
            REQUIRE( n == 0 );
        }
    }

    SECTION( "Legal behaviors on int" ) {

        SECTION( "Basic +=, -=" ) {
            BoundInt<0, 5> n{5};

            REQUIRE( n == 5 );

            REQUIRE_NOTHROW( n -= 5 );
            REQUIRE( n == 0 );

            REQUIRE_NOTHROW( n += 5 );
            REQUIRE( n == 5 );
        }

        SECTION( "Basic *=, /=" ) {
            BoundInt<0, 5> n{2};

            REQUIRE( n == 2 );

            REQUIRE_NOTHROW( n *= 2 );
            REQUIRE( n == 4 );

            REQUIRE_NOTHROW( n /= 4 );
            REQUIRE( n == 1 );

            REQUIRE_NOTHROW( n *= 0 );
            REQUIRE( n == 0 );
        }

        SECTION( "Basic %=" ) {
            BoundInt<0, 5> n{5};

            REQUIRE( n == 5 );

            REQUIRE_NOTHROW( n %= 3 );
            REQUIRE( n == 2 );

            REQUIRE_NOTHROW( n %= 2 );
            REQUIRE( n == 0 );

            REQUIRE_NOTHROW( n %= 1 );
            REQUIRE( n == 0 );
        }
    }

    SECTION( "Illegal behaviors on int" ) {
        SECTION( "Basic += past bounds" ) {
            BoundInt<0, 5> n{2};

            REQUIRE_THROWS(n += 5);
            REQUIRE_THROWS(n += 4);
        }

        SECTION( "Basic -= past bounds" ) {
            BoundInt<0, 5> n{2};

            REQUIRE_THROWS(n -= 3);
            REQUIRE_THROWS(n -= 4);
        }

        SECTION( "Basic *= past bounds" ) {
            BoundInt<0, 5> n{2};

            REQUIRE_THROWS(n *= 4);
            REQUIRE_THROWS(n *= 3);
        }

        SECTION( "Basic /= past bounds" ) {
            BoundInt<0, 5> n{2};

            REQUIRE_THROWS(n /= -1);
            REQUIRE_THROWS(n /= -2);
        }
    }
}


//
// - All comparison operators on Bound objects and on numbers
//    - explore behavior of numbers of different types from that of Bound object
//
// - Assignment from Bound object and from numeric types
//    - explore behavior of numbers of different types from that of Bound object
//
// - All arithmetic operators on a mixture of Bound objects and numeric types
//    - explore behavior of numbers of different types from that of Bound object
