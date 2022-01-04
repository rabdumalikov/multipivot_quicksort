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

std::vector<int64_t> getRandomList( int64_t n )
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, n*n); // define the range

    std::vector<int64_t> output;
    output.reserve( n );

    for(int64_t i=0; i<n; ++i)
        output.push_back( distr(gen) );

    return output;
}

void printList( const std::vector< int64_t > & pivots ) {
    for( auto p : pivots )
        std::cout << p << ", ";
    std::cout << std::endl;
}

TEST_CASE( "sort_pivots_1", "" ) 
{
    using namespace std;

    SECTION( "sort_pivots" ) {
        vector< int64_t > values = { 1, 2, 3, 4, 5, 7, 6 };
        vector< int64_t > pivots = {6};
        const auto results = sort_pivots( values, 0, values.size(), std::move(pivots) );

        REQUIRE( values == vector<int64_t>{ 6, 2, 3, 4, 5, 7, 1  } );
        REQUIRE( results == vector<int64_t>{ 0 } );
    }
}

TEST_CASE( "sort_pivots_2", "" ) 
{
    using namespace std;

    SECTION( "all_permutations" ) {
        vector< int64_t > pivots = {5,6};
        do {
            //printList( pivots );
            
            vector< int64_t > values = { 1, 2, 3, 4, 5, 7, 6 };
            
            auto ps = pivots;
            const auto results = sort_pivots( values, 0, values.size(), std::move(ps) );

            REQUIRE( values == vector<int64_t>{ 6, 7, 3, 4, 5, 1, 2} );
            REQUIRE( results == vector<int64_t>{ 0, 1 } );
        } while( std::next_permutation(std::begin(pivots), std::end(pivots)) );
    }
}

TEST_CASE( "sort_pivots_3", "" ) 
{
    using namespace std;

    SECTION( "all_permutations" ) {
        vector< int64_t > pivots = {0, 3, 6};
        do {
            //printList( pivots );
            
            vector< int64_t > values = { 4, 2, 3, 6, 5, 7, 1 };

            auto ps = pivots;
            const auto results = sort_pivots( values, 0, values.size(), std::move(ps) );

            REQUIRE( values == vector<int64_t>{ 1, 4, 6, 2, 5, 7, 3 } );
            REQUIRE( results == vector<int64_t>{ 0, 1, 2 } );
        } while( std::next_permutation(std::begin(pivots), std::end(pivots)) );
    }
}

TEST_CASE( "sort_pivots_4", "" ) 
{
    using namespace std;

    SECTION( "all_permutations" ) {
        vector< int64_t > pivots = {0, 1, 2, 3};
        do {
            //printList( pivots );
            
            vector< int64_t > values = { 4, 2, 3, 6, 5, 7, 1 };

            auto ps = pivots;
            const auto results = sort_pivots( values, 0, values.size(), std::move(ps) );

            REQUIRE( values == vector<int64_t>{ 2, 3, 4, 6, 5, 7, 1 } );
            REQUIRE( results == vector<int64_t>{ 0, 1, 2, 3 } );
        } while( std::next_permutation(std::begin(pivots), std::end(pivots)) );
    }
}

TEST_CASE( "sort_pivots_5", "" ) 
{
    using namespace std;

    SECTION( "all_permutations" ) {
        vector< int64_t > pivots = {2, 3, 4, 5, 6};
        do {
            //printList( pivots );
            
            vector< int64_t > values = { 4, 2, 3, 6, 5, 7, 1 };

            auto ps = pivots;
            const auto results = sort_pivots( values, 0, values.size(), std::move(ps) );

            REQUIRE( values == vector<int64_t>{ 1, 3, 5, 6, 7, 2, 4 } );
            REQUIRE( results == vector<int64_t>{ 0, 1, 2, 3, 4 } );
        } while( std::next_permutation(std::begin(pivots), std::end(pivots)) );
    }
}

TEST_CASE( "sort_pivots_narrow_range_middle", "" ) 
{
    using namespace std;

    SECTION( "all_permutations" ) {
        vector< int64_t > pivots = {1, 2, 3};
        do {
            //printList( pivots );
            
            vector< int64_t > values = { 4, 6, 2, 3, 5, 7, 1 };

            auto ps = pivots;
            const auto results = sort_pivots( values, 0, 4, std::move(ps) );

            REQUIRE( values == vector<int64_t>{ 2, 3, 6, 4, 5, 7, 1 } );
            REQUIRE( results == vector<int64_t>{ 0, 1, 2 } );
        } while( std::next_permutation(std::begin(pivots), std::end(pivots)) );
    }
}

