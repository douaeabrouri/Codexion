#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include "codexion.h"

int check_the_input(char *str, int loop){

    int value = 0;
    int index = 0;
    int len = strlen(str);
    
    value = atoi(str);
    if (value > INT_MAX){
        printf("%s\n", "Invalid Value ");
    }
    if (value < 0){
        printf("The number must be positive %d in position %d\n", value, loop);
        return (0);
    }
    while (index < len){
        if (str[index] >= '0' && str[index] <= '9'){
            index++;
        }
        else{
            printf("Invalide value -> %c in position %d\n", str[index], loop);
            return (0);
        }
    }
    return value;
}

t_data *parsing(char **lst, int n_arg){
    
    int loop = 1;
    int index = 0;
    t_data *data;

    data = malloc(sizeof(t_data));

    if (data == NULL)
        return (NULL);
    
    if (n_arg != 9){
        fprintf(stderr, "The number of arguments must be 9 not %d\n", n_arg);
        return NULL;
    }

    while (index < strlen(lst[n_arg - 1])){
        if (lst[n_arg - 1][index] >= 'A' && lst[n_arg - 1][index] <= 'Z')
            lst[n_arg - 1][index] += 32;
        index++;
    }

    if (strcmp(lst[n_arg - 1], "fifo") != 0 && strcmp(lst[n_arg - 1], "edf") != 0){
        fprintf(stderr, "The last argument must be 'fifo' or 'edf'\n");
        return NULL;
    }

    while (loop <= n_arg - 2){
        int value = check_the_input(lst[loop], loop);
        if (value < 0)
           return (NULL);
        if (loop == 1)
        {
            data -> number_of_coders = value;
            // printf("value = %d\n", value);
        }
        else if (loop == 2)
            data -> time_to_burnout = value;
        else if (loop == 3)
            data -> time_to_compile = value;
        else if (loop == 4)
            data -> time_to_debug = value;
        else if (loop == 5)
            data -> time_to_refactor = value;
        else if (loop == 6)
            data -> number_of_compiles_required = value;
        else if (loop == 7)
            data -> scheduler = value;
        else if (loop == 8)
            data -> dongle_cooldown = value;
        // else
        //     return NULL;
        loop++;
    }
    return (data);
}

