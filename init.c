#include "codexion.h"

void init_dongles(t_data *data, t_dongle *dongles, int num_dongles){
    int i = 0;
    while (i < num_dongles)
    {
        dongles[i].id = i + 1;
        pthread_mutex_init(&dongles[i].mutex, NULL);
        i++;
    }
}

void compile(t_coder *coder){

    long start = get_time_ms();
    coder->finish_compile++;
    printf("%ld Coder %d is compiling\n", get_time_ms() - coder->data->start_time,coder->id);
}

void debug(t_coder *coder){
    printf("%ld Coder %d is debugging\n", get_time_ms() - coder->data->start_time,coder->id);
    ft_usleep(coder->data->time_to_debug);
}

void refactor(t_coder *coder){
    printf("%ld Coder %d is refactoring\n", get_time_ms() - coder->data->start_time,coder->id);
    ft_usleep(coder->data->time_to_refactor);
}

void *routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    // while(start < coder->data->number_of_coders){
    pthread_mutex_lock(&coder->left->mutex);
    pthread_mutex_lock(&coder->right->mutex);
    compile(coder);
    // if (current - coder->last_compile > coder->data->time_to_burnout)
    //     printf("the coder shoul0.d stop!");
    pthread_mutex_unlock(&coder->left->mutex);
    pthread_mutex_unlock(&coder->right->mutex);
    debug(coder);
    refactor(coder);
    // }
    return (NULL);
}


int main(int argc, char **argv){
    t_data *data;
    t_coder *coders;
    // int mutex = 0;
    int i = 0;

    data = parsing(argv, argc);
    data->start_time = get_time_ms();
    data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
    coders = malloc(sizeof(t_coder) * data->number_of_coders);

    pthread_t thread[data->number_of_coders];
    init_dongles(data, data->dongles, data->number_of_coders);

    while (i < data->number_of_coders)
    {
        int id = i + 1;
        coders[i].id = id;
        coders[i].data = data;

        coders[i].left = &data->dongles[i];
        coders[i].right = &data->dongles[id % data->number_of_coders];
        i++;
    }
    int index = 0;
    while (index < data->number_of_coders){
        pthread_create(&thread[index], NULL, routine, &coders[index]);
        index++;
    }
    index = 0;
    while(index < data->number_of_coders){
        pthread_join(thread[index], NULL);
        index++;
    }
    return (0);
// 4 5 7 6 4 5 7 fifo 
}
