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

int main()
{
    int i = 0;

    pthread_t thread1, thread2; // the type of pthread_t  (unsigned long)
    pthread_create(&thread1, NULL, typing1, NULL);
    pthread_create(&thread2, NULL, typing1, NULL);
    printf("Waiting ...\n");
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("Main continues");
    printf("value = %d", value);
    return 0;
}

// int pthread_create(
//     pthread *thread,  //Id of thread
//     const pthread_attrd_t *attr, // the pthread_attrd_t containt the settings of athread like the stack size ...
//     void *(*start_routine)(* void), //function that u want ot work 
//     void *arg // argument libari t3ti lthread
// );

