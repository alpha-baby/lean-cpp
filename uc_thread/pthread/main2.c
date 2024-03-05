#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void *handle(void *arg) {
    printf("handle running...\n");
    sleep(3);
    return (void *)1;
}

void *handle1(void *arg) {
    printf("handle1 running...\n");
    sleep(3);
    pthread_exit((void*)2);
    return (void *)4;
}

void *handle2(void *arg) {
    while (1) {
        printf("handle2 running...\n");
            sleep(1);
    }
   
    return (void *)5;
}

int main() {
    void *retval;
    pthread_t tid;
    // 创建一个新线程
    pthread_create(&tid, NULL, handle, NULL);
    // 汇合线程, 阻塞等待 tid 这个线程结束
    pthread_join(tid, &retval); // 线程退出状态码存放到 retval
    printf("handle thread exit code: %d\n", (int)retval);

    pthread_create(&tid,NULL,handle1,NULL);
    pthread_join(tid, &retval);
    printf("handle1 thread exit code: %d\n", (int)retval);

    pthread_create(&tid, NULL, handle2, NULL);
    sleep(3);
    pthread_cancel(tid);
    pthread_join(tid, &retval);
    int    returnval = (int)retval;
    if ( retval == PTHREAD_CANCELED ) {
        printf("thread be canceled\n");
    }
    printf("handle2 thread exit code: %d\n", (int)retval);
    return 0;
}