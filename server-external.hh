#include <libwebsockets.h>

#define LWS_PLUGIN_STATIC
#include "protocol-external.hh"

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

struct lws_protocols protocols[] = {
    { "http", lws_callback_http_dummy, 0, 0 },
    LWS_PLUGIN_PROTOCOL_MINIMAL_SERVER_ECHO,
    { NULL, NULL, 0, 0 }
};

int options, interrupted;
const struct lws_protocol_vhost_options pvo_options = {
    NULL,
    NULL,
    "options",        /* pvo name */
    (char *)&options    /* pvo value */
};

const struct lws_protocol_vhost_options pvo_interrupted = {
    &pvo_options,
    NULL,
    "interrupted",        /* pvo name */
    (char *)&interrupted    /* pvo value */
};

const struct lws_protocol_vhost_options pvo = {
    NULL,                /* "next" pvo linked-list */
    &pvo_interrupted,        /* "child" pvo linked-list */
    "lws-minimal-server-echo",    /* protocol name we belong to on this vhost */
    ""                /* ignored */
};

const struct lws_extension extensions[] = {
    {
        "permessage-deflate",
        lws_extension_callback_pm_deflate,
        "permessage-deflate"
         "; client_no_context_takeover"
         "; client_max_window_bits"
    },
    { NULL, NULL, NULL }
};

static struct lws_context* context;

void create_lws_context_with_foreign_loop(void* loop) {
    struct lws_context_creation_info info = {};
    info.port = 9999;
    info.protocols = protocols;
    info.extensions = extensions;
    info.options =
        LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE |
        LWS_SERVER_OPTION_ALLOW_LISTEN_SHARE |
        LWS_SERVER_OPTION_LIBUV;
    info.pvo = &pvo;
    info.mounts = &mount;
    info.pcontext = &context;

    if (false) {
        info.options |= LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
        info.ssl_cert_filepath = "localhost-100y.cert";
        info.ssl_private_key_filepath = "localhost-100y.key";
    }

    void* foreign_loops[1] = {loop};

    info.foreign_loops = foreign_loops;

    context = lws_create_context(&info);
}
