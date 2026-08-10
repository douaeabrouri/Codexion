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
    // if (dongles == NULL || coders == NULL)
    //     return (NULL);
    pthread_t thread[data->number_of_coders];
    // printf("%p\n", (void *)data->dongles);
    // printf("%d\n", data->number_of_coders);

    int i = 0;
    while (i < data->number_of_coders){
        
        printf("1 %d\n", i);
        data->coders[i].id = i + 1;
        printf("2 %d\n", i);

        data->dongles[i].id = i + 1;
        printf("3 %d\n", i);

        data->coders[i].left = &data->dongles[i];
        printf("4 %d\n", i);

        // printf("%p\n", (void *)data->coders[i].left);
        data->coders[i].right = &data->dongles[(i + 1) % data->number_of_coders];
        printf("5 %d\n", i);

        // printf("coder id = %d left = %d right = %d\n", coders[i].id, data->coders[i].left->id, data->coders[i].right->id);
        // printf("coder id = %d\n", coders[i].id);
        // printf("coder id = %d\n", coders[i].id);
        pthread_create(&thread[i], NULL, routine, NULL);
        i++;

    }

    return (0);
}