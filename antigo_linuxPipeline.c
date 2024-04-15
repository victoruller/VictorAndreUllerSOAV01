
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
double time1, timedif;


int mataFilhos(int quant, pid_t listaFilhos[]){
int erro;
    for(quant -1; quant>=0; quant--){
        erro = kill(listaFilhos[quant], SIGTERM);
        if(erro < 0)return -1;
    }
return 1;
}






int main(int argc, char* argv[]){
printf("iniciando Principal\n");

int peso=0, quantidade=0, erro=0;


//Criando Pipeline---------------------------------------------------------------------------------------


    char *pipe1[2], *pipe2[2];

char * myfifo1 = "/tmp/myfifo1", * myfifo2 = "/tmp/myfifo2";

    // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(myfifo1, 0666);
    mkfifo(myfifo2, 0666);
int fd1, fd2;

//Pipe Criado--------------------------------------------------------------------------------------------

// criando processos dos scanners------------------------------------------------------------------------

pid_t scan1_pid, scan2_pid;

scan1_pid = fork();
if (scan1_pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed\n");
		return 1;
	}else if (scan1_pid == 0) { /* scanner 1 */
        execlp("./scan1comp","./scan1comp",NULL);

	}

scan2_pid = fork();
if (scan2_pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed\n");
		return 1;
	}else if (scan2_pid == 0) { /* scanner 2 */
        execlp("./scan2comp","./scan2comp",NULL);

	}



	if(scan1_pid > 0 && scan2_pid > 0){ /* parent process */

    struct timespec t;
	int secs = 2;
	clock_gettime(CLOCK_MONOTONIC ,&t);// Le a hora atual, coloca em t

	t.tv_sec++;	// aumenta t em 1 segundo
    int i=0;

        do {
        i++;
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);// Espera ateh inicio do proximo periodo


//Lendo Pipelines--------------------------------------------------------------------------------------

        // Read from FIFO
        fd1 = open(myfifo1, O_RDONLY| O_CREAT);

        if(fd1 < 0){
            printf("\nfalha ao abrir na %s\n", argv[0]);
            kill(scan1_pid, SIGTERM);
            kill(scan2_pid, SIGTERM);
            return -1;
        }

        erro = read(fd1, pipe1[0], sizeof(char));
        if(erro<0){
            printf("\nfalha ao ler pipe 1 em %s \n", argv[0]);
            kill(scan1_pid, SIGTERM);
            kill(scan2_pid, SIGTERM);
            return -1;
         }


        peso = peso + atoi(pipe1[0]) * 5;
        printf("pipe1: %s\n", pipe2[0]);
        printf("pipe1: %s\n", pipe2[0]);


        quantidade = quantidade + atoi(pipe2[0]);


        fd2 = open(myfifo2, O_RDONLY| O_CREAT);
        if(fd2 < 0){
            printf("\nfalha ao abrir na %s\n", argv[0]);
            kill(scan1_pid, SIGTERM);
            kill(scan2_pid, SIGTERM);
            return -1;
        }
        erro = read(fd2, pipe2[0], sizeof(char));
        if(erro<0){
            printf("\nfalha ao ler pipe 2 em %s \n", argv[0]);
            kill(scan1_pid, SIGTERM);
            kill(scan2_pid, SIGTERM);
            return -1;
         }

        peso = peso + atoi(pipe2[0])*2;
        quantidade = quantidade + atoi(pipe2[0]);
        printf("pipe2: %s\n", pipe2[0]);
        printf("pipe2: %s\n", pipe2[0]);


        erro = close(fd1);
        if(erro<0){
            printf("\nfalha ao fechar pipe 1 em %s \n", argv[0]);
            kill(scan1_pid, SIGTERM);
            kill(scan2_pid, SIGTERM);
                return -1;
         }
        erro = close(fd2);
        if(erro<0){
            printf("\nfalha ao fechar pipe 2 em %s \n", argv[0]);
            kill(scan1_pid, SIGTERM);
            kill(scan2_pid, SIGTERM);
            return -1;
         }


//Pipelines Lidas---------------------------------------------------------------------------------------

		//system("clear");

		/*printf("\nTotal de Itens: %d\n", quantidade);
		printf("Passaram %d quilos!\n", peso);*/

		// Calcula inicio do proximo periodo
		t.tv_sec += 2;
		/*t.tv_nsec += periodo;
		t.tv_nsec += periodo;
		t.tv_nsec += periodo;*/
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;//tira 1 segundo dos nanosegundos
			t.tv_sec++;//aumenta 1 segundo dos segundos (?)
		}
	}while(i < 5);
    kill(scan1_pid, SIGTERM);
    kill(scan2_pid, SIGTERM);
	}




time1 = (double) clock();            /* get initial time */
    time1 = time1 / CLOCKS_PER_SEC;      /*    in seconds    */

    /* call clock a second time */
    timedif = ( ((double) clock()) / CLOCKS_PER_SEC) - time1;
    printf("The elapsed time is %f seconds\n", timedif);

    printf("\nRetornou\n");
    return 0;

}
