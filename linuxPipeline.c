
// no WSL cd /mnt/c/victorTrabalhos/C/VictorAndreUllerSOAV01
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>

#define SOCK_PATH "/tmp/pipeso"
#define SOCK_PATH2 "/tmp/pipeso2"
#define NSEC_PER_SEC    (1000000000)
/*
codigo em lunux com pipe
XXcodigo em linux com multithread
codigo em windows com pipe
codigo em windows com multithreads

disponibilizar no github
criar relatorio


*/

//variaveis globais
double time1, timedif=0;



int main(int argc, char* argv[]){
printf("iniciando Principal\n");


time1 = (double) clock();            /* get initial time */
time1 = time1 / CLOCKS_PER_SEC;      /*    in seconds    */

int peso=0, quantidade=0, qtd=0, erro=0;


///Criando Pipeline

    int sockfd, newsockfd, len;///pipe1
    struct sockaddr_un local, remote;
    char buffer[1024]={0};

    // Create socket
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Falha em criar o pipe 1\n");
        return 1;
    }

    // Bind socket to local address
    memset(&local, 0, sizeof(local));
    local.sun_family = AF_UNIX;
    strncpy(local.sun_path, SOCK_PATH, sizeof(local.sun_path) - 1);
    unlink(local.sun_path);
    len = strlen(local.sun_path) + sizeof(local.sun_family);
    if (bind(sockfd, (struct sockaddr *)&local, len) < 0)
    {
        perror("Falha em capturar o socket 1\n");
        close(sockfd);
        return 1;
    }

int sockfd2, newsockfd2, len2;///pipe 2
    struct sockaddr_un local2, remote2;
    char buffer2[1024]={0};

    // Create socket
    sockfd2 = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd2 < 0)
    {
        perror("Falha em criar o pipe 2\n");
        return 1;
    }

    // Bind socket to local address
    memset(&local2, 0, sizeof(local2));
    local2.sun_family = AF_UNIX;
    strncpy(local2.sun_path, SOCK_PATH2, sizeof(local2.sun_path) - 1);
    unlink(local2.sun_path);
    len2 = strlen(local2.sun_path) + sizeof(local2.sun_family);
    if (bind(sockfd2, (struct sockaddr *)&local2, len2) < 0)
    {
        perror("Falha em capturar o socket 2\n");
        close(sockfd2);
        return 1;
    }

//Pipe Criado--------------------------------------------------------------------------------------------

// criando processos dos scanners------------------------------------------------------------------------

pid_t scan1_pid, scan2_pid;

scan1_pid = fork();
if (scan1_pid < 0) { /* error occurred */
		fprintf(stderr, "Fork 1 Failed\n");
		return 1;
	}else if (scan1_pid == 0) { /* scanner 1 */
        execlp("./scan1comp","/tmp/pipeso",NULL);

	}

scan2_pid = fork();
if (scan2_pid < 0) {
		fprintf(stderr, "Fork 2 Failed\n");
		return 1;
	}else if (scan2_pid == 0) {
        execlp("./scan2comp","/tmp/pipeso2",NULL);

	}


	if(scan1_pid > 0 && scan2_pid > 0){ /* parent process */

    struct timespec t;
	clock_gettime(CLOCK_MONOTONIC ,&t);// Le a hora atual, coloca em t

	t.tv_sec++;	// aumenta t em 1 segundo
    int i=0;


if (listen(sockfd, 5) < 0)    {
        perror("Falha em escutar o socket 1\n");
        close(sockfd);
        return 1;
    }

    printf("Servidor Named pipe 1 ouvindo em %s...\n", SOCK_PATH);


    // Accept connections
    memset(&remote, 0, sizeof(remote));
    len = sizeof(remote);
    newsockfd = accept(sockfd, (struct sockaddr *)&remote, &len);
    if (newsockfd < 0)
    {
        perror("Falha em aceitar coneccao 1\n");
        close(sockfd);
        return 1;
    }


     ///Conectando 2
     if (listen(sockfd2, 5) < 0)
    {
        perror("Falha em escutar o socket 2\n");
        close(sockfd2);
        return 1;
    }

    printf("Servidor Named pipe 2 ouvindo em %s...\n", SOCK_PATH2);


    // Accept connections
    memset(&remote2, 0, sizeof(remote2));
    len2 = sizeof(remote2);
    newsockfd2 = accept(sockfd2, (struct sockaddr *)&remote2, &len2);
    if (newsockfd2 < 0)
    {
        perror("Falha em aceitar coneccao 2\n");
        close(sockfd2);
        return 1;
    }



timedif = timedif + ( ((double) clock()) / CLOCKS_PER_SEC) - time1; /* call clock a second time */
do {///Lendo Pipelines--------------------------------------------------------------------------------------
time1 = (double) clock();
time1 = time1 / CLOCKS_PER_SEC;
        i++;
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);// Espera ateh inicio do proximo periodo


    // Read data from client 1
    if (read(newsockfd, buffer, sizeof(buffer)) < 0)
    {
        perror("\nFalha em ler do socket 1\n");
        close(newsockfd);
        close(sockfd);
        return 1;
    }


    // Read data from client 2
    if (read(newsockfd2, buffer2, sizeof(buffer2)) < 0)
    {
        perror("\nFalha em ler do socket 2\n");
        close(newsockfd2);
        close(sockfd2);
        return 1;
    }


//Pipelines Lidas---------------------------------------------------------------------------------------


    for(int i=0;i<15; i++){
                if(buffer[i] == '1'){
                    peso = peso + 5;
                    quantidade = quantidade + 1;
                }
            }
            printf("\n");
            for(int i=0;i<15; i++){
                if(buffer2[i] == '2'){
                    peso = peso + 2;
                    quantidade = quantidade + 1;
                }
            }
            if(quantidade >= (qtd+500)){
                    qtd = qtd + 500;
            }
            system("clear");
            printf("\nQuantidade de Itens: %d", qtd);
            printf("\npeso = %d\n", peso);
printf("The elapsed time is %f seconds\n", timedif);


		// Calcula inicio do proximo periodo
timedif = timedif + ( ((double) clock()) / CLOCKS_PER_SEC) - time1; /* call clock a second time */


		t.tv_sec += 2;
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;//tira 1 segundo dos nanosegundos
			t.tv_sec++;//aumenta 1 segundo dos segundos (?)

		}

	}while(i < 180);
	printf("teminando");
	usleep(2000000);
    kill(scan1_pid, SIGTERM);
    kill(scan2_pid, SIGTERM);
    close(newsockfd);
    close(sockfd);
	}



printf("The elapsed time is %f seconds\n", timedif);

printf("\nRetornou\n");
return 0;

}
