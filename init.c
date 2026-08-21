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

    log_state(coder->data, coder->id, "is compiling");
    pthread_mutex_lock(&coder->coder_mutex);
    coder->last_compile = get_time_ms();
    pthread_mutex_unlock(&coder->coder_mutex);
    ft_usleep(coder->data->time_to_compile);
    pthread_mutex_lock(&coder->coder_mutex);
    coder->finish_compile++;
    pthread_mutex_unlock(&coder->coder_mutex);
}

void debug(t_coder *coder){
    log_state(coder->data, coder->id, "is debugging");
    ft_usleep(coder->data->time_to_debug);
}

void refactor(t_coder *coder){
    log_state(coder->data, coder->id, "is refactoring");
    ft_usleep(coder->data->time_to_refactor);
}

void *routine(void *arg)
{
    t_coder *coder;
    t_coder *next;

    long now;
    coder = (t_coder *)arg;

    while(!is_simulation_over(coder->data)){

        pthread_mutex_lock(&coder->coder_mutex);
        if (coder->data->number_of_compiles_required > 0 &&
            coder->finish_compile >= coder->data->number_of_compiles_required)
        {
            pthread_mutex_unlock(&coder->coder_mutex);
            break;
        }
        pthread_mutex_unlock(&coder->coder_mutex);

        pthread_mutex_lock(&coder->data->scheduler_mutex);
        coder->waiting = 1;
        coder->waiting_since = get_time_ms();
        while (!is_simulation_over(coder->data))
        {
            next = scheduler(coder->data);

            if (next == coder)
                break;

            pthread_cond_wait(&coder->data->scheduler_cond,
                            &coder->data->scheduler_mutex);
        }
        if (is_simulation_over(coder->data))
        {
            coder->waiting = 0;
            pthread_mutex_unlock(&coder->data->scheduler_mutex);
            break;
        }
        coder->waiting = 0;
        pthread_mutex_unlock(&coder->data->scheduler_mutex);
    
        if(coder->data->number_of_coders == 1){
            pthread_mutex_lock(&coder->left->mutex);
            log_state(coder->data, coder->id, "has taken a dongle");

            while (!is_simulation_over(coder->data))
                usleep(1000);

            pthread_mutex_unlock(&coder->left->mutex);
            return (NULL);
        }
    
        if (coder->id % 2 == 0){
            pthread_mutex_lock(&coder->right->mutex);
            log_state(coder->data, coder->id, "has taken a dongle");
            pthread_mutex_lock(&coder->left->mutex);
            log_state(coder->data, coder->id, "has taken a dongle");
        }
    
        else{
            pthread_mutex_lock(&coder->left->mutex);
            log_state(coder->data, coder->id, "has taken a dongle");
            pthread_mutex_lock(&coder->right->mutex); 
            log_state(coder->data, coder->id, "has taken a dongle");
        }
    
        now = get_time_ms();
        if (now - coder->left->last_relase < coder->data->dongle_cooldown)
            ft_usleep(coder->data->dongle_cooldown - (now - coder->left->last_relase));
            
        now = get_time_ms();
        if (now - coder->right->last_relase < coder->data->dongle_cooldown)
            ft_usleep(coder->data->dongle_cooldown - (now - coder->right->last_relase));

        if (is_simulation_over(coder->data))
        {
            pthread_mutex_unlock(&coder->left->mutex);
            pthread_mutex_unlock(&coder->right->mutex);
            break;
        }
        compile(coder);
        now = get_time_ms();
        coder->left->last_relase = now;
        coder->right->last_relase = now;
        pthread_mutex_unlock(&coder->left->mutex);
        pthread_mutex_unlock(&coder->right->mutex);
    
        pthread_mutex_lock(&coder->data->scheduler_mutex);
        pthread_cond_broadcast(&coder->data->scheduler_cond);
        pthread_mutex_unlock(&coder->data->scheduler_mutex);
        if (is_simulation_over(coder->data))
            break;
        debug(coder);
        refactor(coder);

    }
    return (NULL);
}

int main(int argc, char **argv){
    t_data *data;
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
    {
        free(threads);
        free(data->dongles);
        free(data->coders);
        free(data);
        return (1);
    }

    data->simulation_end = 0;
    init_dongles(data->dongles, data->number_of_coders);
    data->start_time =  get_time_ms();
    pthread_mutex_init(&data->end_mutex, NULL);
    pthread_mutex_init(&data->print_mutex, NULL);
    i = 0;
    while (i < data->number_of_coders)
    {
        id = i + 1;
        data->coders[i].waiting = 0;
        data->coders[i].waiting_since = 0;
        data->coders[i].id = id;
        data->coders[i].data = data;
        data->coders[i].left = &data->dongles[i];
        data->coders[i].right = &data->dongles[id % data->number_of_coders];
        data->coders[i].finish_compile= 0;
        data->coders[i].last_compile = data->start_time;
        data->dongles[i].last_relase = data->start_time - data->dongle_cooldown;
        pthread_mutex_init(&data->coders[i].coder_mutex, NULL);
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
    pthread_mutex_destroy(&data->end_mutex);
    pthread_mutex_destroy(&data->print_mutex);
    pthread_cond_destroy(&data->scheduler_cond);
    index = 0;
    while (index < data->number_of_coders)
    {
        pthread_mutex_destroy(&data->coders[index].coder_mutex);
        pthread_mutex_destroy(&data->dongles[index].mutex);
        index++;
    }
    free(threads);
    free(data->dongles);
    free(data->coders);
    free(data);
    return (0);
}
