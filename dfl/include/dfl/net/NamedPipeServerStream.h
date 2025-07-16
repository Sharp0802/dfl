#pragma once

#include "NamedPipeStream.h"

namespace dfl {
  class NamedPipeServerStream final : public NamedPipeStream {
  public:
    explicit NamedPipeServerStream(const std::string &name);
  };
}
