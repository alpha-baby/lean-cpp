#include <csignal>
#include <iostream>

#include "event.h"


void timeout_cb(evutil_socket_t signal_t, short flags, void *arg_event) {
    std::cout << "receive timeout handle: " << flags << std::endl;
}

int add_timeout_handle(struct event_base *base, int seconds) {
    struct event *evTimeout = nullptr;
    struct timeval tv;
    evutil_timerclear(&tv);

    evTimeout = event_new(base, -1, EV_PERSIST | EV_TIMEOUT, timeout_cb,
                          event_self_cbarg());
    if (!evTimeout) {
        std::cerr << "new signal event" << std::endl;
        return 2;
    }
    tv.tv_sec = seconds;
    int ret = event_add(evTimeout, &tv);
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

    int ret = add_timeout_handle(base, 3);
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