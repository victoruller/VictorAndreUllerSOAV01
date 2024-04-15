
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>


#define NSEC_PER_SEC    (1000000000) 	// Numero de nanosegundos em um milissegundo
double time1, timedif=0;

int main(int argc, char* argv[]){
usleep(1000000);
printf("\niniciando %s\n", argv[0]);
time1 = (double) clock();            /* get initial time */
time1 = time1 / CLOCKS_PER_SEC;      /*    in seconds    */

char * SOCK_PATH = argv[0];

///CRIANDO PIPE
int sockfd, len;
    struct sockaddr_un remote;
    char buffer[128];

    // Create socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Falha em criar o socket 1\n");
        return 1;
    }

///CONECTANDO NO PIPE
    memset(&remote, 0, sizeof(remote));
    remote.sun_family = AF_UNIX;
    strncpy(remote.sun_path, SOCK_PATH, sizeof(remote.sun_path) - 1);
    len = strlen(remote.sun_path) + sizeof(remote.sun_family);
    if (connect(sockfd, (struct sockaddr *)&remote, len) < 0)
    {
        perror("Falha em conectar no servidor 1\n");
        close(sockfd);
        return 1;
    }




	struct timespec t;

	clock_gettime(CLOCK_MONOTONIC ,&t);// Le a hora atual, coloca em t
    int i=0;
timedif = timedif + ( ((double) clock()) / CLOCKS_PER_SEC) - time1; /* call clock a second time */
        do {
time1 = (double) clock();            /* get initial time */
time1 = time1 / CLOCKS_PER_SEC;      /*    in seconds    */
        i++;
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);// Espera ateh inicio do proximo periodo


///ENVIANDO PARA O PIPE
    if (write(sockfd, "1", 1) < 0){
        perror("Falha em escrever no socket");
        close(sockfd);
        return 1;
    }

		t.tv_sec += 2;
timedif = timedif + ( ((double) clock()) / CLOCKS_PER_SEC) - time1; /* call clock a second time */
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;//tira 1 segundo dos nanosegundos
			t.tv_sec++;//aumenta 1 segundo dos segundos (?)
		}
printf("The elapsed time da esteira 1 is %f seconds\n", timedif);
	}while(1);
	/// FECHANDO PIPE
    close(sockfd);
	exit(1);
}
