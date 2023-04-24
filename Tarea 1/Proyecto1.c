#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#define EVEN_ODD_INSTRUCTIONS  "Apuestas par/impar\nReglas\n-Cada jugador apuesta 10 euros\n-A los ganadores se les brinda 20 euros de ganancia\n"
#define MARTINGALA_INSTRUCTIONS  "Modalidad de apuesta Matingala!\nReglas\n-Cada jugador comienza una partida con 10 euros\n -Si no hay ganador, en la siguiente partida el valor de apuesta se duplica\n-A el ganador se le incrementara su saldo en 360 euros\n"
#define CONCRETE_NUMBER_INSTRUCTIONS "Instructions for concrete number" // <- to complete 
#define BET 10
#define EARNIGS 360
#define MIN_NUMBER_TO_GUESS 0
#define MAX_NUMBER_TO_GUESS 36
#define NUM_OF_PLAYERS 4

int bankBalance = 5000;
int numberToGuess;

// locker initialization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// generate a random integer
int generateRandomInteger(int min, int max) {
    srand(time(NULL)); 

    int rango = max - min + 1; 
    int numAleatorio = rand() % rango + min; 

    return numAleatorio; 
}


//El cropier siempre utiliza esta funcion para generar el numero.
void * updateNumber(){

    while(1){    

            pthread_mutex_lock(&mutex);
            numberToGuess =  generateRandomInteger(MIN_NUMBER_TO_GUESS,MAX_NUMBER_TO_GUESS);
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
        guessedNumber = generateRandomInteger(MIN_NUMBER_TO_GUESS+1,MAX_NUMBER_TO_GUESS);
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


int play(char modeGameInstructions[], void (*modeGame)(void)) {
    int opc;
    printf(modeGameInstructions);
    printf("-1. Comenzar a jugar\n");
    printf("-2. Volver al menu principal\n");
    scanf("%d",&opc); 

    // define what to do next (?)
    (*modeGame)(); // call the game method
}



int showMainMenu(){
     int opc;
     printf("Las mejores apuestas aqui!\n");
     printf("La banca comienza con $5000!\n");
     printf("(1) Jugar al azar\n");
     printf("(2) JUgar al par/impar!\n");
     printf("(3) Jugar a la matingala!\n");
     printf("(4) Salir del programa!\n");
     printf("Elige tu opcion!\n");
     scanf("%d",&opc);
     return opc;

}

// ***** START DEFINITION OF GAMEMODES *****

void playEvenOdd(){
    pthread_t players[NUM_OF_PLAYERS];
    pthread_t cropier;
    pthread_create(&cropier, NULL, updateNumber, NULL);

}

void playMartingala(){
    pthread_t players[NUM_OF_PLAYERS];
    pthread_t cropier;
    pthread_create(&cropier, NULL, updateNumber, NULL);

}

void playConcreteNumber(){
    pthread_t players[NUM_OF_PLAYERS];
    pthread_t cropier;

    pthread_create(&cropier, NULL, updateNumber, NULL);

    for (int i = 0; i < NUM_OF_PLAYERS; i++) {
        pthread_create(&(players[i]), NULL, concrete, NULL);
        char thread_name[17];
        sprintf(thread_name, "player->%d", i+1);
        pthread_setname_np(players[i], thread_name);
    }
    
    pthread_cancel(cropier);

    pthread_join(cropier, NULL);
    
    for (int i = 0; i < NUM_OF_PLAYERS; i++) {
    	  pthread_cancel(players[i]);
        pthread_join(players[i], NULL);
    }
}

// ***** END DEFINITION OF GAMEMODES *****

void mainMenu(){
   
    int opcion = showMainMenu();

     switch(opcion){
     
     	case 1:
        play(CONCRETE_NUMBER_INSTRUCTIONS,playConcreteNumber);
     	   break;
     	case 2:
        play(EVEN_ODD_INSTRUCTIONS,playEvenOdd);
     	  break;
     	case 3:
     	   play(MARTINGALA_INSTRUCTIONS,playMartingala);
     	case 4:
     	     printf("Salir del sistema\n");
     	default:
           printf("Opción no válida\n");
        break;
   } 

}
int main(){
    
    mainMenu();


    return 0;
}
