/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doabrour <doabrour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/22 19:21:53 by doabrour          #+#    #+#             */
/*   Updated: 2026/08/23 15:34:19 by doabrour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static	t_heap	*heap_create(t_data *data)
{
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->coders = malloc(sizeof(t_coder *) * data->number_of_coders);
	if (!heap->coders)
	{
		free(heap);
		return (NULL);
	}
	heap->size = 0;
	heap->capacity = data->number_of_coders;
	heap->data = data;
	return (heap);
}

static	void	heap_destroy(t_heap *heap)
{
	if (!heap)
		return ;
	free(heap->coders);
	free(heap);
}

static	void	heap_swap(t_coder **a, t_coder **b)
{
	t_coder	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static	void	heap_push(t_heap *heap, t_coder *coder)
{
	int	i;
	int	parent;

	if (heap->size >= heap->capacity)
		return ;
	i = heap->size;
	heap->coders[i] = coder;
	heap->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (!higher_priority(heap->data, heap->coders[i], heap->coders[parent]))
			break ;
		heap_swap(&heap->coders[i], &heap->coders[parent]);
		i = parent;
	}
}

static	t_coder	*heap_pop(t_heap **heap)
{
	t_coder	*best;

	if (!heap || (*heap)->size == 0)
		return (NULL);
	best = (*heap)->coders[0];
	(*heap)->size--;
	if ((*heap)->size == 0)
		return (best);
	(*heap)->coders[0] = (*heap)->coders[(*heap)->size];
	heapify_down(*heap);
	return (best);
}
