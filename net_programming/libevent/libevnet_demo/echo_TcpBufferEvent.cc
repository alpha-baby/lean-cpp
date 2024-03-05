#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>

#include "event.h"
#include "event2/bufferevent.h"
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

void buf_ev_read_cb(struct bufferevent *bev, void *ctx) {
    std::cout << "calling buf_ev_read_cb" << std::endl;
    char tmp;  // 为了演示 才使用的一个长度
    struct evbuffer *inputBuffer = bufferevent_get_input(bev);
    struct evbuffer *outputBuffer = bufferevent_get_output(bev);
    // while (evbuffer_get_length(inputBuffer) != 0) {
    //     bufferevent_read(bev, &tmp, 1);
    //     bufferevent_write(bev, &tmp, 1);
    // }
    // 优化：
    if (evbuffer_get_length(inputBuffer) != 0) {
        int msgLength = evbuffer_get_length(inputBuffer);
        unsigned char *bufferContent = evbuffer_pullup(inputBuffer, msgLength);
        if (bufferContent != nullptr) {
            std::string s(bufferContent, bufferContent + msgLength);
            std::cout << "Receive msg length: " << msgLength
                      << " , message: " << s << std::endl;
        }
        bufferevent_write_buffer(bev, inputBuffer);
    }
    if (evbuffer_get_length(outputBuffer) != 0) {
        std::cout << "handle read cb, enbale write and disable read" <<  std::endl;
        bufferevent_enable(bev, EV_WRITE);
        bufferevent_disable(bev, EV_READ);
    }
    return;
}

// 如果要启动写回调，需要开启 ，开启方式：bufferevent_enable(bufev, EV_WRITE);
void buf_ev_write_cb(struct bufferevent *bev, void *ctx) {
    std::cout << "calling buf_ev_write_cb" << std::endl;
    struct evbuffer *outputBuffer = bufferevent_get_output(bev);
    
    if (evbuffer_get_length(outputBuffer) == 0) {
        std::cout << "handle write cb, enbale read and disable write" <<  std::endl;
        bufferevent_disable(bev, EV_WRITE);
        bufferevent_enable(bev, EV_READ);
    }
}
void buf_ev_cb(struct bufferevent *bev, short what, void *ctx) {
    std::cout << "Receive event what: " << what << std::endl;
    if (what & BEV_EVENT_EOF) {
        std::cout << "连接关闭" << std::endl;
    } else if (what & BEV_EVENT_ERROR) {
        std::cout << "BEV_EVENT_ERROR" << std::endl;
    } else if (what & BEV_EVENT_TIMEOUT) {
        std::cout << "BEV_EVENT_TIMEOUT" << std::endl;
    } else if (what & BEV_EVENT_WRITING) {
        std::cout << "BEV_EVENT_WRITING" << std::endl;
    } else if (what & BEV_EVENT_READING) {
        std::cout << "BEV_EVENT_READING" << std::endl;
    }
    // 因为前面设置过 BEV_OPT_CLOSE_ON_FREE，那么在释放的时候自动关闭 fd
    bufferevent_free(bev);
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

    struct bufferevent *bufev =
        bufferevent_socket_new(base, clientFD, BEV_OPT_CLOSE_ON_FREE);
    if (!bufev) {
        std::cout << "new buffer event error" << std::endl;
        return;
    }
    bufferevent_setcb(bufev, buf_ev_read_cb, buf_ev_write_cb, buf_ev_cb,
                      event_self_cbarg());
    bufferevent_enable(bufev, EV_READ);  // 只启动读事件
    bufferevent_disable(bufev, EV_WRITE);  // 关闭写事件,
    // 如果关闭后就不能给客户端发送数据了
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
    event_base_dispatch(base);  // 启动事件分发
    evconnlistener_free(evconnlistener);
    event_base_free(base);
    return 0;
}