TEST_CASE( "sort_pivots_narrow_range_begin", "" ) 
{
    using namespace std;

    SECTION( "all_permutations" ) {
        vector< int64_t > pivots = {1, 2, 3};
        do {
            //printList( pivots );
            
            vector< int64_t > values = { 4, 6, 2, 3, 5, 7, 1 };

            auto ps = pivots;
            const auto results = sort_pivots( values, 0, 3, std::move(ps) );

            REQUIRE( values == vector<int64_t>{ 2, 3, 6, 4, 5, 7, 1 } );
            REQUIRE( results == vector<int64_t>{ 0, 1, 2 } );
        } while( std::next_permutation(std::begin(pivots), std::end(pivots)) );
    }
}

TEST_CASE( "sort_pivots_narrow_range_end", "" ) 
{
    using namespace std;

    SECTION( "all_permutations" ) {
        vector< int64_t > pivots = {4, 5, 6};
        do {
            //printList( pivots );
            
            vector< int64_t > values = { 4, 6, 2, 3, 5, 7, 1 };

            auto ps = pivots;
            const auto results = sort_pivots( values, 3, 6, std::move(ps) );

            REQUIRE( values == vector<int64_t>{ 4, 6, 2, 1, 5, 7, 3 } );
            REQUIRE( results == vector<int64_t>{ 3, 4, 5 } );
        } while( std::next_permutation(std::begin(pivots), std::end(pivots)) );
    }
}

TEST_CASE( "get_pivots_narrow_pivot_numbers", "1-10" ) 
{
    using namespace std;

    SECTION( "full size" ) {
        vector< int64_t > values = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        const auto results = get_pivot( values, 0, values.size()-1, values.size() );

        REQUIRE( results.size() == (values.size()-1)/ 2 );
        REQUIRE( results[0] >= 0 );        
        REQUIRE( results[0] <= 4 );
    }
}

TEST_CASE( "get_pivots_1", "1-10" ) 
{
    using namespace std;

    SECTION( "0-9" ) {
        vector< int64_t > values = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        const auto results = get_pivot( values, 0, values.size()-1, 1 );

        REQUIRE( results.size() == 1 );
        REQUIRE( results[0] >= 0 );        
        REQUIRE( results[0] <= 9 );
    }

    SECTION( "0-4" ) {
        vector< int64_t > values = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        const auto results = get_pivot( values, 0, 4, 1 );

        REQUIRE( results.size() == 1 );
        REQUIRE( results[0] >= 0 );        
        REQUIRE( results[0] <= 4 );
    }

    SECTION( "5-9" ) {
        vector< int64_t > values = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

        const auto results = get_pivot( values, 0, 4, 1 );

        REQUIRE( results.size() == 1 );
        REQUIRE( results[0] >= 0 );        
        REQUIRE( results[0] <= 4 );
    }
}

TEST_CASE( "general_partition", "" ) 
{
    using namespace std;

    SECTION( "1_pivot" ) {
        vector< int64_t > values = { 1, 2, 3, 4, 6, 7, 8, 9, 10 };

        do {
            vector< int64_t > new_values;
            new_values.push_back( 5 );

            for( auto v : values ) new_values.push_back( v );

            vector< int64_t > pivots = { 0 };
            
            const auto results = general_partition( new_values, 0, new_values.size()-1, pivots );
            
            REQUIRE( new_values[results[0]] == 5 );

            for( int i = 0; i < new_values.size(); ++i )
            {
                if( i == results[0] ) continue;
                if( i < results[0] )
                    REQUIRE( new_values[i] <= new_values[results[0]] );
                else if( i > results[0] )
                    REQUIRE( new_values[i] > new_values[results[0]] );
            }

        } while( std::next_permutation(std::begin(values), std::end(values)) );

    }
}

