#pragma once

#include <chrono>

#include "Object.h"
#include "Types.h"

namespace dfl {
  interface Stream : Object {
    virtual ~Stream() = 0;
    virtual bool poll(std::chrono::milliseconds timeout) = 0;
    virtual ssize_t write(std::span<u8> src) = 0;
    virtual ssize_t read(std::span<u8> dst) = 0;
    virtual ssize_t copy_to(Stream &dst);
    virtual void flush() = 0;
  };
}
