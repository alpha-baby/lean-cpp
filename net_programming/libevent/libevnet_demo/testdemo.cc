#include <event2/event.h>
#include <event2/util.h>

#include <iostream>

using namespace std;

int main() {
    cout << "Libevnet version: " << event_get_version() << endl;
    cout << "Libevent I/O mutiplexing are  ,当前平台支持哪些 io 多路复用方法：" << endl;
    const char** method = event_get_supported_methods();
    int i;
    for (int i = 0; method[i] != NULL; i++) {
        cout << method[i] << endl;
    }
    struct event_base *base = NULL;
    struct event_config *cfg = NULL;
    cfg = event_config_new();
    if (!cfg) {
        cout << "new event config error" << endl;
        return 1;
    }
    event_config_avoid_method(cfg, "kqueue"); // mac 平台，去除某个方法的支持
    event_config_avoid_method(cfg, "poll"); // mac 平台，去除某个方法的支持

    base = event_base_new_with_config(cfg);
    event_config_free(cfg); // cfg 用不到了就可以立即释放
    if (!base) {
        cout << "new event base with config error" << endl;
        return 1;
    }
    cout << "current method: " << event_base_get_method(base) << endl;
    event_base_free(base);
    return 0;
}