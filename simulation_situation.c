/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_situation.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doabrour <doabrour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 16:59:46 by doabrour          #+#    #+#             */
/*   Updated: 2026/08/22 19:34:45 by doabrour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

//  routine helper 
void release_dongle(t_coder *coder)
{
    long now;

    now = get_time_ms();
    coder->left->last_relase = now;
    coder->right->last_relase = now;
    pthread_mutex_unlock(&coder->left->mutex);
    pthread_mutex_unlock(&coder->right->mutex);
    pthread_mutex_lock(&coder->data->scheduler_mutex);
    pthread_cond_broadcast(&coder->data->scheduler_cond);
    pthread_mutex_unlock(&coder->data->scheduler_mutex);
}