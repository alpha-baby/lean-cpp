#include <csignal>
#include <iostream>

#include "event.h"

int cout = 4;

void signal_cb(evutil_socket_t signal_t, short flags, void *arg_event) {
    std::cout << "receive signal: " << signal_t << std::endl;
    if (--cout == 0) {
        std::cout << "remove signal handler" << std::endl;
        struct event *sinal_event = (struct event *)arg_event;
        event_del(sinal_event);
        exit(0); // 不应该直接退出，应该先把 libevent 中的资源给全部释放完
    }
}

int add_handle_signal(struct event_base* base, int signal) {
    struct event *evSignal = nullptr;
    evSignal = evsignal_new(base, signal, signal_cb, event_self_cbarg());
    if (!evSignal) {
        std::cerr << "new signal event" << std::endl;
        return 2;
    }
    int ret = evsignal_add(evSignal, nullptr);
    if (ret != 0) {
        std::cerr << "add signal event error" << std::endl;
        return 3;
    }
    return 0;
}

int main() {
    struct event_base *base = nullptr;

    base = event_base_new();
    if (!base) {
        std::cerr << "new event base error" << std::endl;
        return 1;
    }

    int ret = add_handle_signal(base, SIGINT);
    if (ret != 0) {
        event_base_free(base);
        return 2;
    }

    ret = add_handle_signal(base, SIGTERM);
    if (ret != 0) {
        event_base_free(base);
        return 2;
    }
    
    ret = event_base_dispatch(base);
    if (ret != 0 && ret != 1) {
        event_base_free(base);
        std::cerr << "add signal event error code: " << ret << std::endl;
        return 3;
    }
    // TODO free all events
    event_base_free(base);
    return 0;
}