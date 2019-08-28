#include <stdint.h>
#include <vector>
#include <deque>

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
static std::deque<struct out_buf> out_bufs {};
static std::deque<struct in_buf> in_bufs {};

int server_start();
