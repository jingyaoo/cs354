////////////////////////////////////////////////////////////////////////////////
// Main File:        generate_magic.c verify_magic.c
// This File:        generate_magic.c
// Other Files:      verify_magic.c
// Semester:         CS 354 Spring 2017
//
// Author:           Jingyao Wei
// Email:            jwei44@wisc.edu
// CS Login:         jwei
//
/////////// IF PAIR PROGRAMMING IS ALLOWED, COMPLETE THIS SECTION //////////////
//
// Pair Partner:     (name of your pair programming partner)
// Email:            (email address of your programming partner)
// CS Login:         (partner's CS login name)
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   fully acknowledge and credit all sources of help,
//                   other than Instructors and TAs.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of 
//                   of any information you find.
////////////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>

// Structure representing Square
// size: dimension(number of rows/columns) of the square
// array: 2D array of integers
typedef struct _Square {
	int size;
	int **array;
} Square;

int get_square_size();
Square * generate_magic(int size);
void write_to_file(Square * square, char *filename);

int main(int argc, char *argv[])
{
	// Check input arguments to get filename
	if (argc != 2) {
	printf("Usage: %s <filename>\n", argv[0]);
           exit(1);
        }
	// Get size from user
	int squareSize = get_square_size();
	
	// Generate the magic square
	Square *generateSquare = generate_magic(squareSize);

	// Write the square to the output file
	write_to_file(generateSquare, argv[1]);

	for (int i = 0; i < generateSquare->size; i++) {
        	free (*(generateSquare->array + i));
    	}
    	free(generateSquare->array);
	free(generateSquare);
	return 0;
}

/* get_square_size prompts the user for the magic square size
 * checks if it is an odd number >= 3 and returns the number
 */
int get_square_size()
{
 	int size = 0;
        printf("Enter size of magic square, must be odd\n");
        scanf("%i", &size);
	if (size % 2 ==0 || size < 3) {
        	printf("Size must be an odd number >=3.\n");
             	exit(1);
        }
    	return size;
}

/* generate_magic constructs a magic square of size n
 * using the Siamese algorithm and returns the Square struct
 */
Square * generate_magic(int n)
{
	//Initialize a new Square struct of that size
	Square *newStruct;
	if ((newStruct = malloc(sizeof(Square))) == NULL) {
        	printf("Failed to allocate memory loaction.\n"); 
        	exit(1);
    	}
    	newStruct->size = n;
    	if ((newStruct->array = malloc(sizeof(int *) * n)) == NULL) {
        	printf("Failed to allocate memory loaction.\n");
        	exit(1);
    	}
    	for(int i=0; i<n; i++) {
        	if (( *(newStruct->array + i) = malloc(sizeof(int) * n)) == NULL) {
            		printf("Failed to allocate memory loaction.\n");
            		exit(1);
        	}
    	}

	//create an array
	int start = (n / 2); // the middle of column
    	int max = n * n; // the final number
    	*(*newStruct->array + start) = 1; // Place the number one in the middle of row 0

	//Loop to start placing numbers in the magic square
	int row = 0;
    	int col = start;
    	int nextRow;
    	int nextCol;
    	for (int i = 2; i < max + 1; i++) {
        	if ((row - 1) < 0) {
            	nextRow = n - 1;
        	} else {
            	nextRow = row - 1;
        	}
        	if ((col + 1) > (n - 1)) {
            	nextCol = 0;
        	} else {
            	nextCol = col + 1;
        	}
        	if(*(*(newStruct->array + nextRow) + nextCol) > 0) {
            		if (row > (n - 1)) {
                	nextRow = 0;
            	} else {
                	nextRow = row + 1;
                	nextCol = col;
            	}
        	}
        	row = nextRow;
        	col = nextCol;
        	*(*(newStruct->array + row) + col) = i;
        }
	return newStruct;
}

/* write_to_file opens up a new file(or overwrites the existing file)
 * and writes out the square in the format expected by verify_magic.c
 */
void write_to_file(Square * square, char *filename)
{
 	FILE *fp;
    	fp = fopen(filename, "w");
    	fprintf(fp, "%i\n", square->size);
    	for (int i = 0; i < square->size; i++){
        	for (int j = 0; j < square->size; j++) {
 			if (j == square->size -1) {
               			fprintf(fp, "%i", *(*(square->array + i) + j));
            		} else {
				fprintf(fp, "%i%s", *(*(square->array + i) + j), ",");        	
		}
		}
        	fprintf(fp, "\n");
    	}
    fclose(fp);
}
