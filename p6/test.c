#include <unistd.h>
#include <stdio.h>

int main() {
	pid_t pid;
	if ((pid = fork()) == 0) {
		for (int i = 0; i < 100; i++) {
			printf("a");
			fflush(stdout);

		}
	} else {
		for (int i = 0; i < 100; i++) {
			printf("b");
			fflush(stdout);
		}
	}
	return 0;
}
