
#ifndef CODEXION_H
#define CODEXION_H

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <sys/time.h>


typedef struct s_data t_data;

typedef struct s_dongle
{
    int id;
    long last_relase;
    pthread_mutex_t mutex;
} t_dongle;

typedef struct s_coder
{
    int id;
    t_dongle *right;
    t_dongle *left;
    t_data *data;
    long last_compile;
    int finish_compile;

} t_coder;

typedef struct s_data
{
    int number_of_coders;
    long time_to_burnout;
    long time_to_compile;
    long time_to_debug;
    long time_to_refactor;
    int number_of_compiles_required;
    long start_time;
    int scheduler;

    int simulation_end;
    int dongle_cooldown;

    t_dongle *dongles;
    t_coder *coders;

} t_data;

t_data *parsing(char **lst, int n_arg);
long get_time_ms(void);
void ft_usleep(long ms);
void *monitor(void *arg);
void *routine(void *arg);

#endif