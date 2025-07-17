#ifdef __unix__

#include "net/NamedPipeServerStream.h"

#include <cstring>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "io/SafeHandle.h"

namespace {
  dfl::SafeHandle create_listen_socket(const std::string &name) {
    const auto path = std::format("/tmp/{}", name);

    const int listen_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listen_fd < 0) {
      return dfl::SafeHandle::Invalid();
    }

    sockaddr_un addr = {};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, path.c_str(), sizeof(addr.sun_path) - 1);

    unlink(path.c_str());

    if (bind(listen_fd, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0) {
      close(listen_fd);
      return dfl::SafeHandle::Invalid();
    }

    if (listen(listen_fd, SOMAXCONN) < 0) {
      close(listen_fd);
      return dfl::SafeHandle::Invalid();
    }

    return dfl::SafeHandle(listen_fd);
  }
}

namespace dfl {
  NamedPipeServerStream::NamedPipeServerStream(const std::string &name)
    : NamedPipeStream(name, SafeHandle::Invalid()),
      _listener(create_listen_socket(name)) {
  }

  bool NamedPipeServerStream::wait_for_connection(std::chrono::milliseconds timeout) {
    pollfd pfd{
      .fd = _listener.native_handle(),
      .events = POLLIN,
      .revents = 0
    };

    int timeout_int = static_cast<int>(timeout.count());
    if (timeout.count() > INT_MAX) {
      dfl_warning(timeout.count() > INT_MAX, "NamedPipeServerStream::wait_for_connection(): timeout is too big");
      timeout_int = INT_MAX;
    } else if (timeout.count() < 0) {
      timeout_int = INT_MAX;
    }

    const auto ret = ::poll(&pfd, 1, timeout_int);
    if (ret < 0) {
      return false;
    }
    if (ret == 0) {
      errno = 0;
      return false;
    }
    if (~pfd.revents & POLLIN) {
      errno = EIO;
      return false;
    }

    const int fd = accept(_listener.native_handle(), nullptr, nullptr);
    if (fd < 0) {
      return false;
    }

    _handle = std::move(SafeHandle(fd));
    return true;
  }
}

#endif
