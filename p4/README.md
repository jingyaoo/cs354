Learning Goals:

This assignment has 2 parts. The purpose of the first part is to understand more about how caches work and learn a bit about simulation along the way. In second part of the assignment, you will work on developing a small cache simulator. This will make you an expert in the basics of caches and further strengthen your C programming skills.

Part 1 - cache analysis with pin:

For this part, you will use a dynamic binary instrumentation framework called pin to measure cache performance statistics of an executable program by specifying certain cache parameters. Specifically you will use pin to run simulations and answer questions in the file "p4questions.txt".

Part 2 - cache simulator csim:

You will write a cache simulator in "csim.c" that takes a valgrind memory trace as input, simulates the hit/miss/eviction behavior of a cache memory on this trace, and outputs the total number of hits, misses and evictions.
