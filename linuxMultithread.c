#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>


#define NSEC_PER_SEC    (1000000000) 	// Numero de nanosegundos em um milissegundo

int var_esteira1 = 0;
int var_esteira2 = 0;

pthread_mutex_t exclusao_mutua = PTHREAD_MUTEX_INITIALIZER;
sem_t mutex;

void *thread_sensor_1(void *param){
   int var_local = 5;
   while(1){
      pthread_mutex_lock(&exclusao_mutua);
      sem_wait(&mutex);
      var_esteira1++;
      pthread_mutex_unlock(&exclusao_mutua);
      sem_post(&mutex);
      //printf("\n1\n");
      usleep(2000000); //sleep for 00 ms
   }
}

void *thread_sensor_2(void *param){
    int var_local = 2;
   while(1){
      pthread_mutex_lock(&exclusao_mutua);
      sem_wait(&mutex);
      var_esteira2++;
      pthread_mutex_unlock(&exclusao_mutua);
      sem_post(&mutex);
      //printf("\n2\n");
      usleep(1000000); //sleep for 100 ms
   }
}



void main(){
    int peso=0, quantidade=0;

   pthread_t tid_1, tid_2;//declarar variáveis e tipos para threads
   pthread_attr_t attr;

   sem_init(&mutex, 0, 1);

   pthread_attr_init(&attr);
   pthread_create(&tid_1,&attr,thread_sensor_1,NULL);//criar threads
   pthread_create(&tid_2,&attr,thread_sensor_2,NULL);


   /*pthread_join(tid_1, NULL);//join threads
    printf("oi");
   pthread_join(tid_2, NULL);
   printf("oi");*/



    struct timespec t;
    int secs = 2;

	clock_gettime(CLOCK_MONOTONIC ,&t);// Le a hora atual, coloca em t

	t.tv_sec++;	// aumenta t em 1 segundo
    int i=0;

        do {
        i++;
		clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);// Espera ateh inicio do proximo periodo

		if(var_esteira1 + var_esteira2 > 500 + quantidade){
            quantidade +=500;
		}
		peso = (var_esteira1*5) + (var_esteira2*2);
		system("clear");
		printf("\nTotal de Itens: %d\n", quantidade);
		printf("Passaram %d quilos!\n", peso);

		// Calcula inicio do proximo periodo
		t.tv_sec += 2;
		while (t.tv_nsec >= NSEC_PER_SEC) {
			t.tv_nsec -= NSEC_PER_SEC;//tira 1 segundo dos nanosegundos
			t.tv_sec++;//aumenta 1 segundo dos segundos (?)
		}
	}while(i < 10);
	pthread_cancel(tid_1);
	pthread_cancel(tid_2);
   sem_destroy(&mutex);
}
