#include "qs.hpp"
#include "qs_pivot.cpp"

#include <iostream>
#include <random>
#include <vector>
#include "benchmark/benchmark.h"
#include <chrono>
#include <ctime>

#include "PredictableRandomGenerator.hpp"

static std::random_device r; // obtain a random number from hardware
static std::seed_seq seed{time(NULL)}; // 2
static std::mt19937 gen(seed); // seed the generator

//static std::default_random_engine gen{static_cast<long unsigned int>(time(0))};



std::vector<int64_t> getRandomList( int64_t n )
{
    std::vector<int64_t> output;
    output.reserve( n );

    auto rns = RandomNumberStream(n);
    for( int i = 0; i < n; ++i ) {
        output.push_back( rns.RandomInteger(0, n) );
    }

    // std::uniform_int_distribution<> distr(0, n*n); // define the range


    // for(int64_t i=0; i<n; ++i)
    //     output.push_back( distr(gen) );

    return output;
}

int compare_ints(const void* a, const void* b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
}

int foo1()
{
    //std::vector< int64_t > v = getRandomList( 100000 );
            
    double total_duration = 0.0;
    int64_t num_durations = 0;
    //constexpr int Index = 1;

    for( int64_t np = 1; np < 60; ++np )
    {
        std::pair< int64_t, int64_t > stats;

        for( int64_t i = 1; i < 90; ++i ) 
        {
            std::vector< int64_t > v = getRandomList( 100000 );

            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

            stats = quicksort( v, np );

            //tmpl::quicksort<Index>( v );
            
            //std::qsort(v.data(), v.size(), sizeof(int64_t),compare_ints);           
    
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            
            total_duration +=  std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
            
            ++num_durations;

            //std::cout <<  "IsSorted:" << std::is_sorted( std::begin( v ), std::end( v ) ) << ',';

        }
        
        std::cout << "[" << np << "] Time difference = " << total_duration/num_durations << "[ms]" << " #Cmps=" << stats.first << " #Swaps=" << stats.second << std::endl;
    }
        
    // std::cout <<  "IsSorted:" << std::is_sorted( std::begin( v ), std::end( v ) ) << std::endl;

    // for( int64_t i = 0; i < 10; ++i )
    //     std::cout << v[i] << std::endl;
    return 1;
}

int foo2()
{
    std::vector< int64_t > v = getRandomList( 100000 );

    const int64_t num_of_pivots = 15;

    //for( int i = 0; i < 10; ++i ) std::cout << v[i] << "," << std::endl;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    auto stats = quicksort( v, num_of_pivots );    
    
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            
    const auto total_duration =  std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
            
    std::cout << "[" << num_of_pivots << "] Time difference = " << total_duration << "[ms]" << std::endl;
    std::cout << "#Cmps=" << stats.first << " #Swaps=" << stats.second << std::endl;

    return 1;
}


int main()
{
    return foo2();
}

// void foo(benchmark::State& state)
// {
//     std::pair< int64_t, int64_t > stats;
//     while (state.KeepRunning()) {
//         std::vector< int64_t > v = getRandomList( 10000 );

//         //tmpl::quicksort<1>( v ); 
//         stats = quicksort( v, 3 );
         
//        // std::qsort(v.data(), v.size(), sizeof(int64_t),compare_ints);           
          
//         benchmark::DoNotOptimize(v);
//     }

//     std::cout << "#Cmps=" << stats.first << " #Swaps=" << stats.second << std::endl;
// }

// BENCHMARK(foo);

// BENCHMARK_MAIN();