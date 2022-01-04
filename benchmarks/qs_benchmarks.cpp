//  Copyright (c) 2019 Rustam Abdumalikov
//
//  "eswitch_v5" library
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#undef NDEBUG
#include <cassert>

#include "benchmark/benchmark.h"
#include "qs.cpp"

void greater_benchmark(benchmark::State& state)
{
    std::vector< int64_t > values = { 1, 2, 3, 4, 9, 10, 11 };
    std::vector< int64_t > pivots = { 0, 1, 2, 3, 4, 5, 6 };
        
    while (state.KeepRunning()) {        
        auto results = greater(values,pivots,0);
        
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK(greater_benchmark);

void less_or_equal_benchmark(benchmark::State& state)
{
    std::vector< int64_t > values = { 1, 2, 3, 4, 9, 10, 11 };
    std::vector< int64_t > pivots = { 0, 1, 2, 3, 4, 5, 6 };
      
    while (state.KeepRunning()) {          
        auto results = less_or_equal(values,pivots,15);
        
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK(less_or_equal_benchmark);

void get_pivot_benchmark(benchmark::State& state)
{
    std::vector< int64_t > values = { 1, 2, 3, 4, 9, 10, 11 };
       
    while (state.KeepRunning()) {
         
        auto results = get_pivot(values, 0, values.size()-1, values.size() / 2);
        
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK(get_pivot_benchmark);


void sort_pivot_benchmark(benchmark::State& state)
{
    std::vector< int64_t > values = { 11, 10, 9, 8, 7, 6, 5, 4, 3, 10, 123 };
    std::vector< int64_t > pivots = { 0, 1, 2, 3, 4, 5, 6, 7 };

    while (state.KeepRunning()) {

        auto results = sort_pivots(values, 0, values.size()-1, std::move(pivots));
        
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK(sort_pivot_benchmark);

void general_partition_benchmark(benchmark::State& state)
{
    std::vector< int64_t > values = { 5, 8, 1, 2, 3, 4, 9, 10, 11 };
    std::vector< int64_t > pivots = { 0, 1 };
        
    while (state.KeepRunning()) {
        const auto results = general_partition( values, 0, values.size()-1, pivots );
            
        benchmark::DoNotOptimize(results);
    }
}

BENCHMARK(general_partition_benchmark);


void quicksort_benchmark(benchmark::State& state)
{
    std::vector< int64_t > values;
    values.reserve( 10 );

    for( int64_t i = 11; i >= 0; --i )
        values.push_back( i );

    while (state.KeepRunning()) {
        quicksort( values, 1 );            
        benchmark::DoNotOptimize(values);
    }
}

BENCHMARK(quicksort_benchmark);


void quicksort_1_pivot_benchmark(benchmark::State& state)
{
    std::vector< int64_t > values;
    values.reserve( 100 );

    for( int64_t i = 100; i >= 0; --i )
        values.push_back( i );

    while (state.KeepRunning()) {
        quicksort( values, 1 );            
        benchmark::DoNotOptimize(values);
    }
}

BENCHMARK(quicksort_1_pivot_benchmark);

void quicksort_2_pivot_benchmark(benchmark::State& state)
{
    std::vector< int64_t > values;
    values.reserve( 100 );

    for( int64_t i = 100; i >= 0; --i )
        values.push_back( i );

    while (state.KeepRunning()) {
        quicksort( values, 2 );            
        benchmark::DoNotOptimize(values);
    }
}

BENCHMARK(quicksort_2_pivot_benchmark);

void quicksort_3_pivot_benchmark(benchmark::State& state)
{
    std::vector< int64_t > values;
    values.reserve( 100 );

    for( int64_t i = 100; i >= 0; --i )
        values.push_back( i );

    while (state.KeepRunning()) {
        quicksort( values, 3 );            
        benchmark::DoNotOptimize(values);
    }
}

BENCHMARK(quicksort_3_pivot_benchmark);

void quicksort_4_pivot_benchmark(benchmark::State& state)
{
    std::vector< int64_t > values;
    values.reserve( 100 );

    for( int64_t i = 100; i >= 0; --i )
        values.push_back( i );

    while (state.KeepRunning()) {
        quicksort( values, 4 );            
        benchmark::DoNotOptimize(values);
    }
}

BENCHMARK(quicksort_4_pivot_benchmark);

void quicksort_5_pivot_benchmark(benchmark::State& state)
{
    std::vector< int64_t > values;
    values.reserve( 100 );

    for( int64_t i = 100; i >= 0; --i )
        values.push_back( i );

    while (state.KeepRunning()) {
        quicksort( values, 5 );            
        benchmark::DoNotOptimize(values);
    }
}

BENCHMARK(quicksort_5_pivot_benchmark);



BENCHMARK_MAIN();