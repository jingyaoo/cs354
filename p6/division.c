////////////////////////////////////////////////////////////////////////////////
// Main File:        division.c
// This File:        division.c
// Other Files:      intdate.c sendsig.c
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
//////////////////////////// 80 columns wide ///////////////////////////////////

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int count;//the number of completed success

void handler_SIGFPE(int sig) {
	printf("Error: a division by 0 operation was attemped.\n");
	printf("Total number of operations completed successfully: %i\n", count);
	printf("The program will be terminated.\n");
	exit(0);
}

void  handler_SIGINT (int sig) {
	printf("\nTotal number of operations successfully completed: %i\n", count);
	printf("The program will be terminated.\n");
	exit(0);
}

int main() {
	count = 0;

	struct sigaction act;
	memset (&act, 0, sizeof(act));
	act.sa_handler = handler_SIGFPE;
	if (sigaction(SIGFPE, &act, NULL) < 0) {
		perror("Error handling");
		exit(0);
	}

	struct sigaction act2;
	memset (&act2, 0, sizeof(act2));
	act2.sa_handler = handler_SIGINT;	
	if (sigaction (SIGINT, &act2, NULL) < 0) {
		perror ("Error handling");
		exit(0);
	}

	while(1) {
		char buffer [100];
		printf("Enter first integer: ");
		fgets(buffer, 100, stdin);
		int firstNum = atoi(buffer);
		printf("Enter second integer: ");
		fgets(buffer, 100, stdin);
		int secondNum = atoi(buffer);
		int answer = firstNum / secondNum;
		int remainder = firstNum % secondNum;
		printf("%i / %i is %i with a remainder of %i\n", firstNum, secondNum, answer, remainder);
		count++;
	}
	return (0);
}
