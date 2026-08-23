#include "codexion.h"

int	check_arg_count(int n_arg)
{
	if (n_arg != 9)
	{
		fprintf(stderr,
			"The number of arguments must be 9 not %d\n", n_arg);
		return (0);
	}
	return (1);
}

int	check_scheduler(char *scheduler)
{
	size_t	i;

	i = 0;
	while (scheduler[i])
	{
		if (scheduler[i] >= 'A' && scheduler[i] <= 'Z')
			scheduler[i] += 32;
		i++;
	}
	if (strcmp(scheduler, "fifo") != 0
		&& strcmp(scheduler, "edf") != 0)
	{
		fprintf(stderr,
			"The last argument must be 'fifo' or 'edf'\n");
		return (0);
	}
	return (1);
}

void	set_data_value(t_data *data, int loop, int value)
{
	if (loop == 1)
		data->number_of_coders = value;
	else if (loop == 2)
		data->time_to_burnout = value;
	else if (loop == 3)
		data->time_to_compile = value;
	else if (loop == 4)
		data->time_to_debug = value;
	else if (loop == 5)
		data->time_to_refactor = value;
	else if (loop == 6)
		data->number_of_compiles_required = value;
	else if (loop == 7)
		data->dongle_cooldown = value;
}

int	parse_values(t_data *data, char **lst, int n_arg)
{
	int		loop;
	int		value;

	loop = 1;
	while (loop <= n_arg - 2)
	{
		value = check_the_input(lst[loop], loop);
		if (value < 0)
			return (0);
		set_data_value(data, loop, value);
		loop++;
	}
	return (1);
}

void	set_scheduler(t_data *data, char *scheduler)
{
	if (strcmp(scheduler, "fifo") == 0)
		data->scheduler = 0;
	else
		data->scheduler = 1;
}