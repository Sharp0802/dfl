#pragma once

#include <chrono>

#include "dfl/Object.h"
#include "dfl/Types.h"

namespace dfl {
  struct Stream : Object {
    virtual ~Stream() = default;
    virtual bool poll(std::chrono::milliseconds timeout) = 0;
    virtual ssize_t write(std::span<u8> src) = 0;
    virtual ssize_t read(std::span<u8> dst) = 0;
    virtual ssize_t copy_to(Stream &dst);
    virtual void flush() = 0;
  };
}
