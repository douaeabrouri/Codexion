#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *routine(void *arg){
    
    t_coder *coder;

    coder = (t_coder *)arg;
    // printf("coder %d start\n", coder->id);
    // while(...){
    //     compile();
    //     debug();
    //     refactor();
    // }
    return (NULL);
}


void init_dongles(t_dongle *dongles, int num_dongles) {
    int i = 0;
    while (i < num_dongles) {
        dongles[i].id = i + 1;
        i++;
    }
}

pthread_mutex_t mutex;

int main(int argc, char **argv){
    t_data *data;
    t_coder *coders;
    // int mutex = 0;
    int i = 0;

    data = parsing(argv, argc);
    data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
    coders = malloc(sizeof(t_coder) * data->number_of_coders);

    pthread_t thread[data->number_of_coders];
    init_dongles(data->dongles, data->number_of_coders);

    while (i < data->number_of_coders){
        int id = i + 1;
        coders[i].id = id;
        coders[i].data = data;

        coders[i].left = &data->dongles[i];
        coders[i].right = &data->dongles[id % data->number_of_coders];
        
        pthread_create(&thread[i], NULL, routine, &coders[i]);
        pthread_join(thread[i], NULL);
        // int n = pthread_mutex_init(&mutex, NULL);
        printf("mutex %d\n", n);
        printf("coder id %d left %d right %d\n", coders[i].id, coders[i].left->id, coders[i].right->id);
        i++;
    }

    return (0);
}
