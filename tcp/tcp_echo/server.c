#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

int echo(int fd);
int loop_echo(int fd);

void handleChildSignal(int n) {
    wait(NULL);
    return;
}

void handleInteruptSignal(int n) {
    printf("receive interupt signal");
    wait(NULL);
    exit(0);
}

int main(int argc, char *argv[]) {
    signal(SIGCHLD, handleChildSignal);
    signal(SIGINT, handleInteruptSignal);
    char ip[16];
    char buf[128];
    int s_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (s_fd == -1) {
        printf("创建 socket 失败！！！");
        return 1;
    }

    struct sockaddr_in serv;
    struct sockaddr_in clie;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(8080);
    serv.sin_addr.s_addr = htonl(INADDR_ANY);

    int b = bind(s_fd, (const struct sockaddr *)&serv, sizeof(serv));

    if (b == -1) {
        perror("bind address error");
        return 1;
    }
    // listen client connection
    listen(s_fd, 5);
    while (1) {
        unsigned int clie_addr_len = sizeof(clie);
        // get connection from queue
        int c_fd = accept(s_fd, (struct sockaddr *)&clie, &clie_addr_len);
        if (c_fd == -1) {
            perror("get connect error:");
            continue;
        }
        inet_ntop(AF_INET, &clie.sin_addr, ip, 32);
        printf("receive connection: %s\n", ip);
        // 创建子进程
        pid_t pid = fork();
        if (pid == -1) {
            close(c_fd);
            perror("create child process error");
        }else if (pid == 0) {
            loop_echo(c_fd);
            close(c_fd);
            close(s_fd);
            exit(0);
        }else {
            close(c_fd);
            printf("create child success, pid: %d\n", pid);
            waitpid(-1, NULL, WNOHANG);
        }
    }
    close(s_fd);
    return 0;
}

int echo(int fd) {
    char buf[128];
    int n = read(fd, buf, 128);
    if (n == -1) {
        perror("read from conn error");
        return -1;
    }
    n = write(fd, buf, n);
    if (n == -1) {
        perror("write socket error");
        return -1;
    }
    return 0;
}

int loop_echo(int fd) {
    char buf[128];
    while (1) {
        int n = read(fd, buf, 128);
        if (n == 0) {
            printf("Peer closed connection.\n");
            return 0;
        }else if (n == -1) {
            perror("read from conn error");
            return -1;
        }
        printf("server receive data: \n");
        write(1, buf, n);
        printf("\n");
        n = write(fd, buf, n);
        if (n == 0) {
            printf("Peer closed connection.\n");
            return 0;
        }else if (n == -1) {
            perror("write socket error");
            return -1;
        }
        if (strncmp(buf, "exit", 4) == 0 ){
            printf("close connection and exit.\n");
            break;
        }
    }
    return 0;
}