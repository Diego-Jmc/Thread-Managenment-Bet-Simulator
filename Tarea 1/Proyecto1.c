#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int bankBalance = 5000;
int numberToGuess;

int pickOption(){
   
   int opc;    
   printf("La banca empieza con 5000!\n");  
   printf("(1) Jugar al azar\n");
   printf("(2) Jugar al numero par/impar \n");
   printf("(3) Jugar a la martingala\n");
   scanf("%d", &opc);
   return opc;
}


// locker initialization
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int main(){
    
   srand(time(NULL));
   
   // initialazing number to guess between 1 and 36
   
   numberToGuess = rand() % (36- 1 + 1) + 1;
  
  // array that contains the 4 players
   pthread_t players[4];
   
   
   int opc = pickOption();
   
   switch(opc){
       
       case 1:
            printf("Eligio al azar");
            break;
            case 2:
                printf("Eligio numero par");
                break;
                case 3:
                    printf("Eligio martingala");
                    break;
                    
       
       
   }
   
   
   

    return 0;
}