/*
*	Implementa uma tarefa periodica usando clock_nanosleep em modo absoluto
*	Compilar com:	gcc -lrt -o tarefaperiodica1 tarefaperiodica1.c
*	ou
*			gcc -o tarefaperiodica1 tarefaperiodica1.c -lrt
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include <pthread.h>

#define NSEC_PER_SEC    (1000000000) 	// Numero de nanosegundos em um milissegundo


int main(int argc, char* argv[])
{
//Abertura do espaço de memória
int *ptrPeso;
int shm_fdPeso;
shm_fdPeso =  shm_open("peso", O_RDWR, 0);
if(shm_fdPeso == -1){
    printf("erro na criacao da memoria Peso em esteira1\n");
    exit(1);
}

ptrPeso = mmap(0,32, PROT_WRITE, MAP_SHARED, shm_fdPeso, 0);
if (ptrPeso == MAP_FAILED) {
		printf("Erro no mapeamento Peso em esteira1\n");
		return -1;
	}

int *ptrUnidades;
int shm_fdUnidades;
shm_fdUnidades =  shm_open("unidades", O_RDWR, 0);
if(shm_fdUnidades == -1){
    printf("erro na criacao da memoria Unidade em esteira1\n");
    exit(1);
}

ptrUnidades = mmap(0,32, PROT_WRITE, MAP_SHARED, shm_fdUnidades, 0);
if (ptrUnidades == MAP_FAILED) {
		printf("Erro no mapeamento Unidade em esteira1\n");
		return -1;
	}






	struct timespec t;

	clock_gettime(CLOCK_MONOTONIC ,&t);// Le a hora atual, coloca em t

	t.tv_sec++;	// aumenta t em 1 segundo
    int i=0;

        do {
        i++;
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);// Espera ateh inicio do proximo periodo


        pthread_mutex_t Peso = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_lock(&Peso);
        *ptrPeso+= 2;
        pthread_mutex_unlock(&Peso);


        pthread_mutex_t Unidades = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_lock(&Unidades);
        *ptrUnidades+=1;
        pthread_mutex_unlock(&Unidades);


		//printf("Passaram %d quilos!\n", *ptr);// Realiza seu trabalho

		// Calcula inicio do proximo periodo
		t.tv_sec += 1;
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
