#include "native_qs.hpp"

/* Discontinue quicksort algorithm when partition gets below this size.
   This particular magic number was chosen to work best on a Sun 4/260. */
#define MAX_THRESH 4
/* Stack node declarations used to store unfulfilled partition obligations. */
typedef struct
  {
    int64_t *lo;
    int64_t *hi;
  } __stack_node;

/* Order size using quicksort.  This implementation incorporates
   four optimizations discussed in Sedgewick:
   1. Non-recursive, using an explicit stack of pointer that store the
      next array partition to sort.  To save time, this maximum amount
      of space required to store an array of SIZE_MAX is allocated on the
      stack.  Assuming a 32-bit (64 bit) integer for size_t, this needs
      only 32 * sizeof(__stack_node) == 256 bytes (for 64 bit: 1024 bytes).
      Pretty cheap, actually.
   2. Chose the pivot element using a median-of-three decision tree.
      This reduces the probability of selecting a bad pivot value and
      eliminates certain extraneous comparisons.
   3. Only quicksorts TOTAL_ELEMS / MAX_THRESH partitions, leaving
      insertion sort to order the MAX_THRESH items within each partition.
      This is a big win, since insertion sort is faster for small, mostly
      sorted array segments.
   4. The larger of the two sub-partitions is always pushed onto the
      stack first, with the algorithm then concentrating on the
      smaller partition.  This *guarantees* no more than log (total_elems)
      stack size is needed (actually O(1) in this case)!  */

#include <vector>
#include <stack>

template< typename T >
void my_swap( T r, T s)
{
   auto temp = *r;
   *r = *s;
   *s = temp;
}

template<typename T>
void insertionSort(std::vector<T> &vec) {
    for (auto it = std::begin(vec) + 1; it != std::end(vec); ++it) {
        auto key = it;

        for (auto i = it - 1; i >= std::begin(vec); --i) {
            if (*i > *key) {
                std::swap(*i, *key);
                key--;
            } else {
                break;
            }
        }
    }
}

template<typename Callback>
void _quicksort(std::vector<int64_t> & data, Callback && cmp )
{
    auto base_ptr = data.data();
    const int64_t size = 1;//sizeof(int64_t);
    const size_t max_thresh = MAX_THRESH * size;
    int64_t total_elems = data.size();
    
    if (total_elems == 0) return;
    
    
    if(total_elems > MAX_THRESH)
    {
      auto lo = base_ptr;
      auto hi = &lo[total_elems - 1];
        
      std::stack< __stack_node > stck;
      stck.push( __stack_node{NULL, NULL} );

      while( !stck.empty() )
        {
            int64_t *left_ptr = nullptr;
            int64_t *right_ptr = nullptr;
          /* Select median value from among LO, MID, and HI. Rearrange
             LO and HI so the three values are sorted. This lowers the
             probability of picking a pathological pivot value and
             skips a comparison for both the LEFT_PTR and RIGHT_PTR in
             the while loops. */
          auto mid = lo + ((hi - lo) / 2);
        
          // sort lo, mid and hi
          if( cmp( *mid, *lo) < 0 ) my_swap(mid, lo);
          if( cmp( *hi, *mid) < 0 ) my_swap(mid, hi);
          else
            goto jump_over;
            
          if( cmp( *mid, *lo) < 0 ) my_swap(mid, lo);
        
        jump_over:;
            
          left_ptr  = lo + size;
          right_ptr = hi - size;
          /* Here's the famous ``collapse the walls'' section of quicksort.
             Gotta like those tight inner loops!  They are the main reason
             that this algorithm runs much faster than others. */
          do
            {
              while( cmp( *left_ptr, *mid) < 0 )
                left_ptr += size;
                
              while( cmp( *mid, *right_ptr) < 0 )
                right_ptr -= size;
                
              if (left_ptr < right_ptr)
                {
                  my_swap(left_ptr, right_ptr);
                    
                  if (mid == left_ptr)
                    mid = right_ptr;
                  else if (mid == right_ptr)
                    mid = left_ptr;
                    
                  left_ptr += size;
                  right_ptr -= size;
                }
              else if (left_ptr == right_ptr)
                {
                  left_ptr += size;
                  right_ptr -= size;
                  break;
                }
            }
          while (left_ptr <= right_ptr);


          /* Set up pointers for next iteration.  First determine whether
             left and right partitions are below the threshold size.  If so,
             ignore one or both.  Otherwise, push the larger partition's
             bounds on the stack and continue sorting the smaller one. */
          if ((size_t) (right_ptr - lo) <= max_thresh)
            {
              if ((size_t) (hi - left_ptr) <= max_thresh)
              {
                  /* Ignore both small partitions. */
                  auto sn = stck.top();
                  stck.pop();
                  lo = sn.lo;
                  hi = sn.hi;
              }
              else
                /* Ignore small left partition. */
                lo = left_ptr;
            }
          else if ((size_t) (hi - left_ptr) <= max_thresh)
            /* Ignore small right partition. */
            hi = right_ptr;
          else if ((right_ptr - lo) > (hi - left_ptr))
            {
              /* Push larger left partition indices. */
              stck.push( __stack_node{lo, right_ptr} );
              lo = left_ptr;
            }
          else
            {
              /* Push larger right partition indices. */
              stck.push( __stack_node{left_ptr, hi} );
              hi = right_ptr;
            }
        }
    }
  /* Once the BASE_PTR array is partially sorted by quicksort the rest
     is completely sorted using insertion sort, since this is efficient
     for partitions below MAX_THRESH size. BASE_PTR points to the beginning
     of the array to sort, and END_PTR points at the very last element in
     the array (*not* one beyond it!). */
  {
    /* Find smallest element in first threshold and place it at the
       array's beginning.  This is the smallest array element,
       and the operation speeds up insertion sort's inner loop. */
    auto iter = std::min_element( std::begin(data), std::begin(data) + max_thresh + 1 );
      
    if( iter != std::begin(data) )
        my_swap( iter, std::begin(data) );

    /* Insertion sort, running from left-hand-side up to right-hand-side.  */
    insertionSort( data );
  }
}
