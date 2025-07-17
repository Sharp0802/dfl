#pragma once

#include <stop_token>

#include "dfl/net/NamedPipeStream.h"

namespace dfl {
  class NamedPipeServerStream final : public NamedPipeStream {
#ifdef __unix__
    SafeHandle _listener;
#endif

  public:
    explicit NamedPipeServerStream(const std::string &name);

    [[nodiscard]]
    bool operator!() const override {
#ifdef __unix__
      return !_listener;
#else
      return true;
#endif
    }

    bool wait_for_connection(std::chrono::milliseconds timeout);
  };
}
