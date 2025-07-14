#include "net/NamedPipeServerStream.h"

#include <fcntl.h>
#include <sys/stat.h>

#include "io/SafeHandle.h"

namespace {
  dfl::SafeHandle CreateNamedPipe(const std::string &name) {
    const auto path = std::format("/tmp/{}", name);

    if (mkfifo(path.c_str(), 0666) < 0 && errno != EEXIST) {
      return dfl::SafeHandle::Invalid();
    }

    const auto fd = open(path.c_str(), O_RDWR);
    if (fd < 0) {
      return dfl::SafeHandle::Invalid();
    }

    return dfl::SafeHandle(fd);
  }
}

namespace dfl {
  NamedPipeServerStream::NamedPipeServerStream(const std::string &name)
    : NamedPipeStream(name, CreateNamedPipe(name)) {
  }
}
