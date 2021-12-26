
#pragma once

#include <vector>

std::vector<int64_t> sort_pivots( std::vector<int64_t> & arr, int64_t start, int64_t end, std::vector<int64_t> && pivots );
std::vector< int64_t > get_pivot( std::vector< int64_t > & arr, int64_t l, int64_t r, int64_t n );
void quicksort( std::vector< int64_t > & arr, int64_t num_pivots );