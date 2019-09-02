#pragma once

#include <chrono>
#include <thread>

namespace aether {

namespace timer {

using time_type = std::chrono::time_point<std::chrono::high_resolution_clock>;
using duration_type = time_type::duration;

static time_type get() {
    return std::chrono::high_resolution_clock::now();
}

static double diff(const time_type &stop_time, const time_type &start_time) {
    std::chrono::duration<double> diff = stop_time - start_time;
    return diff.count();
}

static time_type add(const time_type &start_time, const duration_type &duration) {
    return start_time + duration;
}

static void sleep_until(const time_type &target_time) {
    std::this_thread::sleep_until(target_time);
}

}

}
