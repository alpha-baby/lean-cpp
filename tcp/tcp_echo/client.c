#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

int send_and_receive(int fd);

int main(int argc, char *argv[]) {
    struct sockaddr_in serv;
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("new socket error");
        return 1;
    }
    serv.sin_family = AF_INET;
    // serv.sin_addr.s_addr = htonl("127.0.0.1");
    inet_pton(AF_INET, argv[1], &serv.sin_addr);
    serv.sin_port = htons(8080);

    int c = connect(fd, (struct sockaddr *)&serv, sizeof(serv));
    if (c == -1) {
        perror("connect to remote error");
        return 2;
    }
    printf("connect success\n");
    send_and_receive(fd);
    close(fd);
    return 3;
}

int send_and_receive(int fd) {
    char buf[128];
    while (1) {
        fgets(buf, 128, stdin);
        int bytes_written = write(fd, buf, strlen(buf) + 1);
        if (bytes_written == 0) {
            printf("Peer closed connection.\n");
            return 0;
        } else if (bytes_written == -1) {
            perror("write to socket error");
            close(fd);
            return 4;
        }
        int n = read(fd, buf, 128);
        if (n == 0) {
            printf("Peer closed connection.\n");
            return 0;
        } else if (n == -1) {
            perror("read from socket error");
            close(fd);
            return 5;
        }
        write(1, buf, n);
    }
    return 0;
}