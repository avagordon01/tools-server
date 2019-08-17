#include <stdio.h>
#include <uv.h>

#include <deque>
#include <vector>
#include <stdint.h>

enum class buf_type {
    text, binary,
};
struct out_buf {
    std::vector<uint8_t> data;
    buf_type type;
};
struct in_buf {
    std::vector<uint8_t> data;
    size_t sent;
};
std::deque<struct out_buf> out_bufs {};
std::deque<struct in_buf> in_bufs {};

#include "server-internal.hh"
#include "server-external.hh"

int main() {
    loop = uv_default_loop();

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    struct sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", 1025, &addr);
    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*)&server, 128, on_new_toolstream_connection);
    if (r) {
        fprintf(stderr, "listen error %s\n", uv_strerror(r));
        return 1;
    }

    uv_prepare_t prepare;
    uv_prepare_init(loop, &prepare);
    uv_prepare_start(&prepare, on_prepare);

    create_lws_context_with_foreign_loop(loop);
    if (!context) {
        return 1;
    }

    return uv_run(loop, UV_RUN_DEFAULT);
}
