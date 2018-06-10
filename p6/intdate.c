////////////////////////////////////////////////////////////////////////////////
// Main File:        intdate.c
// This File:        intdate.c
// Other Files:      sendsig.c division.c
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

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

int global_time = 3;//an alarm will go off 3 seconds later
int count =0;//the number of SIGUSR1 caught

/*set an alarm clock for delivery of a signal
 *sig: second
 */
void handler_ALARM (int sig){
	time_t currtime;
	if (time(&currtime) == -1) {
		perror("Error time");
		exit(0);
	}
	printf("PID: %d", getpid());
	printf(" | Current time: %s", ctime(&currtime));
	if (alarm(global_time) != 0) {
		printf("Alarm error.\n");
		exit(0);
	}
}

/*to catch SIGUSR1 and print SIGUSR1 caught
 *sig:second
 */
void handler_SIGUSR (int sig) {
	printf("SIGUSR1 caught!\n");
	count++;
}

/*Typing Control+c sends an interrupt signal to the running program
 *sig:second
 */
void  handler_SIGINT (int sig) {
	printf("\nSIGINT received.\n");
	printf("SIGUSR1 was received %i times. Exiting now.\n", count);
	exit(0);
}

/*The main function exectue SIGALRM, SIGINT, SIGUSR1 signals
 *argc:the number of arguments
 *argv: use the char pointer to store each arguments
 */
int main(int argc, char *argv[]) {
	printf("Pid and time will be printed every 3 seconds.\n");
	printf("Enter ^C to end the program.\n");

	struct sigaction act;
	memset (&act, 0, sizeof(act));
	act.sa_handler = handler_ALARM;
	if (sigaction(SIGALRM, &act, NULL) < 0){
		perror("Error handling");
		exit(0);
	}
	if (alarm(global_time) != 0) {
		printf("Alarm error.\n");
		exit(0);
	}
	struct sigaction act2;
	memset (&act2, 0, sizeof(act2));
	act2.sa_handler = handler_SIGUSR;
	if (sigaction (SIGUSR1, &act2, NULL) < 0) {
		perror ("Error handling");
		exit(0);
	}

	struct sigaction act3;
	memset (&act3, 0, sizeof(act3));
	act3.sa_handler = handler_SIGINT;
	if (sigaction (SIGINT, &act3, NULL) < 0) {
		perror ("Error handling");
		exit(0);
	}

	while(1) {
	}
	return (0);
}
