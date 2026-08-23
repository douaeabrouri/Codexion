/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doabrour <doabrour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/23 15:54:37 by doabrour          #+#    #+#             */
/*   Updated: 2026/08/23 16:11:09 by doabrour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_value(char *str, size_t *index, long *value, int loop)
{
	if (str[*index] < '0' || str[*index] > '9')
	{
		printf("Invalide value -> %c in position %d\n",
			str[*index], loop);
		return (0);
	}
	*value = *value * 10 + (str[*index] - '0');
	if (*value > INT_MAX)
	{
		printf("Invalid Value\n");
		return (0);
	}
	(*index)++;
	return (1);
}

int	check_the_input(char *str, int loop)
{
	size_t	index;
	long	value;

	index = 0;
	value = 0;
	if (!str[0])
	{
		printf("Invalid empty value in position %d\n", loop);
		return (-1);
	}
	while (str[index])
	{
		if (!check_value(str, &index, &value, loop))
			return (-1);
	}
	return ((int)value);
}

t_data	*parsing(char **lst, int n_arg)
{
	t_data	*data;

	if (!check_arg_count(n_arg))
		return (NULL);
	if (!check_scheduler(lst[n_arg - 1]))
		return (NULL);
	data = malloc(sizeof(t_data));
	if (data == NULL)
		return (NULL);
	if (!parse_values(data, lst, n_arg))
	{
		free(data);
		return (NULL);
	}
	set_scheduler(data, lst[n_arg - 1]);
	return (data);
}
