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
#include <stack>
#include "PredictableRandomGenerator.hpp"


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

int64_t Number_of_comparisons = 0;
int64_t Number_of_swaps = 0;

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
    
    Number_of_swaps += first_pivot - start;

    auto iter = std::begin( arr ) + start;
    
    std::sort( iter, iter + pivots.size() );
    
    return new_pivots;
}

static std::random_device rd; // obtain a random number from hardware
static std::mt19937 gen(rd()); // seed the generator


inline std::vector< int64_t > get_pivot( std::vector< int64_t > & arr, int64_t l, int64_t r, int64_t n )
{
    RandomNumberStream prg(n);

    //std::uniform_int_distribution<> distr(l, r); // define the range

    if( abs(r-l) < n ) {
        n = round( (abs(r-l)) / 2 );
    }
    
    std::vector< int64_t > pivots;
    pivots.reserve(n);

    for( int64_t i = 0; i < n; ++i )
    {
        while( true ) {
            auto new_pivot = prg.RandomInteger(l,r);//distr(gen);
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

    Number_of_comparisons += index;
    
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

    Number_of_comparisons += pivots.size() - index;

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
            {
                std::swap( arr[ pivot + 1 ], arr[new_i] );
                Number_of_swaps++;
            }

            std::swap( arr[ pivot ], arr[ pivot + 1 ] );
            Number_of_swaps++;
            
            new_i = pivot;
            pivot += 1;
        }
    }
    
    return std::move( pivots );
}

inline int64_t find_value_sector( const std::vector<int64_t> & pivots, int64_t value_position )
{
    auto pivot_iter = std::find_if( std::begin(pivots), std::end(pivots), [value_position]( const auto & pivot ) 
        {
            return pivot > value_position;
        } );

    return pivot_iter - std::begin(pivots) + ( pivot_iter == std::end(pivots) ? 1 : 0 );
}

inline void sort_pivots2( std::vector<int64_t> & arr, int64_t start, int64_t end, std::vector<int64_t> & pivots )
{
    std::sort( std::begin( pivots ), std::end( pivots ) );
   
    std::vector< int64_t > values;
    values.reserve( pivots.size() );

    for( const auto p : pivots ) {
        values.push_back( arr[p] );
    }

    std::sort( std::begin( values ), std::end( values ) );
    
    for( auto entry : values | boost::adaptors::indexed(0) )
    {
        arr[pivots[entry.index()]] = entry.value();
    }    
}

inline std::vector< int64_t > get_pivot2( std::vector< int64_t > & arr, int64_t l, int64_t r, int64_t n )
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
            
            auto iter2 = std::find( std::begin( pivots ), std::end( pivots ), new_pivot+1 );
            if( iter2 != std::end( pivots ) ) continue;

            auto iter3 = std::find( std::begin( pivots ), std::end( pivots ), new_pivot-1 );
            if( iter3 != std::end( pivots ) ) continue;

            pivots.push_back( new_pivot );
            break;
        }
    }
    
    // sorting part
    sort_pivots2( arr, l, r, pivots );
    
    return pivots;
}

