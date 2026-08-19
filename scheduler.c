#include "codexion.h"


t_coder *edf(t_data *data){

    long best_deadline;
    long current_deadline;
    t_coder *best;
    int i;
    
    best = &data->coders[0];
    best_deadline = best->last_compile + best->data->time_to_burnout;
    i = 0;
    while(i < data->number_of_coders){
        current_deadline = data->coders[i].last_compile + data->coders[i].data->time_to_burnout;

        if (current_deadline < best_deadline)
        {
            best = &data->coders[i];
            best_deadline = current_deadline;
        }
        i++;
    }
    return (best);
}

t_coder *fifo(t_data *data){
    t_coder* best;
    int i;

    best = NULL;
    i = 0;
    while(i < data->number_of_coders){
        if (data->coders[i].waiting == 0)
        {
            i++;
            continue;
        }
        if (best == NULL)
            best = &data->coders[i];
        else if(data->coders[i].waiting_since < best->waiting_since)
            best = &data->coders[i];
        i++;
    }
    return (best);
}

t_coder *scheduler(t_data * data){
    if (data->scheduler == 0)
        return (fifo(data));
    if (data->scheduler == 1)
        return (edf(data));
    return (NULL);
}