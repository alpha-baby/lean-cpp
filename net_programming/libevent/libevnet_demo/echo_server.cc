#include <arpa/inet.h>
#include <event2/event.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

int createListenFD(char *addr, int port) {
    int listenFD = -1;
    listenFD = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(port);
    localAddr.sin_addr.s_addr = inet_addr(addr);
    int ret = bind(listenFD, (struct sockaddr *)&localAddr, sizeof(localAddr));
    if (ret < 0) {
        close(listenFD);
        return -1;
    }

    ret = listen(listenFD, 64);
    if (ret < 0) {
        close(listenFD);
        return -1;
    }
    return listenFD;
}

void tcpEchoCB(evutil_socket_t fd, short event, void *arg) {
    const int buffer_size = 1024;
    char readBuffer[buffer_size];

    int n = read(fd, readBuffer, buffer_size);
    if (n < 0) {
        return;
    } else if (n == 0) {
        close(fd);
        event_del((struct event *)arg);
        std::cout << "Connection FD: " << fd << " close" << std::endl;
        return;
    }
    write(fd, readBuffer, n);
    std::string s(readBuffer, readBuffer+n);
    std::cout << "Receive msg length: " << n << " , message: " << s << std::endl;
}

void tcpConnReqCB(evutil_socket_t fd, short event, void *arg) {
    std::cout << "fd: " << fd << std::endl;
    int clientFD;
    struct sockaddr_in remoteAddr;
    struct event *evTcpEcho = NULL;
    struct event_base *base = NULL;
    socklen_t addrLen = sizeof(remoteAddr);
    clientFD = accept(fd, (struct sockaddr *)&remoteAddr, &addrLen);
    if (clientFD < 0) {
        std::cout << "accept listenFD: " << fd << "error" << std::endl;
        return;
    }
    std::cout << "new connection: " << inet_ntoa(remoteAddr.sin_addr) << ":"
              << ntohs(remoteAddr.sin_port) << " ,FD: " << clientFD << std::endl;
    base = event_get_base((struct event *)arg);
    // EV_READ 表示监听读事件
    // EV_PERSIST 表示持续监听事件，不然就只监听一次
    evTcpEcho = event_new(base, clientFD, EV_READ | EV_PERSIST, tcpEchoCB,
                           event_self_cbarg());
    event_add(evTcpEcho, NULL);
    return;
}

int main() {
    struct event_base *base = NULL;
    base = event_base_new();
    int listenFD = createListenFD("127.0.0.1", 9911);
    if (listenFD < 0) {
        std::cout << "listen port error" << std::endl;
        event_base_free(base);
        return -1;
    }
    std::cout << "listen success, fd: " << listenFD << std::endl;
    struct event *eventConnReq;
    eventConnReq = event_new(base, listenFD, EV_READ | EV_PERSIST, tcpConnReqCB,
                             event_self_cbarg());
    if (!eventConnReq) {
        std::cout << "create event error" << std::endl;
        close(listenFD);
        event_base_free(base);
        return -1;
    }
    event_add(eventConnReq, NULL);
    event_base_dispatch(base);  // 启动事件分发
    event_base_free(base);
    return 0;
}