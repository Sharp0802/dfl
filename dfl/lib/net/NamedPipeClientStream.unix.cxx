#ifdef __unix__

#include "net/NamedPipeClientStream.h"

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>

#include "io/SafeHandle.h"

namespace {
  dfl::SafeHandle connect_to_socket(const std::string &name) {
    const auto path = std::format("/tmp/{}", name);

    const int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd < 0) {
      return dfl::SafeHandle::Invalid();
    }

    sockaddr_un addr = {};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path) - 1);

    if (::connect(fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
      close(fd);
      return dfl::SafeHandle::Invalid();
    }

    return dfl::SafeHandle(fd);
  }
}

namespace dfl {
  NamedPipeClientStream::NamedPipeClientStream(const std::string &name)
    : NamedPipeStream(name, connect_to_socket(name)) {
  }
}

#endif
