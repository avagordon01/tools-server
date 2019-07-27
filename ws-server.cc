#include <libwebsockets.h>
#include <stdbool.h>

int main() {
    struct lws_http_mount mount = {
        .mount_next =            NULL,
        .mountpoint =            "/",
        .origin =                "./static",
        .def =                   "index.html",
        .protocol =              NULL,
        .cgienv =                NULL,
        .extra_mimetypes =       NULL,
        .interpret =             NULL,
        .cgi_timeout =           0,
        .cache_max_age =         0,
        .auth_mask =             0,
        .cache_reusable =        0,
        .cache_revalidate =      0,
        .cache_intermediaries =  0,
        .origin_protocol =       LWSMPRO_FILE,
        .mountpoint_len =        1,
        .basic_auth_login_file = NULL,
    };

    struct lws_context *context;
    struct lws_context_creation_info info = {};
    info.port = 9999;
    info.mounts = &mount;
    info.error_document_404 = "/404.html";
    info.pcontext = &context;
    info.options =
        LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE |
        LWS_SERVER_OPTION_ALLOW_LISTEN_SHARE |
        LWS_SERVER_OPTION_LIBUV;

    if (false) {
        info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
        info.ssl_cert_filepath = "localhost-100y.cert";
        info.ssl_private_key_filepath = "localhost-100y.key";
    }

    uv_loop_t loop_uv;
    uv_loop_init(&loop_uv);

    void *foreign_loops[1] = {&loop_uv};
    info.foreign_loops = foreign_loops;

    context = lws_create_context(&info);
    if (!context) {
        return 1;
    }

    uv_run(&loop_uv, UV_RUN_DEFAULT);

    return 0;
}
