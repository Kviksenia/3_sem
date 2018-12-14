#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/msg.h>


int N;
pthread_t* tid;
float* res;
int NUM = 0;
int c = 0;
sem_t* sem;
int max = 500000000;
#define BILLION 1000000000L;

void* thr_stuff(void *arg);

int main(void){
    printf("Enter the number of streams:\n");
    scanf("%d", &N);
    tid = malloc(sizeof(pthread_t)*N);
    res = malloc(sizeof(float)*N);
    int i = 0;
    NUM = 0;
    int err;
    
    struct timespec start, stop;
    double accum;
    
    if( clock_gettime( CLOCK_MONOTONIC, &start) == -1 ) {
        perror( "clock gettime" );
        return EXIT_FAILURE;
    }
    
    while(i < N) {
        err = pthread_create(&(tid[i]), NULL, &thr_stuff, NULL);
        if (err != 0){
            printf("\ncan't create thread :[%s]", strerror(err));
        }
        else{
            printf("\n Thread %d created successfully with tid=%lu\n", i, (unsigned long)tid[i]);
        }
        i++;
    }
    sem = sem_open("NAME",O_CREAT,0776,0);
    sem_wait(sem);
    float av = 0;
    for(i=0;i<N;i++){
        av = av + res[i];
    }
    av = av/N;
    printf("integrate(xdx),[0;2]=%f\n", av);
    
    if( clock_gettime( CLOCK_MONOTONIC, &stop) == -1 ) {
        perror( "clock gettime" );
        return EXIT_FAILURE;
    }
    
    accum = ( stop.tv_sec - start.tv_sec )
    + (double)( stop.tv_nsec - start.tv_nsec )
    /                (double)BILLION;
    printf( "time=%lf seconds\n", accum );
    free(res);
    free(tid);
    return 0;
}

void* thr_stuff(void *arg)
{
    int i = 0;
    int j = 0;
    float integ = 0;
    int n = 0;
    float x, y, val;
    sem = sem_open("NAME",0);
    if(NUM<N){
        n = NUM;
        NUM++;
    }
    else
    {
        printf("Error in thr_stuff\n");
        return NULL;
    }
    int b = max/N;
    for(i=0;i<b;i++){
        x = (float)rand()*2/RAND_MAX;
        y = (float)rand()*4/RAND_MAX;
        val = x ;
        if(y<=val){
            j++;
        }
    }
    float del = 1.0/max;
    integ = j*del*8*N;
    res[n]=integ;
    c++;
    if(c==N){
        sem_post(sem);
    }
    return NULL;
}
