#include <netinet/in.h>
#include <sys/socket.h>

#include <cstring>
#include <iostream>

#include "event.h"
#include "event2/buffer.h"
#include "event2/bufferevent.h"
#include "event2/listener.h"

struct sockaddr_in proxyListenerAddr;
struct sockaddr_in serverAddr;

void proxyListener_cb(struct evconnlistener *, evutil_socket_t,
                      struct sockaddr *, int socklen, void *);
void transfer_data(struct bufferevent *from, struct bufferevent *to);
void downstream_read_cb(struct bufferevent *bev, void *ctx);
void upstream_read_cb(struct bufferevent *bev, void *ctx);
void transfer_write_cb(struct bufferevent *bev, void *ctx);
void transfer_event_cb(struct bufferevent *from, struct bufferevent *to,
                       short what);
void downstream_event_cb(struct bufferevent *bev, short what, void *ctx);
void upstream_event_cb(struct bufferevent *bev, short what, void *ctx);

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << argv[0] << "proxyIP:proxyPort, ServerIP:ServerPort"
                  << std::endl;
        return 1;
    }

    memset(&proxyListenerAddr, 0, sizeof(proxyListenerAddr));
    int socklen = sizeof(proxyListenerAddr);
    int ret = evutil_parse_sockaddr_port(
        argv[1], (struct sockaddr *)&proxyListenerAddr, &socklen);
    if (ret < 0) {
        std::cerr << "parse proxyIP and proxy Port error" << std::endl;
        return 2;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    socklen = sizeof(serverAddr);
    ret = evutil_parse_sockaddr_port(argv[2], (struct sockaddr *)&serverAddr,
                                     &socklen);
    if (ret < 0) {
        std::cerr << "parse serverIP and server Port error" << std::endl;
        return 3;
    }

    struct event_base *base = event_base_new();
    if (!base) {
        std::cerr << "new event base error" << std::endl;
        return 4;
    }
    struct evconnlistener *proxyListener = evconnlistener_new_bind(
        base, proxyListener_cb, (void *)base, LEV_OPT_CLOSE_ON_FREE, 64,
        (struct sockaddr *)&proxyListenerAddr, sizeof(proxyListenerAddr));
    if (!proxyListener) {
        event_base_free(base);
        std::cerr << "new proxy listener error, " << argv[1] << std::endl;
        return 5;
    }
    std::cout << "new proxy listener success, " << argv[1] << std::endl;

    ret = event_base_dispatch(base);
    if (!ret) {
        event_base_free(base);
        std::cerr << "event base dispath error, code: " << ret << std::endl;
        return 6;
    }
    evconnlistener_free(proxyListener);
    event_base_free(base);
    std::cerr << "exit " << std::endl;
    return 0;
}

void proxyListener_cb(struct evconnlistener *proxyListener,
                      evutil_socket_t clientFD, struct sockaddr *clientAddr,
                      int socklen, void *args) {
    struct sockaddr_in *remoteClientAddr = (struct sockaddr_in *)clientAddr;
    char remoteIP[40];
    memset(&remoteIP, 0, sizeof(remoteIP));
    evutil_inet_ntop(AF_INET, &remoteClientAddr->sin_addr, remoteIP,
                                  sizeof(remoteIP));

    struct event_base *base = evconnlistener_get_base(proxyListener);
    std::cout << "new establish conn, "
              << remoteIP
              << ":" << ntohs(remoteClientAddr->sin_port) << std::endl;
    struct bufferevent *downstreamEvBuffer = bufferevent_socket_new(
        base, clientFD, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
    struct bufferevent *upstreamEvBuffer = bufferevent_socket_new(
        base, -1, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_DEFER_CALLBACKS);
    int ret = bufferevent_socket_connect(
        upstreamEvBuffer, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (ret != 0) { // on error
        std::cerr << "conn upstream server, code: " << ret << std::endl;
        bufferevent_free(downstreamEvBuffer);
        bufferevent_free(upstreamEvBuffer);
        evconnlistener_free(proxyListener);
        return;
    }
    bufferevent_setcb(downstreamEvBuffer, downstream_read_cb, nullptr,
                      downstream_event_cb, upstreamEvBuffer);
    bufferevent_setcb(upstreamEvBuffer, upstream_read_cb, nullptr,
                      upstream_event_cb, downstreamEvBuffer);
    ret = bufferevent_enable(downstreamEvBuffer, EV_READ | EV_WRITE);
    if (ret != 0) {
        bufferevent_free(downstreamEvBuffer);
        bufferevent_free(upstreamEvBuffer);
        evconnlistener_free(proxyListener);
        std::cerr << "enable downstram buffer event READ and WRITE event error"
                  << ret << std::endl;
        return;
    }
    ret = bufferevent_enable(upstreamEvBuffer, EV_READ | EV_WRITE);
    if (ret != 0) {
        bufferevent_free(downstreamEvBuffer);
        bufferevent_free(upstreamEvBuffer);
        evconnlistener_free(proxyListener);
        std::cerr << "enable upstream buffer event READ and WRITE event error"
                  << ret << std::endl;
        return;
    }
}

void transfer_data(struct bufferevent *from, struct bufferevent *to) {
    std::cout << "transfer read data event cb" << std::endl;
    struct evbuffer *srcBuffer = bufferevent_get_input(from);
    struct evbuffer *dstBuffer = bufferevent_get_output(to);
    int ret = 0;
    if (evbuffer_get_length(srcBuffer) == 0) {
        return;
    }
    if (!to) {
        evbuffer_drain(srcBuffer, evbuffer_get_length(srcBuffer));
        return;
    }
    // evbuffer_add_buffer(srcBuffer, dstBuffer);  // TODO 方向是不是反了

    evbuffer_add_buffer(dstBuffer, srcBuffer);  // TODO 方向是不是反了

    return;
}
void downstream_read_cb(struct bufferevent *bev, void *ctx) {
    transfer_data(bev, (struct bufferevent *)ctx);
}
void upstream_read_cb(struct bufferevent *bev, void *ctx) {
    transfer_data(bev, (struct bufferevent *)ctx);
}

void transfer_write_cb(struct bufferevent *bev, void *ctx) {
    std::cout << "transfer write data event cb" << std::endl;
    struct evbuffer *output = bufferevent_get_output(bev);
    if (0 == evbuffer_get_length(output)) {
        bufferevent_free(bev);
    }
}

void transfer_event_cb(struct bufferevent *from, struct bufferevent *to,
                       short what) {
    std::cout << "transfer err event cb" << std::endl;
    if (what & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
        if (to) {
            transfer_data(from, to);
            if (evbuffer_get_length(bufferevent_get_input(from)) > 0) {
                bufferevent_setcb(to, nullptr, transfer_write_cb,
                                  downstream_event_cb, NULL);
                bufferevent_disable(to, EV_READ);
            } else {
                bufferevent_free(to);
            }
        }

        bufferevent_free(from);
    }

    return;
}
void downstream_event_cb(struct bufferevent *bev, short what, void *ctx) {
    std::cout << "transfer downstram data to upstram event cb" << std::endl;
    transfer_event_cb(bev, (struct bufferevent *)ctx, what);
}
void upstream_event_cb(struct bufferevent *bev, short what, void *ctx) {
    std::cout << "transfer upstream data to downstream event cb" << std::endl;
    transfer_event_cb(bev, (struct bufferevent *)ctx, what);
}