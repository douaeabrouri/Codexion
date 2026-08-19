#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int value = 0;

void *typing1()
{
    value++;
}

void *typing(void *arg)
{
    printf("THREAD 2 is working too \n");
    sleep(3);
    printf("ending ...\n");
    return NULL;
}

int main(int argc, char **argv)
{
    int i = 0;
    if (argc != 1){
        printf("");
    }
    return 0;
}

// int pthread_create(
//     pthread *thread,  //Id of thread
//     const pthread_attrd_t *attr, // the pthread_attrd_t containt the settings of athread like the stack size ...
//     void *(*start_routine)(* void), //function that u want ot work 
//     void *arg // argument libari t3ti lthread
// );

