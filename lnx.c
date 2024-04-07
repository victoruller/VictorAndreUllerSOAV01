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
XX  criar esteira
XX  chamar esteiras no lnx
XX  mostrar valores no telão
XX(não sei se faz sendito fazer)    separar esteiras de scanner, criando um sistema de mensagem entre elas
XX  implantar mutex ou semáforo
XX  adicionar segunda esteira
inserir o tal do pipe





codigo em lunux com pipe
codigo em linux com multithread
codigo em windows com pipe
codigo em windows com multithreads

disponibilizar no github
criar relatorio


*/

double time1, timedif;


int main(){

//Criacao da memoria compartilhada------------------------------------------------------------------------
const int size = 4*8;
int *ptrPeso;
int shm_fdPeso;
shm_fdPeso =  shm_open("peso", O_CREAT|O_RDWR, 0666);
if(shm_fdPeso == -1){
    printf("erro na criacao da memoria Peso em lnx\n");
    exit(1);
}
if(-1 == ftruncate(shm_fdPeso, size)){
    printf("erro no tamanho da memoria Peso em lnx\n");

};
ptrPeso = mmap(0,size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fdPeso, 0);
if (ptrPeso == MAP_FAILED) {
		printf("Erro no mapeamento Peso em lnx\n");
		return -1;
	}


int *ptrUnidades;
int shm_fdUnidades;
shm_fdUnidades =  shm_open("unidades", O_CREAT|O_RDWR, 0666);
if(shm_fdUnidades == -1){
    printf("erro na criacao da memoria Unidade em lnx\n");
    exit(1);
}
if(-1 == ftruncate(shm_fdUnidades, size)){
    printf("erro no tamanho da memoria Unidade em lnx\n");

};
ptrUnidades = mmap(0,size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fdUnidades, 0);
if (ptrUnidades == MAP_FAILED) {
		printf("Erro no mapeamento Unidade em lnx\n");
		return -1;
	}



//Memoria Criada

// criando processos dos scanners------------------------------------------------------------------------


pid_t scan1_pid, scan2_pid;

scan1_pid = fork();
scan2_pid = fork();

//wait(NULL);

if (scan1_pid < 0 || scan2_pid < 0) { /* error occurred */
		fprintf(stderr, "Fork Failed\n");
		return 1;
	}
	else if (scan1_pid == 0) { /* scanner 1 */
        execlp("./scan1comp","./scan1comp",NULL);

	}
	else if(scan2_pid == 0) { /* scanner2 */
        execlp("./scan2comp","./scan2comp",NULL);

	}
	else { /* parent process */
		/* parent will wait for the child to complete */
		struct timespec t;
	int secs = 2;

	clock_gettime(CLOCK_MONOTONIC ,&t);// Le a hora atual, coloca em t

	t.tv_sec++;	// aumenta t em 1 segundo
    int i=0;

        do {
        i++;
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);// Espera ateh inicio do proximo periodo
		system("cls");
		printf("\nTotal de Itens: %d\n", *ptrUnidades);
		printf("Passaram %d quilos!\n", *ptrPeso);

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
    printf("\ndando Exit\n");

}
