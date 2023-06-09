#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <sys/prctl.h>
#define EVEN_ODD_INSTRUCTIONS  "Apuestas par/impar\nReglas\n-Cada jugador apuesta 10 euros\n-A los ganadores se les brinda 20 euros de ganancia\n"
#define MARTINGALA_INSTRUCTIONS  "Modalidad de apuesta Matingala!\n Reglas \n-Cada jugador comienza una partida con 10 euros\n -Si no hay ganador, en la siguiente partida el valor de apuesta se duplica\n -A el ganador se le incrementara su saldo en 360 euros\n"
#define CONCRETE_NUMBER_INSTRUCTIONS "Jugar a numero concreto\n cada jugador apuesta 10 euras y gana 360 si atina \n Si el jugador se queda sin dinero pierde automaticamente\n"
#define BET 10
#define EARNIGS 360
#define MIN_NUMBER_TO_GUESS 0
#define MAX_NUMBER_TO_GUESS 36
#define NUM_OF_PLAYERS 4
int bankBalance = 50000;
int numberToGuess;
int playerBalance = 1000;
int guessedNumber = 0;
int finalizado = 0;

/*

    Tarea 1 Sistemas operativos
        **INTEGRANTES**
   Diego Jiménez Chamorro 
   Dylan Molina Obando 
   Juan Daniel Zuñiga Hernadez 
   Luis Torres Villalobos

*/


// locker initialization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// generate a random integer
int generateRandomInteger(int min, int max) {
    return rand() % (max - min +1)+min;  
}


void * updateNumber(){
	    
    while(1){
        pthread_mutex_lock(&mutex); //closed
        printf("-----BANCA ACTUAL DEL CASINO: €-----%d \n",bankBalance);
        
        numberToGuess =  generateRandomInteger(MIN_NUMBER_TO_GUESS,10);

        if(numberToGuess == 0){
            printf("EL NUMERO ELEGIDO FUE 0 , LA BANCA SE QUEDA CON TODO \n");
            bankBalance = 50000;
            pthread_mutex_unlock(&mutex); //reopened
            pthread_exit(NULL); // termina el hilo
        }
        
        else if(finalizado == 4){
            printf("Todos los jugadores se han quedado sin dinero, partida terminada\n");
            
            finalizado = 0;
            pthread_mutex_unlock(&mutex); //reopened
            pthread_exit(NULL); // termina el hilo
            system("cls");
        }

        printf("--- EL NUEVO NUMERO ESCOGIDO POR EL CROPIER ES-> %d \n",numberToGuess);
        pthread_mutex_unlock(&mutex); //reopened
        
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
        

        if(playerBalance>=BET){
            playerBalance-=BET;
            bankBalance+=BET;
        }else{
              printf("el thread %s se quedo sin dinero \n",threadName);
              break;
        }

        if(numberToGuess == 0){
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
            
            break;
        }
        
        guessedNumber = generateRandomInteger(1,10);
        printf("el thread %s eligio %d y tiene un saldo de-> %d \n",threadName,guessedNumber,playerBalance);

        if(guessedNumber == numberToGuess){
           playerBalance +=EARNIGS;
           bankBalance-=EARNIGS;        
                    printf("el thread %s adivino el numero y tiene un saldo de-> %d \n",threadName,playerBalance);

        }     
    
        pthread_mutex_unlock(&mutex);
        usleep(3100000);
        }

}

