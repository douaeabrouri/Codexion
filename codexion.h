
#ifndef CODEXION_H
#define CODEXION_H

#include <pthread.h>
#include <string.h>

typedef struct s_data t_data;

typedef struct s_dongle
{
    int id;
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
    long number_of_compiles_required;
    int scheduler;
    int dongle_cooldown;
    t_dongle *dongles;
} t_data;

t_data *parsing(char **lst, int n_arg);

#endif