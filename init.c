#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *routine(){
    int value;
    value = 0;
    value++;
    // printf("the value become %d\n", value);
}

int main(int argc, char **argv){
    t_data *data;
    data = parsing(argv, argc);
    int n_coders;
 
    n_coders = data -> number_of_coders;
    pthread_t thread[data -> number_of_coders];
    while (n_coders > 0){
        printf("i'm creating a thread ...\n");
        pthread_create(&thread[n_coders], NULL, routine, NULL);
        n_coders--;
    }
    
    // printf("number of coders is: %d", data -> number_of_coders);
    return (0);
}