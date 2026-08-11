#include <pthread.h>
#include <stdio.h>

pthread_mutex_t mutex;
int counter = 0;

void *routine(void *arg){
    int i = 0;

    while(i < 100000000){
        pthread_mutex_lock(&mutex);
        // mutex++;
        counter++;
        pthread_mutex_unlock(&mutex);
        i++;
    }
    printf("mutex = %d\n", counter);
    return NULL;
}

int main(){
    pthread_t thread;

    pthread_create(&thread, NULL, &routine, NULL);
    pthread_join(thread, NULL);   
}