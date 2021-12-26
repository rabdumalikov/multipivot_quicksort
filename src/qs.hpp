
#pragma once

#include <vector>

std::vector<int64_t> sort_pivots( std::vector<int64_t> & arr, int64_t start, int64_t end, std::vector<int64_t> && pivots );

void quicksort( std::vector< int64_t > & arr, int64_t num_pivots );