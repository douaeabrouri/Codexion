/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doabrour <doabrour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 17:04:56 by doabrour          #+#    #+#             */
/*   Updated: 2026/08/21 19:29:20 by doabrour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


void compile(t_coder *coder)
{
    log_state(coder->data, coder->id, "is compiling");
    pthread_mutex_lock(&coder->coder_mutex);
    coder->last_compile = get_time_ms();
    pthread_mutex_unlock(&coder->coder_mutex);
    ft_usleep(coder->data->time_to_compile);
    pthread_mutex_lock(&coder->coder_mutex);
    coder->finish_compile++;
    pthread_mutex_unlock(&coder->coder_mutex);
}

void debug(t_coder *coder)
{
    log_state(coder->data, coder->id, "is debugging");
    ft_usleep(coder->data->time_to_debug);
}

void refactor(t_coder *coder)
{
    log_state(coder->data, coder->id, "is refactoring");
    ft_usleep(coder->data->time_to_refactor);
}

int coders_is_finished(t_coder *coder)
{
    int finished;

    finished = 0;
    pthread_mutex_lock(&coder->coder_mutex);
    if (coder->data->number_of_compiles_required > 0 &&
        coder->finish_compile
        >= coder->data->number_of_compiles_required)
        finished = 1;
    pthread_mutex_unlock(&coder->coder_mutex);
    return (finished);

}

int	scheduler_stopped(t_coder *coder)
{
	coder->waiting = 0;
	pthread_mutex_unlock(&coder->data->scheduler_mutex);
	return (0);
}

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
