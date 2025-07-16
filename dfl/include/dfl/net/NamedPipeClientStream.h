#pragma once

#include "dfl/net/NamedPipeStream.h"

namespace dfl {
  class NamedPipeClientStream final : public NamedPipeStream {
  public:
    explicit NamedPipeClientStream(const std::string &name);
  };
}
