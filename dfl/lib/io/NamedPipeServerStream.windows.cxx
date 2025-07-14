#ifdef _WIN32

#include "net/NamedPipeServerStream.h"

#include <windows.h>
#include <format>

#include "io/SafeHandle.h"

namespace {
  dfl::SafeHandle CreateNamedPipe(const std::string &name) {
    const auto pipeName = std::format("\\\\.\\pipe\\{}", name);

    HANDLE hPipe = CreateNamedPipeA(
      pipeName.c_str(),           // pipe name
      PIPE_ACCESS_DUPLEX,         // read/write access
      PIPE_TYPE_BYTE |            // byte type pipe
      PIPE_READMODE_BYTE |        // byte-read mode
      PIPE_WAIT,                  // blocking mode
      1,                          // max instances (1 for simplicity)
      4096,                       // output buffer size
      4096,                       // input buffer size
      0,                          // client time-out
      nullptr                     // default security attribute
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
      return dfl::SafeHandle::Invalid();
    }

    // Wait for a client to connect
    BOOL connected = ConnectNamedPipe(hPipe, nullptr);
    if (!connected && GetLastError() != ERROR_PIPE_CONNECTED) {
      CloseHandle(hPipe);
      return dfl::SafeHandle::Invalid();
    }

    return dfl::SafeHandle(hPipe);
  }
}

namespace dfl {
  NamedPipeServerStream::NamedPipeServerStream(const std::string &name)
    : NamedPipeStream(name, CreateNamedPipe(name)) {
  }
}

#endif