void *evenOdd(){

    int guessedNumber,even;
    int playerBalance = 1000;
    char threadName[16];
    pthread_t thread_id = pthread_self();
    pthread_getname_np(thread_id, threadName, 16);
    
    
    while(1){
    pthread_mutex_lock(&mutex);
    guessedNumber =  generateRandomInteger(1,3);
    
     if(playerBalance>=BET){
            playerBalance-=BET;
            bankBalance+=BET;
      }else{
              printf("el thread %s se quedo sin dinero \n",threadName);
              break;
     }

        if(numberToGuess == 0){
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
            
            break;
        }
        

    //al azar el sistema le dice al jugador si va a jugar par o impar.
    if(guessedNumber % 2 == 0){
        printf("el jugador %s ha escogido par \n",threadName);
        even = 1;
    }
    else{
       printf("el jugador %s ha escogido impar\n",threadName);
       even = 0;
    }

    // check if the chosen number was even or odd . if then the players wins
    if(even && numberToGuess %2 == 0 || !even && numberToGuess%2 != 0){
        playerBalance = playerBalance+20;       
        bankBalance = bankBalance-BET;
        printf("el jugador %s ha GANADO,tiene un saldo de -> %d\n",threadName,playerBalance);
    }
    else
     printf("el jugador %s ha PERDIDO,tiene un saldo de -> %d\n",threadName,playerBalance);
     
        pthread_mutex_unlock(&mutex);
        usleep(3100000);
      
    }
    
}

void *martingala(){
    int playerBalance = 1000,apuesta = 10,guessedNumber;
    char threadName[16];
    pthread_t thread_id = pthread_self();
    pthread_getname_np(thread_id, threadName, 16);
        while(1){
        pthread_mutex_lock(&mutex);
          
        if(numberToGuess == 0){
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL);
            break;
        }
               
        if(playerBalance < BET || playerBalance < apuesta ){
          pthread_mutex_unlock(&mutex);
            printf("el jugador %s se quedo sin dinero \n",threadName);
            finalizado = finalizado+1;
            pthread_exit(NULL);
            break;
        
        }else{
        
        guessedNumber = generateRandomInteger(1,10);
        printf("el jugador %s eligio %d y tiene un saldo de-> %d \n",threadName,guessedNumber,playerBalance);
        if(guessedNumber == numberToGuess){
           playerBalance = playerBalance+EARNIGS;
            printf("el jugador %s eligio %d, adivino el numero y tiene un saldo de-> %d \n",threadName, guessedNumber,playerBalance);
        }else{
            playerBalance = playerBalance -apuesta;
            bankBalance = bankBalance+apuesta;
            apuesta = apuesta*2;
        }
             
        pthread_mutex_unlock(&mutex);
        usleep(3100000);
        }
    }
    
   }

int showMainMenu(){
     int opc;
     printf("Las mejores apuestas aqui!\n");
     printf("La banca comienza con $5000!\n");
     printf("(1) Jugar al azar\n");
     printf("(2) Jugar al par/impar!\n");
     printf("(3) Jugar a la matingala!\n");
     printf("(4) Salir del programa!\n");
     printf("Elige tu opcion!\n");
     scanf("%d",&opc);
     return opc;
 
}

void setTimer(){
    int count = 3;

    while(count>=1){
        printf("*** %d *** \n",count);
        sleep(1);
        count--;
    }

}

void * startGame(void *(*game)(void *), char gameInstructions[]){
    printf("%s",gameInstructions);
    setTimer();
    pthread_t players[NUM_OF_PLAYERS];
    pthread_t cropier;
    pthread_create(&cropier, NULL, updateNumber, NULL);

 for (int i = 0; i < NUM_OF_PLAYERS; i++) {
    pthread_t player;
    char thread_name[17];
    sprintf(thread_name, "Jugador (%d)", i+1);
    pthread_create(&player, NULL, game, NULL);
    pthread_setname_np(player, thread_name);
    players[i] = player;
}
    
    for (int i = 0; i < NUM_OF_PLAYERS; i++) {
        pthread_join(players[i], NULL);
    }
    pthread_join(cropier, NULL);
}


void mainMenu(){

    int playing = 1;

    while(playing){
     int opcion = showMainMenu();
     system("clear");
     switch(opcion){ 
     	case 1:
        startGame(concrete,CONCRETE_NUMBER_INSTRUCTIONS);
     	   break;
     	case 2:
      startGame(evenOdd,EVEN_ODD_INSTRUCTIONS);
     	  break;
     	case 3:
           startGame(martingala,MARTINGALA_INSTRUCTIONS);
     	   break;
     	case 4:
             playing = 0;
             break;
     	default:
           printf("Opción no válida\n");
        break;
     }

    }
}

int main(){
    mainMenu();
    return 0;
}    