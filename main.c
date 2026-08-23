/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doabrour <doabrour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 19:32:02 by doabrour          #+#    #+#             */
/*   Updated: 2026/08/23 15:42:00 by doabrour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	create_coder_threads(t_data *data, pthread_t *threads)
{
	int	index;

	index = 0;
	while (index < data->number_of_coders)
	{
		pthread_create(&threads[index], NULL,
			routine, &data->coders[index]);
		index++;
	}
}

void	join_coder_threads(t_data *data, pthread_t *threads)
{
	int	index;

	index = 0;
	while (index < data->number_of_coders)
	{
		pthread_join(threads[index], NULL);
		index++;
	}
}

void	cleanup(t_data *data, pthread_t *threads)
{
	int	index;

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
}

int	main(int argc, char **argv)
{
	t_data		*data;
	pthread_t	*threads;
	pthread_t	monitor_thread;

	data = parsing(argv, argc);
	if (!data || data->number_of_coders <= 0)
		return (1);
	if (!allocate_resources(data, &threads))
	{
		free(data);
		return (1);
	}
	init_simulation(data);
	init_coders(data);
	create_coder_threads(data, threads);
	pthread_create(&monitor_thread, NULL, monitor, data);
	join_coder_threads(data, threads);
	pthread_join(monitor_thread, NULL);
	cleanup(data, threads);
	return (0);
}
