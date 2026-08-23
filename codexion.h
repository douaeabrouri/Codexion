/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doabrour <doabrour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 15:18:52 by doabrour          #+#    #+#             */
/*   Updated: 2026/08/23 15:28:24 by doabrour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <string.h>
# include <stdio.h>
# include <pthread.h>
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <sys/time.h>

typedef struct s_data	t_data;

typedef struct s_dongle
{
	int				id;
	long			last_relase;
	pthread_mutex_t	mutex;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	t_dongle		*right;
	t_dongle		*left;
	t_data			*data;
	long			last_compile;
	int				finish_compile;
	long			waiting_since;
	int				waiting;
	pthread_mutex_t	coder_mutex;
}	t_coder;

typedef struct s_data
{
	int				number_of_coders;
	long			time_to_burnout;
	long			time_to_compile;
	long			time_to_debug;
	long			time_to_refactor;
	int				number_of_compiles_required;
	long			start_time;
	int				scheduler;
	int				simulation_end;
	int				dongle_cooldown;
	pthread_mutex_t	end_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	scheduler_mutex;
	pthread_cond_t	scheduler_cond;
	t_dongle		*dongles;
	t_coder			*coders;
}	t_data;

typedef struct s_heap{
	t_coder	**coders;
	int		size;
	int		capacity;
	t_data	*data;
}	t_heap;

t_data			*parsing(char **lst, int n_arg);
long			get_time_ms(void);
void			ft_usleep(long ms);
void			*monitor(void *arg);
void			*routine(void *arg);
int				is_simulation_over(t_data *data);
void			set_simulation_over(t_data *data);
void			log_state(t_data *data, int id, char *state);
t_coder			*scheduler(t_data *data);
void			set_simulation_over(t_data *data);
void			log_state(t_data *data, int id, char *state);
int				is_simulation_over(t_data *data);
int				coders_is_finished(t_coder *coder);
void			release_dongle(t_coder *coder);
void			init_dongles(t_dongle *dongles, int number_of_dongles);
int				wait_for_scheduler(t_coder *coder);
int				one_coder(t_coder *coder);
void			take_two_dongles(t_coder *coder);
void			handle_cooldown(t_coder *coder);
void			release_dongle(t_coder *coder);
void			compile(t_coder *coder);
void			debug(t_coder *coder);
void			refactor(t_coder *coder);
int				check_arg_count(int n_arg);
int				check_scheduler(char *scheduler);
void			set_data_value(t_data *data, int loop, int value);
int				parse_values(t_data *data, char **lst, int n_arg);
void			set_scheduler(t_data *data, char *scheduler);
static void		heap_swap(t_coder **a, t_coder **b);
static t_heap	*heap_create(t_data *data);
static void		heap_push(t_heap *heap, t_coder *coder);
static t_coder	*heap_pop(t_heap **heap);

#endif
