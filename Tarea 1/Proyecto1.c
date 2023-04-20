#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define BET 10
#define EARNIGS 360
#define MIN_NUMBER_TO_GUESS 0
#define MAX_NUMBER_TO_GUESS 36

int bankBalance = 5000;
int numberToGuess;

// locker initialization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// generate a random integer
int generarRandomInteger(int min, int max) {
    srand(time(NULL)); 

    int rango = max - min + 1; 
    int numAleatorio = rand() % rango + min; 

    return numAleatorio; 
}


void * updateNumber(){

    while(1){    

            pthread_mutex_lock(&mutex);
            numberToGuess = generarRandomInteger(MIN_NUMBER_TO_GUESS,MAX_NUMBER_TO_GUESS);
            printf("El nuevo numero es %d \n",numberToGuess);

            pthread_mutex_unlock(&mutex);

            sleep(3);
    }

}

void *concrete(){

    int playerBalance = 1000;
    int guessedNumber;
    char threadName[16];
    pthread_t thread_id = pthread_self();
    pthread_getname_np(thread_id, threadName, 16);

    while(1){
        
        pthread_mutex_lock(&mutex);
        guessedNumber = generarRandomInteger(MIN_NUMBER_TO_GUESS+1,MAX_NUMBER_TO_GUESS);
        printf("el thread %s eligio %d \n",threadName,guessedNumber);
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }


}

void playConcreteNumber(){
    pthread_t players[4];
    pthread_t cropier;

    pthread_create(&cropier, NULL, updateNumber, NULL);

    for (int i = 0; i < 4; i++) {
        pthread_create(&(players[i]), NULL, concrete, NULL);
        char thread_name[17];
        sprintf(thread_name, "player->%d", i+1);
        pthread_setname_np(players[i], thread_name);
    }

    pthread_join(cropier, NULL);
    
    for (int i = 0; i < 4; i++) {
        pthread_join(players[i], NULL);
    }
}


int pickOption(){
   
   int opc;    
   printf("La banca empieza con 5000!\n");  
   printf("(1) Jugar al azar\n");
   printf("(2) Jugar al numero par/impar \n");
   printf("(3) Jugar a la martingala\n");
   scanf("%d", &opc);
   return opc;
}



int main(){
    
    playConcreteNumber();


    return 0;
}