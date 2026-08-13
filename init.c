#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

long get_time_ms()
{
    struct timespec tv; // struct that store the time that system give it to us.
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000l) + (tv.tv_nsec * 1000l);
}

void *routine(void *arg)
{

    t_coder *coder;

    coder = (t_coder *)arg;
    // while(...){
    pthread_mutex_lock(&coder->left->mutex);
    long start = get_time_ms();
    printf("The coder %d is compiling\n", coder->id);
    coder->finish_compile++;
    long end = get_time_ms();
    usleep(coder->data->time_to_compile * 1000);
    long current = end - start;
    // and here i will need a condition to compere with burnout
    // if (current - coder->last_compile > coder->data->time_to_burnout)
    pthread_mutex_unlock(&coder->right->mutex);
    //     debug();
    //     refactor();
    // }
    return (NULL);
}

void init_dongles(t_dongle *dongles, int num_dongles){
    int i = 0;
    while (i < num_dongles)
    {
        dongles[i].id = i + 1;
        pthread_mutex_init(&dongles[i].mutex, NULL);
        i++;
    }
    init_dongles(data->dongles, data->number_of_coders);

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
    while (i < data->number_of_coders){
    pthread_t thread[data->number_of_coders];
    init_dongles(data->dongles, data->number_of_coders);

    while (i < data->number_of_coders)
    {
        int id = i + 1;
        coders[i].id = id;
        coders[i].data = data;

        coders[i].left = &data->dongles[i];
        coders[i].right = &data->dongles[id % data->number_of_coders];

        pthread_create(&thread[i], NULL, routine, &coders[i]);
        pthread_join(thread[i], NULL);
        int n = pthread_mutex_init(&mutex, NULL);
        i++;
    }

    return (0);
}