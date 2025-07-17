#ifdef __unix__

#include <climits>
#include <cstring>
#include <poll.h>
#include <unistd.h>

#include "io/SafeHandleStream.h"

namespace dfl {
  bool SafeHandleStream::poll(std::chrono::milliseconds timeout) {
    pollfd pfd{
      .fd = _handle.native_handle(),
      .events = POLLIN
    };

    const auto timeout_long = timeout.count();
    int timeout_int = static_cast<int>(timeout_long);
    if (timeout_long > INT_MAX) {
      dfl_warning(timeout_long > INT_MAX, "SafeHandleStream::poll(): timeout is too big");
      timeout_int = INT_MAX;
    } else if (timeout_long < 0) {
      timeout_int = INT_MAX;
    }

    const auto r = ::poll(&pfd, 1, timeout_int);
    dfl_warning(r >= 0, "poll(): {}", strerror(errno));
    return r > 0;
  }

  ssize_t SafeHandleStream::write(const std::span<u8> src) {
    return ::write(_handle.native_handle(), src.data(), src.size());
  }

  ssize_t SafeHandleStream::read(std::span<u8> dst) {
    return ::read(_handle.native_handle(), dst.data(), dst.size());
  }

  void SafeHandleStream::flush() {
    fdatasync(_handle.native_handle());
  }
}

#endif
