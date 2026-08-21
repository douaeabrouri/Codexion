/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doabrour <doabrour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 17:04:03 by doabrour          #+#    #+#             */
/*   Updated: 2026/08/21 19:28:03 by doabrour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void init_dongles(t_dongle *dongles, int num_dongles){
    int i = 0;
    while (i < num_dongles)
    {
        dongles[i].id = i + 1;
        pthread_mutex_init(&dongles[i].mutex, NULL);
        i++;
    }
}

void *routine(void *arg)
{
    t_coder *coder;
    long now;

    coder = (t_coder *)arg;
    while(!is_simulation_over(coder->data)){
        if(!coders_is_finished(coder))
            break ;
        if(!wait_for_scheduler(coder))
            break ;
		if (one_coder(coder))
			return (NULL);
        //
        take_two_dongles(coder);
        handle_cooldown(coder);
        if (is_simulation_over(coder->data))
        {
            pthread_mutex_unlock(&coder->left->mutex);
            pthread_mutex_unlock(&coder->right->mutex);
            break;
        }
        compile(coder);
        now = get_time_ms();
        release_dongle(coder);
        if (is_simulation_over(coder->data))
            break;
        debug(coder);
        refactor(coder);

    }
    return (NULL);
}

int main(int argc, char **argv){
    t_data *data;
    int i, id;
    pthread_t monitor_thread;

    data = parsing(argv, argc);
    if(!data || data->number_of_coders <= 0)
        return (1);

    pthread_t *threads;
    threads = malloc(sizeof(pthread_t) * data->number_of_coders);
    data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
    data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
    
    if (!threads || !data->dongles || !data->coders)
    {
        free(threads);
        free(data->dongles);
        free(data->coders);
        free(data);
        return (1);
    }

    data->simulation_end = 0;
    init_dongles(data->dongles, data->number_of_coders);
    data->start_time =  get_time_ms();
    pthread_mutex_init(&data->end_mutex, NULL);
    pthread_mutex_init(&data->print_mutex, NULL);
    i = 0;
    while (i < data->number_of_coders)
    {
        id = i + 1;
        data->coders[i].waiting = 0;
        data->coders[i].waiting_since = 0;
        data->coders[i].id = id;
        data->coders[i].data = data;
        data->coders[i].left = &data->dongles[i];
        data->coders[i].right = &data->dongles[id % data->number_of_coders];
        data->coders[i].finish_compile= 0;
        data->coders[i].last_compile = data->start_time;
        data->dongles[i].last_relase = data->start_time - data->dongle_cooldown;
        pthread_mutex_init(&data->coders[i].coder_mutex, NULL);
        i++;
    }

    int index = 0;
    pthread_mutex_init(&data->scheduler_mutex, NULL);
    pthread_cond_init(&data->scheduler_cond, NULL);
    while (index < data->number_of_coders){
        pthread_create(&threads[index], NULL, routine, &data->coders[index]);
        index++;
    }
    pthread_create(&monitor_thread, NULL, monitor, data);
    index = 0;
    while(index < data->number_of_coders){
        pthread_join(threads[index], NULL);
        index++;
    }
    pthread_join(monitor_thread, NULL); 

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
    return (0);
}