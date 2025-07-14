#pragma once
#include "NamedPipeStream.h"

namespace dfl {

class NamedPipeClientStream final : public NamedPipeStream {
public:
  explicit NamedPipeClientStream(const std::string &name);
};

}
