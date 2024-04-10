#include <stdio.h>
#include<stdlib.h>
int main(){

system("gcc -o linuxMultithread linuxMultithread.c -lrt");
system("gcc -o comp lnx.c -lrt");
system("gcc -o scan1comp esteira1.c -lrt");
system("gcc -o scan2comp esteira2.c -lrt");
printf("\ncompilado\n");
system("./linuxMultithread");
}
