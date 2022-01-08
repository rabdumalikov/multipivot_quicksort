#include <stdlib.h>
#include <vector>
#include <stdint.h>
#include <iostream>
//#include <boost/range.hpp>
#include <algorithm>
#include <boost/range/irange.hpp>
//#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>
#include <random>
#include <qs.hpp>

void printList( const std::vector< int64_t > & list, const char * text = "" )
{
    std::cout << text;
    
    for( int64_t i = 0; i < list.size(); ++i )
    {
        std::cout << list[i] << " ";
    }

    printf( "\n\n" );
}

void printList( const std::vector< int64_t > & list, const std::vector< int64_t > & pivots, const int64_t to_swap, const char * text = "" )
{
    std::cout << text;
    
    for( int64_t i = 0; i < list.size(); ++i )
    {
        auto iter = std::find( std::begin(pivots), std::end(pivots), i);
        if( iter != std::end(pivots) )
        {
            std::cout << "[" << list[i] << "],";
        }
        else if( i == to_swap )
        {
            std::cout << "{" << list[i] << "},";
        }
        else
            std::cout << list[i] << ",";
    }

    printf( "\n\n" );
}

void printPivots( const std::vector< int64_t > & arr, const std::vector< int64_t > & pivots )
{
    std::cout << "PivotValues=";
    
    for( auto & pivot : pivots ) {
        std::cout << arr[pivot] << ",";
    }
    
    std::cout << "\n\n";
}

inline std::vector<int64_t> sort_pivots( std::vector<int64_t> & arr, int64_t start, int64_t end, std::vector<int64_t> && pivots )
{
    std::sort( std::begin( pivots ), std::end( pivots ) );
   
    std::vector<int64_t> new_pivots;
    new_pivots.reserve( pivots.size() );

    int64_t first_pivot = start;
    for( auto p : pivots )
    {
        new_pivots.push_back( first_pivot );

        std::swap( arr[p], arr[first_pivot]);

        ++first_pivot;
    }
    
    auto iter = std::begin( arr ) + start;
    
    std::sort( iter, iter + pivots.size() );
    
    return new_pivots;
}

std::random_device rd; // obtain a random number from hardware
std::mt19937 gen(rd()); // seed the generator

inline std::vector< int64_t > get_pivot( std::vector< int64_t > & arr, int64_t l, int64_t r, int64_t n )
{
    std::uniform_int_distribution<> distr(l, r); // define the range

    if( abs(r-l) < n ) {
        //std::cout << "Changed " << n << " to " << abs(r-l) << std::endl;  
        n = round( (abs(r-l)) / 2 );
    }
    
    std::vector< int64_t > pivots;
    pivots.reserve(n);

    for( int64_t i = 0; i < n; ++i )
    {
        while( true ) {
            auto new_pivot = distr(gen);
            auto iter = std::find( std::begin( pivots ), std::end( pivots ), new_pivot );
            
            if( iter != std::end( pivots ) ) continue;
            
            pivots.push_back( new_pivot );
            break;
        }
    }
    
    // sorting part
    pivots = sort_pivots( arr, l, r, std::move(pivots) );

    //printPivots( arr, pivots );
    
    return pivots;
}

inline std::vector< bool > less_or_equal( const std::vector< int64_t > & arr, const std::vector<int64_t> & pivots, const int64_t value )
{
    std::vector< bool > output( pivots.size() + 1, true );

    int64_t index = 0;

    for( const auto p : pivots ) {
        const auto res = arr[p] >= value;

        if( res ) break;

        output[index++] = res;
    }
    
    return output;
}

inline std::vector< bool > greater( const std::vector< int64_t > & arr, const std::vector<int64_t> & pivots, const int64_t value )
{
    std::vector< bool > output( pivots.size() + 1, true );

    int64_t index = pivots.size();
    
    for( const auto p : pivots | boost::adaptors::reversed ) {
        const auto res = arr[p] < value;
        
        // since all the pivot values are sorted, thus if value greater 
        // then last pivot implies that for the remaining pivots it also greater
        if( res ) break;
        
        output[index--] = res;
    }

    return output;
}

// TODO: cover with test
inline int64_t identify_new_sector( const std::vector< int64_t > & arr, const std::vector<int64_t> & pivots, const int64_t value )
{
    const auto lq = less_or_equal( arr, pivots, value );
    const auto gt = greater( arr, pivots, value );

    int64_t index = 0;

    for( const auto l : lq | boost::adaptors::indexed(0) )
    {
        const auto idx = l.index();

        if( l.value() && gt[index] )
        {
            index = idx;
            break;
        } 
    }

    return index;
}

std::vector<int64_t> general_partition( std::vector<int64_t> & arr, int64_t l, int64_t r, std::vector<int64_t> & pivots )
{
    for( const auto i : boost::irange<int64_t>( l + pivots.size()-1, r+1 ) )
    {
        const auto iter = std::find( std::begin( pivots ), std::end( pivots ), i );

        if( iter != std::end( pivots ) )
            continue;
        
        const auto new_sector = identify_new_sector( arr, pivots, arr[i] );

        // check if value already in right sector
        if( new_sector == pivots.size() )
            continue;

        int64_t new_i = i;
        for( const auto sector : boost::irange<int64_t>( new_sector, pivots.size() ) | boost::adaptors::reversed )
        {
            auto & pivot( pivots[sector] );

            if( new_i > sector+1 )
                std::swap( arr[ pivot + 1 ], arr[new_i] );

            std::swap( arr[ pivot ], arr[ pivot + 1 ] );
            
            new_i = pivot;
            pivot += 1;
        }
    }
    
    return std::move( pivots );
}

void __quicksort( std::vector< int64_t > & arr, int64_t l, int64_t r, int64_t num_pivots )
{
    if( l >= r ) return;

    if( l + 1 == r )
    {
        if( arr[r] < arr[l] )
            std::swap(arr[r], arr[l]);

        return;
    }

    //std::cout << "getting_pivots..." << std::endl;

    // if( abs(r-l) <= 10 ) {
    //     //std::cout << "r=" << r << ", l=" << l << std::endl;
    //     std::sort( std::begin( arr )+l, std::begin( arr )+r );
    //     return;
    // }

    auto pivots = get_pivot( arr, l, r, num_pivots );

    pivots = general_partition( arr, l, r, pivots );
    
    for( const auto pivot : pivots )
    {
        __quicksort(arr, l, pivot - 1, num_pivots);
        l = pivot + 1;
    }

    __quicksort(arr, pivots.back()+1, r, num_pivots);
}

void quicksort( std::vector< int64_t > & arr, int64_t num_pivots )
{
    if( arr.size() <= 1 )
        return;

    __quicksort(arr, 0, arr.size() - 1, num_pivots );
}


/* TODO:
 Problems:
 1. unknown crash on linux
 2. implementation is slow as hell
 
 Tasks:
 1. Support visual studio
 2. Test coverage - DONE
 2.1. I will set up project - DONE
 3. Fix bug on linux - DONE
 4. Improve performance( mainly for paper ) - PARTIALLY DONE
 5. Implement counting of number swaps and comparisons
 6. Vargring to check all the cache misses
 7. Maybe dynamic pivoting would be interesting
 
 Aral TODO: Tasks - [1, 4 ]
 Rustam TODO: Tasks - [2.1, 3, 4 ]
 
 */