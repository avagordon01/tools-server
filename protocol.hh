#include <stdint.h>
#include <nlohmann/json.hpp>

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
NLOHMANN_JSON_SERIALIZE_ENUM(streams, {
    {streams::in, "in"},
    {streams::out, "out"},
    {streams::err, "err"},
})

const uint64_t good_token = 0x12345678;

struct connect_message {
    uint64_t token0;
    tools tool_id;
    streams stream_id;
    uint64_t pid;
    uint64_t token1;
};
