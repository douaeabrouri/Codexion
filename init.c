#include "codexion.h"

void init_dongles(t_dongle *dongles, int num_dongles){
    int i = 0;
    while (i < num_dongles)
    {
        dongles[i].id = i + 1;
        pthread_mutex_init(&dongles[i].mutex, NULL);
        i++;
    }
}

void compile(t_coder *coder){

    printf("%ld %d is compiling\n", get_time_ms() - coder->data->start_time, coder->id);
    coder->last_compile = get_time_ms();
    ft_usleep(coder->data->time_to_compile);
    coder->finish_compile++;
}

void debug(t_coder *coder){
    printf("%ld %d is debugging\n", get_time_ms() - coder->data->start_time,coder->id);
    ft_usleep(coder->data->time_to_debug);
}

void refactor(t_coder *coder){
    printf("%ld %d is refactoring\n", get_time_ms() - coder->data->start_time,coder->id);
    ft_usleep(coder->data->time_to_refactor);
}
int counter = 0;
void *routine(void *arg)
{
    t_coder *coder;
    long now;
    coder = (t_coder *)arg;
    while(!coder->data->simulation_end){
        pthread_mutex_lock(&coder->left->mutex);
        pthread_mutex_lock(&coder->right->mutex);
        now = get_time_ms();
        compile(coder);
        coder->left->last_release = get_time_ms();
        coder->right->last_release = get_time_ms();
        if (now - dongles->last_release < data->dongle_cooldown)
            ft_usleep(data->dongle_cooldown);
        pthread_mutex_unlock(&coder->left->mutex);
        pthread_mutex_unlock(&coder->right->mutex);
        debug(coder);
        refactor(coder);
    }
    return (NULL);
}


int main(int argc, char **argv){
    t_data *data;
    t_coder *coders;
    int i, id;
    pthread_t monitor_thread;

    data = parsing(argv, argc);
    if(!data || data->number_of_coders <= 0)
        return (1);

    pthread_t *threads;
    threads = malloc(sizeof(pthread_t) * data->number_of_coders);
    data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
    data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
    
    if (!threads || !data->dongles || !data->coders)
        return (1);

    data->simulation_end = 0;

    init_dongles(data->dongles, data->number_of_coders);

    data->start_time =  get_time_ms();
    i = 0;
    while (i < data->number_of_coders)
    {
        id = i + 1;
        data->coders[i].id = id;
        data->coders[i].data = data;
        data->coders[i].left = &data->dongles[i];
        data->coders[i].right = &data->dongles[id % data->number_of_coders];
        data->coders[i].finish_compile= 0;
        data->coders[i].last_compile = data->start_time;
        i++;
    }


    int index = 0;
    while (index < data->number_of_coders){
        pthread_create(&threads[index], NULL, routine, &data->coders[index]);
        index++;
    }
    pthread_create(&monitor_thread, NULL, monitor, data);
    index = 0;
    while(index < data->number_of_coders){
        pthread_join(threads[index], NULL);
        index++;
    }

    pthread_join(monitor_thread, NULL);
    free(threads);
    free(data->dongles);
    free(data->coders);
    free(data);
    return (0);

}
