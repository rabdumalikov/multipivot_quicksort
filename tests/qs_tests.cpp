//  Copyright (c) 2019 Rustam Abdumalikov
//
//  "eswitch_v4" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include "qs.hpp"
#include <iostream>
#include <list>
#include <array>
#include <vector>
#include <string>
#include <boost/range/adaptor/indexed.hpp>
#include <chrono>

#include <random>

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch2/catch.hpp"

std::vector<int> getRandomList( int n )
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, n*n); // define the range

    std::vector<int> output;
    output.reserve( n );

    for(int i=0; i<n; ++i)
        output.push_back( distr(gen) );

    return output;
}

void printList( const std::vector< int > & pivots ) {
    for( auto p : pivots )
        std::cout << p << ", ";
    std::cout << std::endl;
}

// TEST_CASE( "qs::range", "" ) 
// {
//     using namespace std;

//     SECTION( "inside_range" ) {
//         for( int np = 1; np <= 10000; ++np ) {
//             double total_duration = 0.0;
//             int num_durations = 0;
            
//             for( int i = 1; i < 30; ++i ) {
//                 std::vector< int > v = getRandomList( 300000 );

//                 std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

//                 quicksort( v, np );
                
//                 // std::qsort(
//                 //     v.data(),
//                 //     v.size(),
//                 //     sizeof(int),
//                 //     [](const void* x, const void* y) {
//                 //         return ( *(int*)x - *(int*)y );
//                 //     });
                
//                 std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

//                 if( !std::is_sorted( std::begin( v ), std::end( v ) ) )
//                 {
//                     std::cout << "Failed to sort!!!" << std::endl;
//                     break;
//                 }
                
//                 total_duration +=  std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
                
//                 ++num_durations;
                
//             }

//             std::cout << "[" << np << "] Time difference = " << total_duration/num_durations << "[ms]" << std::endl;
//         }

//         REQUIRE( true );
//     }
// }

TEST_CASE( "sort_pivots_1", "" ) 
{
    using namespace std;

    SECTION( "sort_pivots" ) {
        vector< int > values = { 1, 2, 3, 4, 5, 7, 6 };
        vector< int > pivots = {6};
        const auto results = sort_pivots( values, 0, values.size(), std::move(pivots) );

        REQUIRE( values == vector<int>{ 6, 2, 3, 4, 5, 7, 1  } );
        REQUIRE( results == vector<int>{ 0 } );
    }
}

TEST_CASE( "sort_pivots_2", "" ) 
{
    using namespace std;

    SECTION( "all_permutations" ) {
        vector< int > pivots = {5,6};
        do {
            //printList( pivots );
            
            vector< int > values = { 1, 2, 3, 4, 5, 7, 6 };
            
            auto ps = pivots;
            const auto results = sort_pivots( values, 0, values.size(), std::move(ps) );

            REQUIRE( values == vector<int>{ 6, 7, 3, 4, 5, 1, 2} );
            REQUIRE( results == vector<int>{ 0, 1 } );
        } while( std::next_permutation(std::begin(pivots), std::end(pivots)) );
    }
}

TEST_CASE( "sort_pivots_3", "" ) 
{
    using namespace std;

    SECTION( "all_permutations" ) {
        vector< int > pivots = {0, 3, 6};
        do {
            //printList( pivots );
            
            vector< int > values = { 4, 2, 3, 6, 5, 7, 1 };

            auto ps = pivots;
            const auto results = sort_pivots( values, 0, values.size(), std::move(ps) );

            REQUIRE( values == vector<int>{ 1, 4, 6, 2, 5, 7, 3 } );
            REQUIRE( results == vector<int>{ 0, 1, 2 } );
        } while( std::next_permutation(std::begin(pivots), std::end(pivots)) );
    }
}

TEST_CASE( "sort_pivots_4", "" ) 
{
    using namespace std;

    SECTION( "all_permutations" ) {
        vector< int > pivots = {0, 1, 2, 3};
        do {
            //printList( pivots );
            
            vector< int > values = { 4, 2, 3, 6, 5, 7, 1 };

            auto ps = pivots;
            const auto results = sort_pivots( values, 0, values.size(), std::move(ps) );

            REQUIRE( values == vector<int>{ 2, 3, 4, 6, 5, 7, 1 } );
            REQUIRE( results == vector<int>{ 0, 1, 2, 3 } );
        } while( std::next_permutation(std::begin(pivots), std::end(pivots)) );
    }
}

TEST_CASE( "sort_pivots_5", "" ) 
{
    using namespace std;

    SECTION( "all_permutations" ) {
        vector< int > pivots = {2, 3, 4, 5, 6};
        do {
            //printList( pivots );
            
            vector< int > values = { 4, 2, 3, 6, 5, 7, 1 };

            auto ps = pivots;
            const auto results = sort_pivots( values, 0, values.size(), std::move(ps) );

            REQUIRE( values == vector<int>{ 1, 3, 5, 6, 7, 2, 4 } );
            REQUIRE( results == vector<int>{ 0, 1, 2, 3, 4 } );
        } while( std::next_permutation(std::begin(pivots), std::end(pivots)) );
    }
}

// bool test_partitioning()
// {
//     for( int i = 0; i < 100; ++i )
//     {
//         std::vector< int > v = getRandomList( 50 );

//         auto l = get_pivot( v, 0, v.size() - 1, 4 );
        
//         general_partition( v, 0, v.size() - 1,  l );

//         for( auto pivot : l )
//         {
//             for( int j = 0; j <= pivot; ++j )
//             {
//                 if( v[pivot] < v[j] ) { printf( "Error" ); return false; }
//             }
//         }

//         auto lastIdx = *(l.rbegin());

//         for( auto k = lastIdx+1; k < v.size(); ++k )
//         {
//             if( !(v[k] > v[lastIdx]) ) { printf( "Error" ); return false; }
//         }
//     }

//     return true;
// }
