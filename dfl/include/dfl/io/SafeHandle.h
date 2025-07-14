#pragma once

#if __unix__
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#endif

#include "Types.h"
#include "dfl/Object.h"

namespace dfl {
  class SafeHandle final : public Object {
#if __unix__
    int _fd;
    static constexpr int InvalidHandle = -1;
#elif _WIN32
    HANDLE _fd;
    static constexpr int InvalidHandle = INVALID_HANDLE_VALUE;
#endif

  public:
    explicit SafeHandle(const int fd) : _fd(fd) {
    }

    SafeHandle(SafeHandle &&other) noexcept {
      _fd = other._fd;
      other._fd = InvalidHandle;
    }

    ~SafeHandle() {
      if (_fd != InvalidHandle) {
#if __unix__
        close(_fd);
#elif _WIN32
        CloseHandle(_fd);
#endif
      }
    }

    static constexpr SafeHandle Invalid();

    [[nodiscard]]
    decltype(_fd) native_handle() const {
      dfl_assert(_fd != InvalidHandle);
      return _fd;
    }
  };

  constexpr SafeHandle SafeHandle::Invalid() {
    return SafeHandle(InvalidHandle);
  }
}
