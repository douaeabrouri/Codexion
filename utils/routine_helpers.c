/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doabrour <doabrour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 19:33:23 by doabrour          #+#    #+#             */
/*   Updated: 2026/08/23 15:17:39 by doabrour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile(t_coder *coder)
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

void	debug(t_coder *coder)
{
	log_state(coder->data, coder->id, "is debugging");
	ft_usleep(coder->data->time_to_debug);
}

void	refactor(t_coder *coder)
{
	log_state(coder->data, coder->id, "is refactoring");
	ft_usleep(coder->data->time_to_refactor);
}

int	coders_is_finished(t_coder *coder)
{
	int	finished;

	finished = 0;
	pthread_mutex_lock(&coder->coder_mutex);
	if (coder->data->number_of_compiles_required > 0 && \
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
