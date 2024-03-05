//创建两个线程，偶数时进程1给变量加1，奇数时进程2给变量加1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_COUNT 9

pthread_mutex_t mutex;
pthread_cond_t cond;
int count=0;

void addCount_odd_Func(void)
{
    pthread_mutex_lock(&mutex);
    while(count<MAX_COUNT){
        if(count%2==1){
            count++;
            printf("addcout_odd_Func():count=%d.\n",count);
            pthread_cond_signal(&cond);
        }
        else
            pthread_cond_wait(&cond,&mutex);
    }
    pthread_mutex_unlock(&mutex);
}

void addCount_even_Func(void)
{
    pthread_mutex_lock(&mutex);
    while(count<MAX_COUNT){
        if(count%2==0){
            count++;
            printf("addCount_even_Func:count=%d.\n",count);
            pthread_cond_signal(&cond);
        }
        else
            pthread_cond_wait(&cond,&mutex);
    }
    pthread_mutex_unlock(&mutex);
}

int main(int argc, char **argv)
{
    int ret;
    pthread_t odd_thread,even_thread;
    pthread_attr_t thread_attr;
    count=0;

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    ret=pthread_attr_init(&thread_attr);
    if(ret!=0){
        perror("attribute creation failed!");
        exit(EXIT_FAILURE);
    }

    pthread_attr_setdetachstate(&thread_attr,PTHREAD_CREATE_DETACHED);

    ret=pthread_create(&odd_thread,&thread_attr,(void*)&addCount_odd_Func,NULL);
    if(ret!=0){
        perror("thread creation failed!");
        exit(EXIT_FAILURE);
    }

    ret=pthread_create(&even_thread,&thread_attr,(void*)&addCount_even_Func,NULL);
    if(ret!=0){
        perror("thread creation failed!");
        exit(EXIT_FAILURE);
    }

    while(count<MAX_COUNT);

    printf("Finished!");
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);

    return 0;
}