
#pragma once

#include <vector>

std::vector<int64_t> sort_pivots( std::vector<int64_t> & arr, int64_t start, int64_t end, std::vector<int64_t> && pivots );
std::vector< int64_t > get_pivot( std::vector< int64_t > & arr, int64_t l, int64_t r, int64_t n );
std::vector< int64_t > less_or_equal( const std::vector< int64_t > & arr, const std::vector<int64_t> & pivots, const int64_t value );
std::vector<int64_t> general_partition( std::vector<int64_t> & arr, int64_t l, int64_t r, std::vector<int64_t> & pivots );

void quicksort( std::vector< int64_t > & arr, int64_t num_pivots );