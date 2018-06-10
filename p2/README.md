Learning Goals:
  The purpose of this assignment is to get comfortable writing C programs, gaining the experience of working in a non-object oriented language. By the end of this assignment you should be comfortable with arrays, command-line arguments, file I/O, pointers, and structures in C.

Specifications:
  A magic square is a square matrix of size n x n with positive numbers from 1… n2 arranged such that the sum of the numbers in any horizontal, vertical, or diagonal (primary and secondary) line is always the same number. You can read more about it in this reference (Links to an external site.).
For this assignment you will be writing two programs, verify_magic.c and generate_magic.c to verify and generate a magic square respectively. Both the programs will require you to work with dynamically allocated 2D arrays.
The caveat in this assignment is that while accessing the 2D array of the magic square you are not allowed to use square brackets, like array[i][j] that you would normally use to index an array. Instead you are required to use pointer arithmetic and dereferences to traverse and index the 2D array. This is so that you can get comfortable with using pointers.
Using square brackets to index the 2D square will result in a deduction of points.
