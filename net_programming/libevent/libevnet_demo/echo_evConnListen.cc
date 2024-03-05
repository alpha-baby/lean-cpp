#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "event.h"
#include "event2/listener.h"

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
    std::string s(readBuffer, readBuffer + n);
    std::cout << "Receive msg length: " << n << " , message: " << s
              << std::endl;
}

void listener_cb_fn(struct evconnlistener *listener, evutil_socket_t clientFD,
                    struct sockaddr *addr, int socklen, void *arg) {
    std::cout << "fd: " << clientFD << std::endl;
    struct sockaddr_in *remoteAddr = (struct sockaddr_in *)addr;
    struct event *evTcpEcho = NULL;
    struct event_base *base = NULL;

    std::cout << "new connection: " << inet_ntoa(remoteAddr->sin_addr) << ":"
              << ntohs(remoteAddr->sin_port) << " ,FD: " << clientFD
              << std::endl;
    base = (struct event_base *)arg;
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
    struct sockaddr_in localAddr;
    localAddr.sin_family = AF_INET;
    localAddr.sin_port = htons(9910);
    localAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    struct evconnlistener *evconnlistener = evconnlistener_new_bind(
        base, listener_cb_fn, (void *)(base),
        LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE, 64,
        (struct sockaddr *)&localAddr, sizeof(localAddr));
    if (!evconnlistener) {
        std::cout << "create event listener error" << std::endl;
        event_base_free(base);
        return 1;
    }
    // event_base_dispatch(base);  // 启动事件分发
    event_base_loop(base, 0);
    evconnlistener_free(evconnlistener);
    event_base_free(base);
    return 0;
}