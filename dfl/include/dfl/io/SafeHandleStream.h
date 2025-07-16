#pragma once

#include "dfl/io/SafeHandle.h"
#include "dfl/io/Stream.h"

namespace dfl {
  class SafeHandleStream : public Stream {
    SafeHandle _handle;

  protected:
    explicit SafeHandleStream(SafeHandle&& handle) : _handle(std::move(handle)) {}

  public:
    ~SafeHandleStream() override = default;

    bool poll(std::chrono::milliseconds timeout) override;
    ssize_t write(std::span<u8> src) override;
    ssize_t read(std::span<u8> dst) override;
    void flush() override;

    [[nodiscard]]
    const SafeHandle& handle() const { return _handle; }
  };
}
