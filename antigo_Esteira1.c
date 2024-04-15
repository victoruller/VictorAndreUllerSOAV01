/*
*	Implementa uma tarefa periodica usando clock_nanosleep em modo absoluto
*	Compilar com:	gcc -lrt -o tarefaperiodica1 tarefaperiodica1.c
*	ou
*			gcc -o tarefaperiodica1 tarefaperiodica1.c -lrt
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>


#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>


#include <pthread.h>
#define NSEC_PER_SEC    (1000000000) 	// Numero de nanosegundos em um milissegundo


int main(int argc, char* argv[]){

printf("iniciando %s\n", argv[0]);

int fd, erro;
char *x = "1";
char *myfifo1 = "/tmp/myfifo1";
/*
printf("\nestah iniciando esteira 1 e o argumento é ");
printf("%s\n", argv[0]);*/



	struct timespec t;

	clock_gettime(CLOCK_MONOTONIC ,&t);// Le a hora atual, coloca em t
    int i=0;

        do {
        i++;
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);// Espera ateh inicio do proximo periodo


///COLOCAR A COMUNICAÇÂO PIPE AQUI

        fd = open(myfifo1, O_WRONLY);
        if(fd < 0){
            printf("falha ao abrir na %s", argv[0]);
            return -1;
        }

        erro = write(fd, x, sizeof(char));
        if(erro<0){
                printf("\nfalha ao escrever em %s \n", argv[0]);
                return -1;
        }

        erro = close(fd);
        if(erro<0){
                printf("\nfalha ao fechar em %s \n", argv[0]);
                return -1;
        }



		//printf("Passaram %d quilos!\n", pipe[0]);// Realiza seu trabalho

		// Calcula inicio do proximo periodo
		t.tv_sec += 2;
		/*t.tv_nsec += periodo;
		t.tv_nsec += periodo;
		t.tv_nsec += periodo;*/
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;//tira 1 segundo dos nanosegundos
			t.tv_sec++;//aumenta 1 segundo dos segundos (?)
		}
	}while(1);
	exit(1);
}
