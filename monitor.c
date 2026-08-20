#include "codexion.h"

int compiles_finish(t_data *data)
{
    int index;

    if (data->number_of_compiles_required <= 0)
        return (0);
    index = 0;
    while (index < data->number_of_coders)
    {
        pthread_mutex_lock(&data->coders[index].coder_mutex);
        if (data->coders[index].finish_compile
                < data->number_of_compiles_required)
        {
            pthread_mutex_unlock(&data->coders[index].coder_mutex);
            return (0);
        }
        pthread_mutex_unlock(&data->coders[index].coder_mutex);
        index++;
    }
    return (1);
}
void set_simulation_over(t_data *data){

    pthread_mutex_lock(&data->end_mutex);
    data->simulation_end = 1;
    pthread_mutex_unlock(&data->end_mutex);

    pthread_mutex_lock(&data->scheduler_mutex);
    pthread_cond_broadcast(&data->scheduler_cond);
    pthread_mutex_unlock(&data->scheduler_mutex);

}

void log_state(t_data *data, int id, char *state){

    pthread_mutex_lock(&data->print_mutex);
    if (!is_simulation_over(data) || strcmp(state, "burned out") == 0)
        printf("%ld %d %s\n", get_time_ms() - data->start_time, id, state);
    pthread_mutex_unlock(&data->print_mutex);
}

int is_simulation_over(t_data *data){

    int end;
    pthread_mutex_lock(&data->end_mutex);
    end = data->simulation_end;
    pthread_mutex_unlock(&data->end_mutex);
    return (end);

}

void *monitor(void *arg){
    t_data *data;
    int index;

    data = (t_data *)arg;
    while(!is_simulation_over(data)){
        index = 0;
        while(index < data->number_of_coders){
            pthread_mutex_lock(&data->coders[index].coder_mutex);
            if (data->number_of_compiles_required > 0 && 
                data->coders[index].finish_compile >= data->number_of_compiles_required)
            {
                pthread_mutex_unlock(&data->coders[index].coder_mutex);
                index++;
                continue;
            }
            long last_compile = data->coders[index].last_compile;
            pthread_mutex_unlock(&data->coders[index].coder_mutex);
            if(get_time_ms() - last_compile > data->time_to_burnout){
                log_state(data, data->coders[index].id, "burned out");
                set_simulation_over(data);
                return (NULL);
            }
            index++;
        }
        if(compiles_finish(data)){
            set_simulation_over(data);
           return (NULL);
        }
        usleep(1000);
    }
    return (NULL);
}
