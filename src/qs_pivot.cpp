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
#include <stack>

namespace tmpl {

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

template< int Pivots >
inline std::array<int64_t, Pivots> sort_pivots( std::vector<int64_t> & arr, int64_t start, int64_t end, std::array< int64_t, Pivots > && pivots )
{
    std::sort( std::begin( pivots ), std::end( pivots ) );
   
    std::array< int64_t, Pivots > new_pivots;
    
    int64_t first_pivot = start;
    for( const auto p : pivots  | boost::adaptors::indexed(0) )
    {
        new_pivots[p.index()] = first_pivot;

        std::swap( arr[p.value()], arr[first_pivot]);

        ++first_pivot;
    }
    
    auto iter = std::begin( arr ) + start;
    
    std::sort( iter, iter + pivots.size() );
    
    return new_pivots;
}

template< int Pivots >
inline std::array< int64_t, Pivots > get_pivot( std::vector< int64_t > & arr, int64_t l, int64_t r )
{
    static std::random_device rd; // obtain a random number from hardware
    static std::mt19937 gen(rd()); // seed the generator

    std::uniform_int_distribution<> distr(l, r); // define the range
    
    std::array< int64_t, Pivots > pivots;

    for( int64_t i = 0; i < Pivots; ++i )
    {
        while( true ) {
            auto new_pivot = distr(gen);
            auto iter = std::find( std::begin( pivots ), std::end( pivots ), new_pivot );
            
            if( iter != std::end( pivots ) ) continue;
            
            pivots[i] = new_pivot;
            break;
        }
    }
    
    // sorting part
    return sort_pivots< Pivots >( arr, l, r, std::move(pivots) );
}

template< int Pivots >
inline std::array< bool, Pivots+1 > less_or_equal( const std::vector< int64_t > & arr, const std::array<int64_t, Pivots> & pivots, const int64_t value )
{
    std::array< bool, Pivots+1 > output;
    output.fill( true );

    int64_t index = 0;

    for( const auto p : pivots ) {
        const auto res = arr[p] >= value;

        if( res ) break;

        output[index++] = res;
    }
    
    return output;
}

template< int Pivots >
inline std::array< bool, Pivots+1 > greater( const std::vector< int64_t > & arr, const std::array<int64_t, Pivots> & pivots, const int64_t value )
{
    std::array< bool, Pivots+1 > output;
    output.fill( true );

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
template< int Pivots >
inline int64_t identify_new_sector( const std::vector< int64_t > & arr, const std::array<int64_t, Pivots> & pivots, const int64_t value )
{
    const auto lq = less_or_equal<Pivots>( arr, pivots, value );
    const auto gt = greater<Pivots>( arr, pivots, value );

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

template< int Pivots >
inline std::array<int64_t, Pivots> general_partition( std::vector<int64_t> & arr, int64_t l, int64_t r, std::array<int64_t, Pivots> && pivots )
{
    for( const auto i : boost::irange<int64_t>( l + pivots.size()-1, r+1 ) )
    {
        const auto iter = std::find( std::begin( pivots ), std::end( pivots ), i );

        if( iter != std::end( pivots ) )
            continue;
        
        const auto new_sector = identify_new_sector<Pivots>( arr, pivots, arr[i] );

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

struct stack_node {
    int64_t l;
    int64_t r;
};

template< int Pivots >
void quicksort( std::vector< int64_t > & arr )
{
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
   
        auto pivots = general_partition<Pivots>( arr, l, r, get_pivot< Pivots >( arr, l, r ) );

        for( const auto pivot : pivots )
        {
            stck.push( stack_node{l, pivot - 1} );

            l = pivot + 1;
        }

        stck.push( stack_node{pivots.back()+1, r} );     
    }
}

} // namespace tmpl