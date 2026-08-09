
#ifndef CODEXION_H
# define CODEXION_H

#include <pthread.h>
#include <string.h>

typedef struct s_dongle{

    int id;
    pthread_mutex_t mutex;
    long  cooldown;

} t_dongle;



typedef struct s_coder{

    int id;
    pthread_t thread;
    int compile_time;
    t_dongle *right;
    t_dongle *left;

} t_coder;

typedef struct s_data{

    int number_of_coders;
    long time_to_burnout;
    long time_to_compile;
    long time_to_debug;
    long time_to_refactor;
    long number_of_compiles_required;
    int scheduler;
    int dongle_cooldown;
    
    t_coder *coders;
    t_dongle *dongle;
} t_data;


typedef struct s_simulation{
    
    t_data data;
    t_coder *coder;
    t_dongle *dongle;

} s_simulation;


t_data *parsing(char **lst, int n_arg);

#endif