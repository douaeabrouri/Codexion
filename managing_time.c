#include "codexion.h"

long get_time_ms(void)
{
    struct timeval tv; // struct that store the time that system give it to us.
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000L) + (tv.tv_usec / 1000L);
}

void ft_usleep(long ms){
    usleep(ms * 1000);
}