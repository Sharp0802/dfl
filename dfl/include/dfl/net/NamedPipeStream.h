#pragma once

#include <string>

#include "io/SafeHandleStream.h"

namespace dfl {
  class SafeHandle;

  class NamedPipeStream : public SafeHandleStream {
    std::string _name;

  protected:
    NamedPipeStream(std::string name, SafeHandle &&handle)
      : SafeHandleStream(std::move(handle)),
        _name(std::move(name)) {
    }

  public:
    [[nodiscard]]
    const std::string& name() const {
      return _name;
    }
  };
}
