#include "qs.hpp"

#include <iostream>
#include <random>
#include <vector>
#include "benchmark/benchmark.h"

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

// int main()
// {
//     std::vector< int64_t > v = getRandomList( 100 );
            
//     quicksort( v, 1 );    
// }


void foo(benchmark::State& state)
{
    std::vector< int64_t > v = getRandomList( 100 );

    while (state.KeepRunning()) {
        quicksort( v, 1 );               
        benchmark::DoNotOptimize(v);
    }
}

BENCHMARK(foo);


BENCHMARK_MAIN();