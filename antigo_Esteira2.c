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


int main(int argc, char* argv[])
{
printf("iniciando %s\n", argv[0]);
int fd, erro;
char *x = "1";
char *myfifo2 = "/tmp/myfifo2";
/*printf("\nestá iniciando esteira 2 e o argumento é ");
printf("%s%p\n", argv[0], argv[1]);*/



	struct timespec t;

	clock_gettime(CLOCK_MONOTONIC ,&t);// Le a hora atual, coloca em t

	t.tv_sec++;	// aumenta t em 1 segundo
    int i=0;

        do {
        i++;
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);// Espera ateh inicio do proximo periodo

///COLOCAR A COMUNICAÇÂO PIPE AQUI


        fd = open(myfifo2, O_WRONLY);
        if(fd < 0){
            printf("falha em abrir na %s", argv[0]);
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
                exit (-1);
                return -1;
        }


		t.tv_sec += 1;
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;//tira 1 segundo dos nanosegundos
			t.tv_sec++;//aumenta 1 segundo dos segundos (?)
		}
	}while(1);
	exit(1);
}
