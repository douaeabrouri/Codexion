#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

int check_the_output(char *str, int loop)
{

    int value = 0;
    int index = 0;
    int len = strlen(str);
    
    value = atoi(str);
    if (value > INT_MAX){
        printf("Invalid Value ")
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

}

char **parsing(char ** lst, int n_arg){
    
    int loop = 1;
    int index = 0;

    if (n_arg != 9){
        printf("The number of arguments must be 9 not %d\n", argc);
        return (1);
    }

    while (index < strlen(lst[n_arg - 1])){
        if (lst[n_arg - 1][index] >= 'A' && lst[n_arg - 1][index] <= 'Z')
            lst[n_arg - 1][index] += 32;
            // printf(lst[n_arg - 1][index])
        index++;
    }

    if (strcmp(lst[n_arg - 1], "fifo") != 0 && strcmp(lst[n_arg - 1], "edf") != 0){
        printf("The last argument must be 'fifo' or 'edf'\n");
        return (1);
    }

    while (loop <= n_arg - 2){
        check_the_output(lst[loop], loop);
        loop++;
    }
}
