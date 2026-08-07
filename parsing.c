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



int main(int argc, char ** argv){
    
    int loop = 1;
    int index = 0;

    if (argc != 9){
        printf("The number of arguments must be 9 not %d\n", argc);
        return (1);
    }

    while (index < strlen(argv[argc - 1])){
        if (argv[argc - 1][index] >= 'A' && argv[argc - 1][index] <= 'Z')
            argv[argc - 1][index] += 32;
            // printf(argv[argc - 1][index])
        index++;
    }

    if (strcmp(argv[argc - 1], "fifo") != 0 && strcmp(argv[argc - 1], "edf") != 0){
        printf("The last argument must be 'fifo' or 'edf'\n");
        return (1);
    }

    while (loop <= argc - 2){
        check_the_output(argv[loop], loop);
        loop++;
    }
}

