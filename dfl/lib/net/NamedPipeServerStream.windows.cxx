#ifdef _WIN32

#include "net/NamedPipeServerStream.h"

#include <format>
#include <windows.h>

#include "io/SafeHandle.h"

namespace {
#ifdef CreateNamedPipe
#undef CreateNamedPipe
#endif

  dfl::SafeHandle CreateNamedPipe(const std::string &name) {
    const auto pipeName = std::format(R"(\\.\pipe\{})", name);

    const HANDLE hPipe = CreateNamedPipeA(
      pipeName.c_str(),
      PIPE_ACCESS_DUPLEX,
      PIPE_TYPE_BYTE |
      PIPE_READMODE_BYTE |
      PIPE_NOWAIT,
      PIPE_UNLIMITED_INSTANCES,
      4096,
      4096,
      0,
      nullptr
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
      return dfl::SafeHandle::Invalid();
    }

    return dfl::SafeHandle(hPipe);
  }
}

namespace dfl {
  NamedPipeServerStream::NamedPipeServerStream(const std::string &name)
    : NamedPipeStream(name, CreateNamedPipe(name)) {
  }

  bool NamedPipeServerStream::wait_for_connection(const std::chrono::milliseconds timeout) {
    for (const auto begin = std::chrono::high_resolution_clock::now();
         timeout.count() < 0 || std::chrono::high_resolution_clock::now() - begin < timeout;) {
      if (ConnectNamedPipe(_handle.native_handle(), nullptr) || GetLastError() == ERROR_PIPE_CONNECTED) {
        return true;
      }
    }

    return false;
  }
}

#endif
