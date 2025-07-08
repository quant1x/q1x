#pragma once
#ifndef API_AFFINITY_H
#define API_AFFINITY_H 1

#include "q1x/std/base.h"

// CPU亲和性
namespace affinity {

    bool bind_current_thread_to_optimal_cpu(std::error_code &ec);
}

#endif // API_AFFINITY_H