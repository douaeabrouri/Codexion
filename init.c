/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: doabrour <doabrour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/21 17:04:03 by doabrour          #+#    #+#             */
/*   Updated: 2026/08/22 19:31:52 by doabrour         ###   ########.fr       */
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

    coder = (t_coder *)arg;
    while(!is_simulation_over(coder->data)){
        if(!coders_is_finished(coder))
            break ;
        if(!wait_for_scheduler(coder))
            break ;
		if (one_coder(coder))
			return (NULL);
        take_two_dongles(coder);
        handle_cooldown(coder);
        if (is_simulation_over(coder->data))
        {
            pthread_mutex_unlock(&coder->left->mutex);
            pthread_mutex_unlock(&coder->right->mutex);
            break;
        }
        compile(coder);
        release_dongle(coder);
        if (is_simulation_over(coder->data))
            break;
        debug(coder);
        refactor(coder);
    }
    return (NULL);
}
