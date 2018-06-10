////////////////////////////////////////////////////////////////////////////////
// Main File:        verify_magic.c generate_magic.c
// This File:        verify_magic.c
// Other Files:      generate_magic.c
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
#include <string.h>

// Structure representing Square
// size: dimension(number of rows/columns) of the square
// array: 2D array of integers
typedef struct _Square {
	int size;
	int **array;
} Square;

Square * construct_square(char *filename);
int verify_magic(Square * square);

int main(int argc, char *argv[])
{

	// Check input arguments to get filename
	if (argc != 2) {
		printf("Usage: %s <filename>\n", argv[0]);
       		exit(1);
   	}	
    
	// Construct square
	Square * newSquare = construct_square(argv[1]);

	// Verify if it's a magic square and print true or false
	if (verify_magic(newSquare) == 1) {
        	printf("%s\n", "true");
    	} else {
        	printf("%s\n", "false");
    	}
 	for (int i = 0; i < newSquare->size; i++) {
            	free (*(newSquare->array + i));
        }
    	free(newSquare->array);
	free(newSquare);
	return 0;
}

/* construct_square reads the input file to initialize a square struct
 * from the contents of the file and returns the square.
 * The format of the file is defined in the assignment specifications
 */
Square * construct_square(char *filename)
{

	// Open and read the file
  	FILE* fp = fopen(filename, "r");
    	if (fp == NULL) {
        	printf("Cannot open file for reading.\n");
        	exit(1);
    	}
	// Read the first line to get the square size
  	char str [100];
    	int sizeSquare = 0;
    	if (fgets(str, sizeof(str), fp) != NULL) {
        	sizeSquare = atoi(str);
    	}

	// Initialize a new Square struct of that size
   	Square *newStruct;
    	if ((newStruct = malloc(sizeof(Square))) == NULL) {
        	printf("Failed to allocate memory loaction.\n");
        	exit(1);
    	}
    	newStruct->size = sizeSquare;
    	if ((newStruct->array = malloc(sizeof(int *) * sizeSquare)) == NULL) {
        	printf("Failed to allocate memory loaction.\n");
        	exit(1);
    	}
    	for(int i= 0; i < sizeSquare; i++) {
        	if (( *(newStruct->array + i) = malloc(sizeof(int) * sizeSquare)) == NULL){
            		printf("Failed to allocate memory loaction.\n");
            		exit(1);
        	}
    	}

	// Read the rest of the file to fill up the square
 	char *eachline;
    	if ((eachline = malloc(sizeof(str))) == NULL) {
        	printf("Failed to allocate memory loaction.\n");
        	exit(1);
    	}
   	for (int i = 0; i < sizeSquare; i++) {
    		if(fgets(str,sizeof(str), fp) != NULL) {
        		eachline = strtok (str, ",");
        	if (( *(*(newStruct->array + i)+0) = atoi(eachline)) == 0) {
            printf("No valid conversion");
            exit(1);
        }
        	for (int j = 1; j < sizeSquare; j++) {
           		eachline = strtok (NULL, ",");
            		if(eachline != NULL) {
            		 if ((*(*(newStruct->array+i)+j) = atoi(eachline)) == 0) {
                    printf("No valid conversion");
                    exit(1);
                }
            		}	
                }
    		}
    	}
    
        fclose(fp);
    	return newStruct;
}

/* verify_magic verifies if the square is a magic square
 * 
 * returns 1(true) or 0(false)
 */
int verify_magic(Square * square)
{
	int flag = 0;
	int target = 0;
	// sum the first line
 	for(int i = 0; i < square -> size ; i++) {
 		target += *(*(square->array) + i);
	}

	// Check all rows sum to same number
 	int sum1 = 0;
    	for (int i = 0; i < square -> size; i++) {
        for (int j = 0; j < square -> size; j++) {          
            sum1 += *(*(square->array + i) + j);
        }
        if (target == sum1) {
            flag = 1;
	    sum1 = 0;
        }else {
            flag = 0;
            break;
        }
    	}

	// Check all cols sum to same number
	int sum2 = 0; 
    	for (int i = 0; i < square -> size; i++) {
        for (int j = 0; j < square -> size; j++) {
            sum2 += *(*(square->array + i) + j);
        }
        if (target == sum2) {
            flag = 1;
	    sum2 = 0;
        } else {
            flag = 0;
            break;
        }
    	}

	// Check main diagonal
 	int sum3 = 0;
    	for (int i = 0; i < square -> size; i++) {
            sum3 += *(*(square->array + i) + i);
        }
        if (target == sum3) {
            flag = 1;
        } else {
            flag = 0;
    	}

	// Check secondary diagonal
   	int sum4 = 0;
    	for (int i = 0; i < square -> size; i++) {
            sum4 += *(*(square->array + i) + (square->size - 1 - i));
     	}   
        if ( target== sum4) {
            flag = 1;
        } else {
            flag = 0;
    	}
	return flag;
}
