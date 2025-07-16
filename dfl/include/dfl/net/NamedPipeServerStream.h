#pragma once

#include "dfl/net/NamedPipeStream.h"

namespace dfl {
  class NamedPipeServerStream final : public NamedPipeStream {
  public:
    explicit NamedPipeServerStream(const std::string &name);
  };
}
