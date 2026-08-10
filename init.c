#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *routine(){
    int value;
    value = 0;
    value++;
    return (NULL);
    // printf("the value become %d\n", value);
}

int main(int argc, char **argv){
    t_data *data;
    t_coder *coders;
    t_dongle *dongles;

    data = parsing(argv, argc);
    data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
    data->coders = malloc(sizeof(t_coder) * data->number_of_coders);

    pthread_t thread[data->number_of_coders];


    int i = 0;
    while (i < data->number_of_coders){
        int id = i + 1;
        data->coders[i].id = id;
        data->dongles[i].id = id;

        data->coders[i].left = &data->dongles[i];
        int left_dogle_index = id % data->number_of_coders;
        data->coders[i].right = &data->dongles[left_dogle_index];
        pthread_create(&thread[i], NULL, routine, NULL);
        i++;
    }

    return (0);
}
