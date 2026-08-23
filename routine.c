/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doabrour <doabrour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 17:04:56 by doabrour          #+#    #+#             */
/*   Updated: 2026/08/22 19:34:21 by doabrour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int	wait_for_scheduler(t_coder *coder)
{
	t_coder	*next;
	t_data	*data;

	data = coder->data;
	pthread_mutex_lock(&data->scheduler_mutex);
	coder->waiting = 1;
	coder->waiting_since = get_time_ms();
	while (!is_simulation_over(data))
	{
		next = scheduler(data);
		if (next == coder)
			break ;
		pthread_cond_wait(&data->scheduler_cond,
			&data->scheduler_mutex);
	}
	if (is_simulation_over(data))
		return (scheduler_stopped(coder));
	coder->waiting = 0;
	pthread_mutex_unlock(&data->scheduler_mutex);
	return (1);
}

int one_coder(t_coder *coder)
{
    if(coder->data->number_of_coders != 1)
        return (0);
    pthread_mutex_lock(&coder->left->mutex);
    log_state(coder->data, coder->id, "has taken a dongle");
    while (!is_simulation_over(coder->data))
        usleep(1000);
    pthread_mutex_unlock(&coder->left->mutex);
    return (1);
}

void take_two_dongles(t_coder *coder)
{
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
}

void  wait_dongle_cooldown(t_coder *coder, t_dongle *dongle)
{
    long now;
    long wait;
    
    (void)dongle;
    now = get_time_ms();
    wait = coder->data->dongle_cooldown - (now - coder->left->last_relase);
    if (wait > 0)
        ft_usleep(wait);
}

void	handle_cooldown(t_coder *coder)
{
	wait_dongle_cooldown(coder, coder->left);
	wait_dongle_cooldown(coder, coder->right);
}

