
#include "codexion.h"

int check_the_input(char *str, int loop){

    size_t index = 0;
    size_t len = strlen(str);
    long value = 0;
    
    if (len == 0){
        printf("Invalid empty value in position %d\n", loop);
        return (-1);
    }
    while (index < len){
        if (str[index] >= '0' && str[index] <= '9'){
            value = value * 10 + (str[index] - '0');
            if (value > INT_MAX){
                printf("%s\n", "Invalid Value ");
                return (-1);
            }
            index++;
        }
        else{
            printf("Invalide value -> %c in position %d\n", str[index], loop);
            return (-1);
        }
    }
    return ((int)value);
}

t_data *parsing(char **lst, int n_arg){
    
    int loop = 1;
    size_t index = 0;
    t_data *data;

    data = malloc(sizeof(t_data));

    if (data == NULL)
        return (NULL);
    
    if (n_arg != 9){
        fprintf(stderr, "The number of arguments must be 9 not %d\n", n_arg);
        free(data);
        return NULL;
    }

    while (index < strlen(lst[n_arg - 1])){
        if (lst[n_arg - 1][index] >= 'A' && lst[n_arg - 1][index] <= 'Z')
            lst[n_arg - 1][index] += 32;
        index++;
    }

    if (strcmp(lst[n_arg - 1], "fifo") != 0 && strcmp(lst[n_arg - 1], "edf") != 0){
        fprintf(stderr, "The last argument must be 'fifo' or 'edf'\n");
        free(data);
        return NULL;
    }

    while (loop <= n_arg - 2){
        int value = check_the_input(lst[loop], loop);
        if (value < 0){
           free(data);
           return (NULL);
        }
        if (loop == 1)
            data -> number_of_coders = value;
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
            data -> dongle_cooldown = value;
        loop++;
    }
    if(strcmp(lst[n_arg - 1], "fifo") == 0)
        data->scheduler = 0;
    else if(strcmp(lst[n_arg - 1], "edf") == 0)
        data->scheduler = 1;

    return (data);
}
