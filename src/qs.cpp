#include <stdlib.h>
#include <vector>
#include <stdint.h>
#include <iostream>
//#include <boost/range.hpp>
#include <algorithm>
//#include <boost/range/irange.hpp>
//#include <boost/range/algorithm.hpp>
#include <random>
#include <qs.hpp>

void printList( const std::vector< int > & list, const char * text = "" )
{
    std::cout << text;
    
    for( int i = 0; i < list.size(); ++i )
    {
        std::cout << list[i] << " ";
    }

    printf( "\n\n" );
}

void printList( const std::vector< int > & list, const std::vector< int > & pivots, const int to_swap, const char * text = "" )
{
    std::cout << text;
    
    for( int i = 0; i < list.size(); ++i )
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

void printPivots( const std::vector< int > & arr, const std::vector< int > & pivots )
{
    std::cout << "PivotValues=";
    
    for( auto & pivot : pivots ) {
        std::cout << arr[pivot] << ",";
    }
    
    std::cout << "\n\n";
}

std::vector<int> sort_pivots( std::vector<int> & arr, int l, int r, const std::vector<int> & pivots )
{
    std::vector<int> new_pivots;
    new_pivots.reserve( pivots.size() );

    int first_pivot = l;
    for( auto p : pivots )
    {
        new_pivots.push_back( first_pivot );

        std::swap( arr[p], arr[first_pivot]);

        ++first_pivot;
    }
    
    auto iter = std::begin( arr ) + l;
    
    std::sort( iter, iter + pivots.size() );
    
    return new_pivots;
}

std::random_device rd; // obtain a random number from hardware
std::mt19937 gen(rd()); // seed the generator

std::vector< int > get_pivot( std::vector< int > & arr, int l, int r, int n )
{
    std::uniform_int_distribution<> distr(l, r); // define the range

    if( abs(r-l)+1 <= n ) {
        n = abs(r-l)+1;
    }
    
    std::vector< int > pivots;
    pivots.reserve(n);

    for( int i = 0; i < n; ++i )
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
    pivots = sort_pivots( arr, l, r, pivots );

    //printPivots( arr, pivots );
    
    return pivots;
}

std::vector< int > less_or_equal( const std::vector< int > & arr, const std::vector<int> & pivots, const int value )
{
    std::vector< int > output;
    output.reserve( pivots.size() + 1 );

    for( const auto p : pivots ) {
        output.push_back( arr[p] >= value );
    }

    output.push_back( true );
    
    return output;
}

std::vector< int > greater( const std::vector< int > & arr, const std::vector<int> & pivots, const int value )
{
    std::vector< int > output;
    output.reserve( pivots.size() + 1 );
    output.push_back( true );

    for( const auto p : pivots ) {
        output.push_back( arr[p] < value );
    }

    return output;
}


std::vector<int> general_partition( std::vector<int> & arr, int l, int r, std::vector<int> & pivots )
{
    if( abs( l-r )+1 == 1 )
    {
        return pivots;
    }
    else if( abs(l-r)+1 == 2 )
    {
        if( arr[l] > arr[r] )
        {
            std::swap( arr[l], arr[r] );
            return pivots;
        }
    }
    
    //printList(arr,"part1:");
    
    if( !(l + pivots.size() <= r) )
    {
        //std::cout << "No FOR_LOOP" << std::endl;
        //exit(-1);
        return pivots;
    }
    
    for( int i = l + pivots.size()-1; i <= r; ++i )
    {
        auto iter = std::find( std::begin( pivots ), std::end( pivots ), i );

        if( iter != std::end( pivots ) )
            continue;
        
        const auto value = arr[i];

        auto lq = less_or_equal( arr, pivots, value );
        auto gt = greater( arr, pivots, value );
        
        int value_sector = pivots.size() + 1;
        for( int j = 0; j < pivots.size(); j++ )
        {
            if( i < pivots[j] )
            {
                value_sector = j;
                break;
            }
        }

        //printList( lq );
        //printList( gt );
        //printList( arr );

        for( int k = 0; k < lq.size(); ++k )
        {
            if( !(lq[k] && gt[k]) ) continue;
                
            if( value_sector-k-1 )
            {
                ///printList( arr, pivots, i, "before_swap:" );
                

                int new_i = i;
                for( int h = pivots.size()-1; h >= k; --h )
                {
                    if( new_i == h+1 )
                    {
                        std::swap( arr[ pivots[h] ], arr[new_i] );
                        new_i = pivots[h];
                        pivots[h] += 1;
                        //printList( arr, pivots, new_i, "int_swap+:" );

                    }
                    else if( new_i > h ) {
                        std::swap( arr[ pivots[h] + 1 ], arr[new_i] );
                        std::swap( arr[ pivots[h] ], arr[ pivots[h] + 1 ] );
                        new_i = pivots[h];
                        pivots[h] += 1;
                        //printList( arr, pivots, new_i, "int_swap:" );
                    }
                    
                    if( h == k ) break;
                    
//                    else {
//                        std::swap( arr[ pivots[h] + 1 ], arr[i] );
//                        std::swap( arr[ pivots[h] ], arr[ pivots[h] + 1 ] );
//                        new_i = h;
//                        pivots[h] += 1;
//                        printList( arr, pivots, new_i, "int_swap:" );
//                    }
                }
                
                //std::swap( arr[ pivots[k] + 1 ], arr[i] );
                //std::swap( arr[ pivots[k] ], arr[ pivots[k] + 1 ] );
                
                ///std::cout << "value_sector-1=" << value_sector-1 << " k=" << k << std::endl;
                
                //for( int j = k; j < value_sector-1; ++j )
                //{
                //    pivots[j] += 1;
                //}
                
                ///printList( arr, pivots, -1, "after_swap:" );

            }
            
            break;
        }

        // int index = -1;

        // for( int k = 0; k < lq.size(); ++k ) {
        //     if( lq[k] && gt[k] ) {
        //         index = k;
        //         break;
        //     }
        // }

        // //std::cout << "Value=" << value << ", i=" << i << " sect=" << value_sector << " idx=" << index << std::endl;

        // if( index == -1 ) exit(-1);

        // if( index >= pivots.size() )
        // {
        //     auto lidx = *pivots.rbegin();
        //     if( lidx > i )
        //     {
        //         // insert after last pivot
        //         arr.erase( std::begin(arr) + i );
        //         arr.push_back( value );
        //     }
        // }
        // else {
        //     // insert before the pivot
        //     arr.erase( std::begin(arr) + i );
        //     arr.insert( std::begin( arr ) + pivots[index], value );
            
        //     if( value_sector-1-index > 0 )
        //     {
        //         i--;
        //     }
        // }

        // for( int j = index; j < value_sector-1; ++j )
        // {
        //     pivots[j] += 1;
        // }
    }
    
    return std::move( pivots );
}

void __quicksort( std::vector< int > & arr, int l, int r, int num_pivots )
{
    //std::cout << "L=" << l << " R=" << r << std::endl;
    
    if( l >= r )
        return;
    else if( l + 1 == r )
    {
        if( arr[r] < arr[l] )
            std::swap(arr[r], arr[l]);

        //printList( arr, "swap:" );
        return;
    }
    //std::cout << "getting_pivots..." << std::endl;

    auto pivots = get_pivot( arr, l, r, num_pivots );
    
    //std::cout << "getting_pivots..." << std::endl;

    pivots = general_partition( arr, l, r, pivots );
    
    for( auto pivot : pivots )
    {
        __quicksort(arr, l, pivot - 1, num_pivots);
        l = pivot + 1;
    }

    if( pivots.size() != 0 )
        __quicksort(arr, pivots.back()+1, r, num_pivots);
}

/* TODO:
 Problems:
 1. unknown crash on linux
 2. implementation is slow as hell
 
 Tasks:
 1. Support visual studio
 2. Test coverage
 2.1. I will set up project
 3. Fix bug on linux
 4. Improve performance( mainly for paper )
 5. Implement counting of number swaps and comparisons
 6. Vargring to check all the cache misses
 7. Maybe dynamic pivoting would be interesting
 
 Aral TODO: Tasks - [1, 4 ]
 Rustam TODO: Tasks - [2.1, 3, 4 ]
 
 */

void quicksort( std::vector< int > & arr, int num_pivots )
{
    if( arr.size() <= 1 )
        return;

    __quicksort(arr, 0, arr.size() - 1, num_pivots );
}