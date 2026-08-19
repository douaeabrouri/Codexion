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

void *routine(void *arg)
{
    t_coder *coder;
    t_coder *next;

    long now;
    coder = (t_coder *)arg;
    while(!is_simulation_over(coder->data)){

        coder->waiting = 1;
        coder->waiting_since = get_time_ms();
        next = scheduler(coder->data);
        if (next == coder){
            if (coder->id % 2 == 0){
                pthread_mutex_lock(&coder->right->mutex);
                pthread_mutex_lock(&coder->left->mutex);
            }
            else{
                pthread_mutex_lock(&coder->left->mutex);
                pthread_mutex_lock(&coder->right->mutex); 
            }
            if (is_simulation_over(coder->data))
            {
                pthread_mutex_unlock(&coder->left->mutex);
                pthread_mutex_unlock(&coder->right->mutex);
                break;
            }
            now = get_time_ms();
            coder->waiting = 0;
            compile(coder);
            coder->left->last_relase = get_time_ms();
            coder->right->last_relase = get_time_ms();
            if (now - coder->data->dongles->last_relase < coder->data->dongle_cooldown)
                ft_usleep(coder->data->dongle_cooldown);
            pthread_mutex_unlock(&coder->left->mutex);
            pthread_mutex_unlock(&coder->right->mutex);
            if (is_simulation_over(coder->data))
                break;
            debug(coder);
            refactor(coder);
        }
        else{
            pthread_cond_wait(&coder->data->scheduler_cond, &coder->data->scheduler_mutex);
        }

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
    pthread_mutex_init(&data->scheduler_mutex, NULL);
    pthread_cond_init(&data->scheduler_cond, NULL);
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

    pthread_mutex_destroy(&data->scheduler_mutex);
    pthread_cond_destroy(&data->scheduler_cond);
    free(threads);
    free(data->dongles);
    free(data->coders);
    free(data);
    return (0);
}
