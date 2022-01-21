make && clear

perf record -e cycles,instructions,l1d-loads,l1d-load-misses,branches,branch-misses,cache-references,cache-misses,cs,migrations,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores --call-graph=lbr ./main 
#perf stat -e cycles,instructions,l1d-loads,l1d-load-misses,branches,branch-misses,cache-references,cache-misses,page-faults,cs,migrations,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores --call-graph=lbr ./main 


valgrind --tool=callgrind --cache-sim=yes --dump-instr=yes --bransh-sim=yes ./main