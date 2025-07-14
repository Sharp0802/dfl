#ifdef __unix__

#include "net/NamedPipeClientStream.h"

#include <fcntl.h>
#include <sys/stat.h>

#include "io/SafeHandle.h"

namespace {
  dfl::SafeHandle OpenNamedPipe(const std::string &name) {
    const auto path = std::format("/tmp/{}", name);

    const auto fd = open(path.c_str(), O_RDWR);
    if (fd < 0) {
      return dfl::SafeHandle::Invalid();
    }

    return dfl::SafeHandle(fd);
  }
}

namespace dfl {
  NamedPipeClientStream::NamedPipeClientStream(const std::string &name)
    : NamedPipeStream(name, OpenNamedPipe(name)) {
  }
}

#endif
