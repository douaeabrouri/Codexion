/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doabrour <doabrour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 17:02:40 by doabrour          #+#    #+#             */
/*   Updated: 2026/08/23 15:59:31 by doabrour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static	int	higher_priority(t_data *data, t_coder *a, t_coder *b)
{
	long	a_deadline;
	long	b_deadline;
	long	a_last;
	long	b_last;

	if (data->scheduler == 0)
	{
		if (a->waiting_since != b->waiting_since)
			return (a->waiting_since < b->waiting_since);
		return (a->id < b->id);
	}
	pthread_mutex_lock(&a->coder_mutex);
	a_last = a->last_compile;
	pthread_mutex_unlock(&a->coder_mutex);
	pthread_mutex_lock(&b->coder_mutex);
	b_last = b->last_compile;
	pthread_mutex_unlock(&b->coder_mutex);
	a_deadline = a_last + a->data->time_to_burnout;
	b_deadline = b_last + b->data->time_to_burnout;
	if (a_deadline != b_deadline)
		return (a_deadline < b_deadline);
	if (a->waiting_since != b->waiting_since)
		return (a->waiting_since < b->waiting_since);
	return (a->id < b->id);
}

static	void	heapify_down(t_heap *heap)
{
	int	i;
	int	left;
	int	right;
	int	priority;

	i = 0;
	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		priority = i;
		if (left < heap->size
			&& higher_priority(heap->data, heap->coders[left],
				heap->coders[priority]))
			priority = left;
		if (right < heap->size
			&& higher_priority(heap->data, heap->coders[right],
				heap->coders[priority]))
			priority = right;
		if (priority == i)
			break ;
		heap_swap(&heap->coders[i], &heap->coders[priority]);
		i = priority;
	}
}

static int	is_better_edf(t_data *data, t_coder *coder, t_coder *best)
{
	long	current_deadline;
	long	best_deadline;

	if (!best)
		return (1);
	current_deadline = coder->last_compile + data->time_to_burnout;
	best_deadline = best->last_compile + data->time_to_burnout;
	if (current_deadline < best_deadline)
		return (1);
	if (current_deadline > best_deadline)
		return (0);
	if (coder->waiting_since < best->waiting_since)
		return (1);
	if (coder->waiting_since > best->waiting_since)
		return (0);
	return (coder->id < best->id);
}

t_coder	*edf(t_data *data)
{
	t_coder	*best;
	int		i;

	best = NULL;
	i = 0;
	while (i < data->number_of_coders)
	{
		if (data->coders[i].waiting
			&& is_better_edf(data, &data->coders[i], best))
			best = &data->coders[i];
		i++;
	}
	return (best);
}

t_coder	*scheduler(t_data *data)
{
	t_heap	*heap;
	t_coder	*best;
	int		i;

	if (data->scheduler == 1)
		return (edf(data));
	heap = heap_create(data);
	if (!heap)
		return (NULL);
	i = 0;
	while (i < data->number_of_coders)
	{
		if (data->coders[i].waiting)
			heap_push(heap, &data->coders[i]);
		i++;
	}
	best = heap_pop(heap);
	heap_destroy(heap);
	return (best);
}
