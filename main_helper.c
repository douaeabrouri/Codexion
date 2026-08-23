/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doabrour <doabrour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 15:41:39 by doabrour          #+#    #+#             */
/*   Updated: 2026/08/23 15:42:17 by doabrour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	allocate_resources(t_data *data, pthread_t **threads)
{
	int	count;

	count = data->number_of_coders;
	*threads = malloc(sizeof(pthread_t) * count);
	data->dongles = malloc(sizeof(t_dongle) * count);
	data->coders = malloc(sizeof(t_coder) * count);
	if (!*threads || !data->dongles || !data->coders)
	{
		free(*threads);
		free(data->dongles);
		free(data->coders);
		return (0);
	}
	return (1);
}

void	init_simulation(t_data *data)
{
	data->simulation_end = 0;
	init_dongles(data->dongles, data->number_of_coders);
	data->start_time = get_time_ms();
	pthread_mutex_init(&data->end_mutex, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->scheduler_mutex, NULL);
	pthread_cond_init(&data->scheduler_cond, NULL);
}

void	init_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].waiting = 0;
		data->coders[i].waiting_since = 0;
		data->coders[i].data = data;
		data->coders[i].left = &data->dongles[i];
		data->coders[i].right = &data->dongles[(i + 1)
			% data->number_of_coders];
		data->coders[i].finish_compile = 0;
		data->coders[i].last_compile = data->start_time;
		data->dongles[i].last_relase = data->start_time - data->dongle_cooldown;
		pthread_mutex_init(&data->coders[i].coder_mutex, NULL);
		i++;
	}
}
