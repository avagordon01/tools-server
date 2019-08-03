#include <stdio.h>
#include <uv.h>

#include "server-internal.hh"
#include "server-external.hh"

int main() {
    loop = uv_default_loop();

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    struct sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", 19000, &addr);
    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    int r = uv_listen((uv_stream_t*)&server, 128, on_new_toolstream_connection);
    if (r) {
        fprintf(stderr, "listen error %s\n", uv_strerror(r));
        return 1;
    }

    uv_ip4_addr("0.0.0.0", 19001, &addr);
    uv_tcp_bind(&server, (const struct sockaddr*)&addr, 0);
    uv_listen((uv_stream_t*)&server, 128, on_new_user_connection);

    create_lws_context_with_foreign_loop(loop);
    if (!context) {
        return 1;
    }

    return uv_run(loop, UV_RUN_DEFAULT);
}