TEST_CASE( "general_partition2", "" ) 
{
    using namespace std;

    SECTION( "2_pivot" ) {
        vector< int64_t > values = { 1, 2, 3, 4, 9, 10, 11 };

        do {
            vector< int64_t > new_values;
            new_values.push_back( 5 );
            new_values.push_back( 8 );

            for( auto v : values ) new_values.push_back( v );

            vector< int64_t > pivots = { 0, 1 };
            
            const auto results = general_partition( new_values, 0, new_values.size()-1, pivots );
            
            REQUIRE( new_values[results[0]] == 5 );
            REQUIRE( new_values[results[1]] == 8 );

            for( int i = 0; i < new_values.size(); ++i )
            {
                if( i == results[0] || i == results[1] ) continue;
                
                if( i < results[0] )
                    REQUIRE( new_values[i] <= new_values[results[0]] );
                else if( i > results[0] && i < results[1] )
                {
                    REQUIRE( new_values[i] > new_values[results[0]] );
                    REQUIRE( new_values[i] <= new_values[results[1]] );
                }
                else if( i > results[0] && i > results[1] )
                {
                    REQUIRE( new_values[i] > new_values[results[1]] );
                }
            }

        } while( std::next_permutation(std::begin(values), std::end(values)) );

    }
}

TEST_CASE( "auxiliary_functions", "" ) 
{
    SECTION( "less_or_equal_1" ) {
        std::vector< int64_t > values = { 1, 2, 3, 4, 9, 10, 11 };
        std::vector< int64_t > pivots = { 0, 1, 2, 3, 4, 5, 6 };
        int64_t val;
        std::vector< bool > correctResults = { 1, 1, 1, 1, 1, 1, 1, 1 };
        
        int i =0;
        for(auto v: values){
            val=v;
            auto results=less_or_equal(values,pivots,val);
            REQUIRE(results==correctResults);
            correctResults[i]=0;
            i++;
        }
        
    }
    SECTION( "greater_1" ) {
        std::vector< int64_t > values = { 1, 2, 3, 4, 9, 10, 11 };
        std::vector< int64_t > pivots = { 0, 1, 2, 3, 4, 5, 6 };
        int64_t val;
        std::vector< bool > correctResults = { 1, 0, 0, 0, 0, 0, 0, 0 };
        
        int i =0;
        for(auto v: values){
            val=v;
            auto results=greater(values,pivots,val);
            REQUIRE(results==correctResults);
            i++;
            correctResults[i]=1;
        }
        
    }
}

TEST_CASE( "quicksort", "" ) 
{
    using namespace std;

    SECTION( "100_entries" ) {
        for( int64_t np = 1; np <= 90; ++np ) {
            std::vector< int64_t > v = getRandomList( 100 );            
            quicksort( v, np );

            REQUIRE( std::is_sorted( std::begin( v ), std::end( v ) ) );
        }
    }

    SECTION( "1000_entries" ) {
        for( int64_t np = 1; np <= 900; ++np ) {
            std::vector< int64_t > v = getRandomList( 1000 );            
            quicksort( v, np );

            REQUIRE( std::is_sorted( std::begin( v ), std::end( v ) ) );
        }
    }

    SECTION( "10000_entries" ) {
        for( int64_t np = 1; np <= 50; ++np ) {
            std::vector< int64_t > v = getRandomList( 10000 );            
            quicksort( v, np );

            REQUIRE( std::is_sorted( std::begin( v ), std::end( v ) ) );
        }
    }
}


TEST_CASE( "qs::range", "" ) 
{
    using namespace std;

    SECTION( "inside_range" ) {
        for( int64_t np = 1; np <= 10000; ++np ) {
            double total_duration = 0.0;
            int64_t num_durations = 0;
            
            for( int64_t i = 1; i < 30; ++i ) {
                std::vector< int64_t > v = getRandomList( 300000 );

                std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

                quicksort( v, np );
                
                // std::qsort(
                //     v.data(),
                //     v.size(),
                //     sizeof(int64_t),
                //     [](const void* x, const void* y) {
                //         return ( *(int64_t*)x - *(int64_t*)y );
                //     });
                
                std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

                if( !std::is_sorted( std::begin( v ), std::end( v ) ) )
                {
                    std::cout << "Failed to sort!!!" << std::endl;
                    break;
                }
                
                total_duration +=  std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
                
                ++num_durations;
                
            }

            std::cout << "[" << np << "] Time difference = " << total_duration/num_durations << "[ms]" << std::endl;
        }

        REQUIRE( true );
    }
}

// bool test_partitioning()
// {
//     for( int64_t i = 0; i < 100; ++i )
//     {
//         std::vector< int64_t > v = getRandomList( 50 );

//         auto l = get_pivot( v, 0, v.size() - 1, 4 );
        
//         general_partition( v, 0, v.size() - 1,  l );

//         for( auto pivot : l )
//         {
//             for( int64_t j = 0; j <= pivot; ++j )
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
