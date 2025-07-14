#pragma once

#include "NamedPipeStream.h"

namespace dfl {
  class NamedPipeServerStream : public NamedPipeStream {
  public:
    explicit NamedPipeServerStream(const std::string &name);
  };
}
