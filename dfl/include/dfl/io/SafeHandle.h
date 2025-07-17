#pragma once

#if __unix__
#include <unistd.h>
#elif _WIN32
#include <windows.h>
#endif

#include "dfl/Object.h"
#include "dfl/Types.h"

namespace dfl {
  class SafeHandle final : public Object {
#if __unix__
    int _fd;
    static constexpr int InvalidHandle = -1;
#elif _WIN32
    HANDLE _fd;
    static constexpr LONG_PTR InvalidHandle = -1;
#endif

  public:
    explicit SafeHandle(decltype(_fd) fd) : _fd(fd) {
    }

    SafeHandle(SafeHandle &&other) noexcept {
      _fd = other._fd;
      other._fd = reinterpret_cast<decltype(_fd)>(InvalidHandle);
    }

    SafeHandle& operator=(SafeHandle &&other) noexcept {
      std::swap(_fd, other._fd);
      return *this;
    }

    ~SafeHandle() {
      if (_fd != reinterpret_cast<decltype(_fd)>(InvalidHandle)) {
#if __unix__
        close(_fd);
#elif _WIN32
        CloseHandle(_fd);
#endif
      }
    }

    static constexpr SafeHandle Invalid();

    [[nodiscard]]
    bool operator!() const {
      return _fd == reinterpret_cast<decltype(_fd)>(InvalidHandle);
    }

    [[nodiscard]]
    decltype(_fd) native_handle() const {
      dfl_assert(_fd != reinterpret_cast<decltype(_fd)>(InvalidHandle));
      return _fd;
    }
  };

  constexpr SafeHandle SafeHandle::Invalid() {
    // ReSharper disable once CppRedundantCastExpression
    return SafeHandle(reinterpret_cast<decltype(_fd)>(InvalidHandle));
  }
}
