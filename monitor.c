#include "codexion.h"

int compiles_finish(t_data *data){
    int index;

    index = 0;
    while(index < data->number_of_coders){
        if(data->coders[index].finish_compile < data->number_of_compiles_required)
            return (0);
        index++;
    }
    return (1);
}
void set_simulation_over(t_data *data){

    pthread_mutex_lock(&data->end_mutex);
    data->simulation_end = 1;
    pthread_mutex_unlock(&data->end_mutex);

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
    while(!data->simulation_end){
        index = 0;
        while(index < data->number_of_coders){
            if(get_time_ms() - data->coders[index].last_compile > data->time_to_burnout){
                printf("%ld %d burned out\n", get_time_ms() - data->start_time, data->coders[index].id);
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

