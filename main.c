#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "wait.h"
#include <sys/types.h>

/**
 * This program forks a separate process using the fork()/exec() system calls.
 *
 *
 */

int main(){

    printf("oi victor, ");

	pid_t pid, pid1, pid2;

	/* fork a child process */

	pid = fork();


	if (pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed\n");
		return 1;
	}
	else if (pid == 0) { /* child process */
		printf("I am the child 1 - %d\n",pid);

	}
	else { /* parent process */
		/* parent will wait for the child to complete */
		printf("I am the parent %d\n",pid);



		pid1 = fork();
		pid2 = fork();
		wait(NULL);
		printf("\n\n%d - %d - %d\n",pid,pid1,pid2);



		/*
		if(pid1 == 0){
            printf("I am the child 2");
            if (pid > 0){
                 printf("and i am son of the child 1\n");
            }

		}

		if(pid2 == 0){
            printf("I am the child 3 ");
            if(pid1 == 0){
                    printf("and I am son of child 2 - ");
                    if(pid == 0){
                        printf("that is son of the child 1\n");
                    }else if(pid > 0){
                        printf("that is son of the parent\n");
                    }
            } else if(pid1 > 0){
                 printf("but i am son of the parent\n");
            }

            }

		printf("Child Complete\n");
		*/

	}
	printf("\n\n%d - %d - %d\n",pid,pid1,pid2);
    printf("\n\n\n");
    return 0;
}
