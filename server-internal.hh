#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <uv.h>
#include <memory>
#include <assert.h>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <libwebsockets.h>

#include "protocol-internal.hh"

uv_loop_t *loop;

enum class connection_state {
    starting,
    reading_connect_message,
    reading_data,
};

struct harness_tcp_t {
    uv_tcp_t base;
    connect_message m;
    connection_state s;
};

std::vector<harness_tcp_t> toolstreams;

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = (char*)malloc(suggested_size);
    buf->len = suggested_size;
}

void on_write(uv_write_t *req, int status) {
    if (status) {
        fprintf(stderr, "write error %s\n", uv_strerror(status));
    }
    free(req);
}

void on_prepare(uv_prepare_t* prepare) {
    while (!in_bufs.empty()) {
        for (auto toolstream: toolstreams) {
            uv_stream_t* client = (uv_stream_t*)&toolstream;

            uv_buf_t uv_b = uv_buf_init("bt\n", 3);
            uv_write_t *req = (uv_write_t*)malloc(sizeof(uv_write_t));
            req->handle = client;
            uv_write(req, client, &uv_b, 1, on_write);
        }
        in_bufs.pop_back();
    }
}

void on_read(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread < 0) {
        if (nread != UV_EOF) {
            fprintf(stderr, "read error %s\n", uv_err_name(nread));
            uv_close((uv_handle_t*)client, nullptr);
        }
    } else if (nread > 0) {
        bool done = false;
        harness_tcp_t* harness = (harness_tcp_t*)client;
        switch (harness->s) {
            while (!done) {
                case connection_state::reading_connect_message:
                    {
                        if (nread >= sizeof(connect_message)) {
                            harness->m = *((connect_message*)buf->base);
                            assert(harness->m.token0 == good_token);
                            assert(harness->m.token1 == good_token);
                            harness->s = connection_state::reading_data;
                        } else {
                            fprintf(stderr, "bleh, have to handle small partial packets\n");
                            assert(false);
                            exit(1);
                        }
                    }
                    break;
                case connection_state::reading_data:
                    {
                        if (true) {
                            json j;
                            j["tool"] = harness->m.tool_id;
                            j["stream"] = harness->m.stream_id;
                            j["pid"] = harness->m.pid;
                            std::string js = j.dump();
                            out_bufs.push_back({{js.begin(), js.end()}, buf_type::text});
                            std::vector<uint8_t> b;
                            b.assign(buf->base, buf->base + nread);
                            out_bufs.push_back({b, buf_type::binary});
                        }
                    }
                    break;
            }
        }
    }

    if (buf->base) {
        free(buf->base);
    }
}

void on_new_toolstream_connection(uv_stream_t *server, int status) {
    if (status < 0) {
        fprintf(stderr, "new connection error %s\n", uv_strerror(status));
        return;
    }

    toolstreams.push_back({});
    harness_tcp_t *client = &toolstreams.back();
    client->s = connection_state::starting;
    uv_tcp_init(loop, (uv_tcp_t*)client);
    if (uv_accept(server, (uv_stream_t*)client) == 0) {
        uv_read_start((uv_stream_t*)client, alloc_buffer, on_read);
        client->s = connection_state::reading_connect_message;
    } else {
        uv_close((uv_handle_t*)client, nullptr);
    }
}
