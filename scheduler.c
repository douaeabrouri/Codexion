#include "codexion.h"

static t_heap *heap_create(t_data *data)
{
    t_heap *heap;

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

static void heap_destroy(t_heap *heap)
{
    if (!heap)
        return ;
    free(heap->coders);
    free(heap);
}

static int higher_priority(t_data *data, t_coder *a, t_coder *b)
{
    long a_deadline;
    long b_deadline;
    long a_last;
    long b_last;
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

static void heap_swap(t_coder **a, t_coder **b)
{
    t_coder *tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heap_push(t_heap *heap, t_coder *coder)
{
    int i;
    int parent;

    if (heap->size >= heap->capacity)
        return ;
    i = heap->size;
    heap->coders[i] = coder;
    heap->size++;
    while (i > 0)
    {
        parent = (i - 1) / 2;
        if (!higher_priority(heap->data, heap->coders[i],
                heap->coders[parent]))
            break ;
        heap_swap(&heap->coders[i], &heap->coders[parent]);
        i = parent;
    }
}

static t_coder *heap_pop(t_heap *heap)
{
    t_coder *best;
    int i;
    int left;
    int right;
    int priority;

    if (!heap || heap->size == 0)
        return (NULL);
    best = heap->coders[0];
    heap->size--;
    if (heap->size == 0)
        return (best);
    heap->coders[0] = heap->coders[heap->size];
    i = 0;
    while (1)
    {
        left = 2 * i + 1;
        right = 2 * i + 2;
        priority = i;
        if (left < heap->size && higher_priority(heap->data,
                heap->coders[left], heap->coders[priority]))
            priority = left;
        if (right < heap->size && higher_priority(heap->data,
                heap->coders[right], heap->coders[priority]))
            priority = right;
        if (priority == i)
            break ;
        heap_swap(&heap->coders[i], &heap->coders[priority]);
        i = priority;
    }
    return (best);
}

t_coder *edf(t_data *data)
{
    t_coder *best;
    long best_deadline;
    long current_deadline;
    int i;

    best = NULL;
    i = 0;

    while (i < data->number_of_coders)
    {
        if (data->coders[i].waiting == 0)
        {
            i++;
            continue;
        }
        current_deadline = data->coders[i].last_compile
            + data->time_to_burnout;

        if (best == NULL)
        {
            best = &data->coders[i];
            best_deadline = current_deadline;
        }
        else if (current_deadline < best_deadline)
        {
            best = &data->coders[i];
            best_deadline = current_deadline;
        }
        else if (current_deadline == best_deadline)
        {
            if (data->coders[i].waiting_since < best->waiting_since)
            {
                best = &data->coders[i];
                best_deadline = current_deadline;
            }
            else if (data->coders[i].waiting_since == best->waiting_since
                && data->coders[i].id < best->id)
            {
                best = &data->coders[i];
                best_deadline = current_deadline;
            }
        }

        i++;
    }

    return (best);
}

t_coder *scheduler(t_data *data)
{
    t_heap *heap;
    t_coder *best;
    int i;

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