std::vector<int64_t> general_partition2( std::vector<int64_t> & arr, int64_t l, int64_t r, std::vector<int64_t> & pivots )
{
    int64_t i = l;
    int64_t pivot_boundary = 0;
    int64_t pivot_id = 0;
    int64_t after_pivot = -1;
    int64_t prev_pivot = -1;

    for( auto & pivot : pivots ) { 
        
        const auto pivot_value = arr[pivot];
        
        while( i < pivot )
        {
            if( arr[i] > pivot_value )
            {                
                if( pivot-1 > pivot_boundary )
                {
                    std::swap( arr[pivot], arr[pivot-1] );
                    std::swap( arr[i], arr[pivot] );
                    // updating pivot
                    pivot -= 1;
                }
                else
                {
                    std::swap( arr[i], arr[pivot] );
                    // updating pivot
                    pivot -= 1;
                }
            }
            else if( pivot_id != 0 )
            {
                std::swap( arr[after_pivot++], arr[i] );
            }

            ++i;
        }

        if( pivot_id != 0 )
        {
            std::swap( arr[prev_pivot], arr[after_pivot-1] );
            pivots[pivot_id] = after_pivot-1;
        }

        prev_pivot = pivot;

        after_pivot = pivot+1;
        pivot_boundary = pivot+1;
        pivot_id += 1;        
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

    if( abs(r-l) <= 10 ) {
        std::sort( std::begin( arr )+l, std::begin( arr )+r+1 );
        return;
    }

    auto pivots = get_pivot( arr, l, r, num_pivots );

    pivots = general_partition( arr, l, r, pivots );
    
    for( const auto pivot : pivots )
    {
        __quicksort(arr, l, pivot - 1, num_pivots);
        l = pivot + 1;
    }

    __quicksort(arr, pivots.back()+1, r, num_pivots);
}

struct stack_node {
    int64_t l;
    int64_t r;
};

std::pair<int64_t,int64_t> quicksort( std::vector< int64_t > & arr, int64_t num_pivots )
{
    Number_of_comparisons = 0;
    Number_of_swaps = 0;

    std::stack< stack_node > stck;
    stck.push( stack_node{0, static_cast<int64_t>(arr.size() - 1) } );

    while( !stck.empty() ) 
    {
        auto sn = stck.top();
        stck.pop();

        int64_t l = sn.l;
        int64_t r = sn.r;
        
        if( abs(r-l) <= 10 ) {
            std::sort( std::begin( arr )+l, std::begin( arr )+r+1 );
            continue;
        }

        auto pivots = get_pivot( arr, l, r, num_pivots );    
        pivots = general_partition( arr, l, r, pivots );

        for( const auto pivot : pivots )
        {
            stck.push( stack_node{l, pivot - 1} );

            l = pivot + 1;
        }

        stck.push( stack_node{pivots.back()+1, r} );     
    }

    return std::make_pair( Number_of_comparisons, Number_of_swaps ); 
}

std::vector<int64_t> general_partition3( std::vector<int64_t> & arr, int64_t l, int64_t r, std::vector<int64_t> & pivots )
{
    int64_t current_pivot_index = 0;
    
    for( auto & pivot : pivots )
    {
        const auto pivot_value = arr[pivot];
        
        int64_t low_boundary = l;
        
        auto start_position = ( pivot-16 <= low_boundary ? low_boundary : pivot-16 );
        
        int64_t new_left_pivot_position = -1;
        
        // checking elements on left side of the pivot
        for( int64_t i = start_position; i < pivot; ++i )
        {
            if( arr[i] > pivot_value && new_left_pivot_position == -1 )
                new_left_pivot_position = i;
            else
                new_left_pivot_position = -1;
        }
        
        int64_t high_boundary = r;
        if( current_pivot_index+1 < pivots.size() )
        {
            high_boundary = pivots[current_pivot_index+1];
        }
        
        auto end_position = ( pivot+16 >= high_boundary ? high_boundary : pivot+16 );

        int64_t new_right_pivot_position = -1;
                
        // checking elements on left side of the pivot
        for( int64_t i = pivot+1; i < end_position; ++i )
        {
            if( arr[i] <= pivot_value && new_right_pivot_position == -1 )
                new_right_pivot_position = i;
            else
                new_right_pivot_position = -1;
        }
        
        if( new_left_pivot_position >= new_right_pivot_position && new_left_pivot_position != -1 )
        {
            std::swap(arr[new_left_pivot_position], arr[pivot]);
            pivot = new_left_pivot_position;
        }
        else if( new_right_pivot_position != -1 ){
            std::swap(arr[new_right_pivot_position], arr[pivot]);
            pivot = new_left_pivot_position;
        }
        
        ++current_pivot_index;
        
        printPivots( arr, pivots );
        //printList( arr, pivots );
    }
    
    return std::move( pivots );
}

// void quicksort( std::vector< int64_t > & arr, int64_t num_pivots )
// {
//     if( arr.size() <= 1 )
//         return;

//     __quicksort(arr, 0, arr.size() - 1, num_pivots );
// }


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