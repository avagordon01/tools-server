#include <stdint.h>

enum class tools {
    gdb,
    perf,
    ss,
    ebpf,
};

enum class streams {
    in,
    out,
    err,
};

const uint64_t good_token = 0x12345678;

struct __attribute__((__packed__)) connect_message {
    uint64_t token0;
    tools tool_id;
    streams stream_id;
    uint64_t hadean_pid;
    uint64_t token1;
};
