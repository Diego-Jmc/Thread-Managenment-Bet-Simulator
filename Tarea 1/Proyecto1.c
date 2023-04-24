[0:42 a. m., 24/4/2023] LT Villalobos: #define _GNU_SOURCE
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
            printf("El nuevo numero es %d \n",num…
[0:43 a. m., 24/4/2023] LT Villalobos: #define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define BET 10
#define EARNIGS 360
#define MIN_NUMBER_TO_GUESS 0
#define MAX_NUMBER_TO_GUESS 36

int saldoPlayer= 1000;

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


//Debido a la naturaleza de los hilos, se tendra que trabajar con variables llamadas compartidas(globales), y acceder a ellas por turnos utilizando el mutex, este nos permite acceder a una variable compartida, utilizando el metodo de turnos de hilos.

//El cropier siempre utiliza esta funcion para generar el numero.
void * updateNumber(){

    while(1){    

            pthread_mutex_lock(&mutex);
            numberToGuess =  generarRandomInteger(MIN_NUMBER_TO_GUESS,MAX_NUMBER_TO_GUESS);
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
        if(guessedNumber == numberToGuess){
           playerBalance = playerBalance+360;
           printf("el jugador %s resulto ganador \n");
        }else{
           playerBalance = playerBalance -10;
        }
        
        printf("el thread %s tiene un saldo de %d \n",threadName,playerBalance);
        pthread_mutex_unlock(&mutex);
        sleep(2);
    }


}


//en todos los modos de juego, la escogencia del numero aleatorio por parte del cropier es el mismo.
void playParImpar(){
    pthread_t players[4];
    pthread_t cropier;
    pthread_create(&cropier, NULL, updateNumber, NULL);

}

void Matingala(){
    pthread_t players[4];
    pthread_t cropier;
    pthread_create(&cropier, NULL, updateNumber, NULL);

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
    
    pthread_cancel(cropier);

    pthread_join(cropier, NULL);
    
    for (int i = 0; i < 4; i++) {
    	pthread_cancel(players[i]);
        pthread_join(players[i], NULL);
    }
}


void menuParImpar(){

int opcion;
   do{
     
     printf("Apuestas par/impar\n");
     printf("Reglas\n");
     printf("-Cada jugador apuesta 10 euros\n");
     printf("-A los ganadores se les brinda 20 euros de ganancia\n");
     printf("\n");
     printf("-1. Comenzar a jugar\n");
     printf("-2. Volver al menu principal\n");
     printf("Elige tu opcion!\n");
     scanf("%d",&opcion);
     switch(opcion){
     
     	case 1:
     	     printf("2. Jugar al par/impar\n");
     	   break;
     	case 2:
     	     menuPrincipal();
     	  break;
     	default:
              printf("Opción no válida\n");
        break;
     } 
   } while(opcion != 2);


}


void menuPlayAlzar(){


}

void menuPlayMatingala(){

int opcion;
   do{
     
     printf("Modalidad de apuesta Matingala!\n");
     printf("Reglas\n");
     printf("-Cada jugador comienza una partida con 10 euros\n");
     printf("-Si no hay ganador, en la siguiente partida el valor de apuesta se duplica\n");
     printf("-A el ganador se le incrementara su saldo en 360 euros\n");
     printf("\n");
     printf("-Elige tu opcion\n");
     printf("-1. Comenzar a jugar\n");
     printf("-2. Volver al menu principal\n");
     printf("Elige tu opcion!\n");
     scanf("%d",&opcion);
     switch(opcion){
     
     	case 1:
     	     printf("2. Jugar al par/impar\n");
     	   break;
     	case 2:
     	     menuPrincipal();
     	  break;
     	default:
              printf("Opción no válida\n");
        break;
     } 
   } while(opcion != 2);


}


//Menus...
void menuPrincipal(){
   
   int opcion;
   do{
     printf("Las mejores apuestas aqui!\n");
     printf("La banca comienza con $5000!\n");
     printf("(1) Jugar al azar\n");
     printf("(2) JUgar al par/impar!\n");
     printf("(3) Jugar a la matingala!\n");
     printf("(4) Salir del programa!\n");
     printf("Elige tu opcion!\n");
     scanf("%d",&opcion);
     switch(opcion){
     
     	case 1:
     	     playConcreteNumber();
     	   break;
     	case 2:
     	     menuParImpar();
     	  break;
     	case 3:
     	     menuPlayMatingala();
     	case 4:
     	     printf("Salir del sistema\n");
     	default:
              printf("Opción no válida\n");
        break;
     } 
   } while(opcion != 4);
}



int main(){
    
    menuPrincipal();


    return 0;
}
