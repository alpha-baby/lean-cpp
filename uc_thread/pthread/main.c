#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void *doit(void *arg) {
    pid_t pid = getpid();
    pthread_t tid = pthread_self();
    for (int i = 0; i < 10; i++) {
        printf("create new thread: %s\tpid: %d\t tid: %lu \n", (char *)arg, pid,
               (unsigned long int)tid);
        sleep(1);
    }
    return NULL;
}

int main() {
    // create new thread
    pthread_t tid;
    pthread_create(&tid, NULL, doit, "new");
    // 分离新线程
    pthread_detach(tid);
    
    doit("main-thread.");
    // 到这里有两个线程了，这两个线程目前是异步的
    sleep(1);
    
    return 0;
}