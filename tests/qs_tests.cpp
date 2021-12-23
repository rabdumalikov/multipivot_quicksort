//  Copyright (c) 2019 Rustam Abdumalikov
//
//  "eswitch_v4" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2/catch.hpp"

#include "qs.hpp"
#include <iostream>
#include <list>
#include <array>
#include <vector>
#include <string>
#include <boost/range/adaptor/indexed.hpp>

TEST_CASE( "eswitch_v5::range", "" ) 
{
    // using namespace eswitch_v5;
    // using namespace std;

    // SECTION( "inside_range" )
    // {
    //     auto r = 
    //     eswitch( 6 )
    //     (
    //         Case( _1.between( 1, 10 ) ) { return true; },
    //         Default   { return false; } 
    //     );

    //     REQUIRE( r );    
    // }

    // SECTION( "inside_range" )
    // {
    //     auto r = 
    //     eswitch( 10 )
    //     (
    //         Case( _1.between( 1, 10 ) ) { return true; },
    //         Default   { return false; } 
    //     );

    //     REQUIRE( !r );    
    // }

    // SECTION( "within_range" )
    // {
    //     auto r = 
    //     eswitch( 6 )
    //     (
    //         Case( _1.within( 1, 10 ) ) { return true; },
    //         Default   { return false; } 
    //     );

    //     REQUIRE( r );    
    // }

    // SECTION( "within_range" )
    // {
    //     auto r = 
    //     eswitch( 10 )
    //     (
    //         Case( _1.within( 1, 10 ) ) { return true; },
    //         Default   { return false; } 
    //     );

    //     REQUIRE( r );    
    // }
}

