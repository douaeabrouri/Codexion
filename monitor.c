/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doabrour <doabrour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 17:00:02 by doabrour          #+#    #+#             */
/*   Updated: 2026/08/23 15:50:01 by doabrour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	compiles_finish(t_data *data)
{
	int	index;

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

int	check_finish(t_data *data, int index)
{
	int	finished;

	pthread_mutex_lock(&data->coders[index].coder_mutex);
	finished = data->number_of_compiles_required > 0
		&& data->coders[index].finish_compile
		>= data->number_of_compiles_required;
	pthread_mutex_unlock(&data->coders[index].coder_mutex);
	return (finished);
}

long	get_last_compile(t_data *data, int index)
{
	long	last_compile;

	pthread_mutex_lock(&data->coders[index].coder_mutex);
	last_compile = data->coders[index].last_compile;
	pthread_mutex_unlock(&data->coders[index].coder_mutex);
	return (last_compile);
}

void	check_coders(t_data *data, long current_time, int *all_finished)
{
	int		index;
	long	last_compile;

	index = 0;
	while (index < data->number_of_coders)
	{
		if (!check_finish(data, index))
			*all_finished = 0;
		else
		{
			index++;
			continue ;
		}
		last_compile = data->coders[index].last_compile;
		if (current_time - last_compile > data->time_to_burnout)
		{
			log_state(data, data->coders[index].id, "burned out");
			set_simulation_over(data);
			return ;
		}
		index++;
	}
}

void	*monitor(void *arg)
{
	t_data		*data;
	int			all_finished;
	long		current_time;

	data = (t_data *)arg;
	while (!is_simulation_over(data))
	{
		all_finished = 1;
		current_time = get_time_ms();
		check_coders(data, current_time, &all_finished);
		if (data->number_of_compiles_required > 0 && all_finished)
		{
			set_simulation_over(data);
			return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}
