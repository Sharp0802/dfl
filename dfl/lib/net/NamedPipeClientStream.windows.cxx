#ifdef _WIN32

#include "net/NamedPipeClientStream.h"

#include <windows.h>
#include <format>

#include "io/SafeHandle.h"

namespace {
  dfl::SafeHandle OpenNamedPipe(const std::string &name) {
    const auto pipeName = std::format(R"(\\.\pipe\{})", name);

    // Try to open the named pipe
    HANDLE hPipe = CreateFileA(
      pipeName.c_str(),           // pipe name
      GENERIC_READ | GENERIC_WRITE, // read and write access
      0,                          // no sharing
      nullptr,                    // default security attributes
      OPEN_EXISTING,              // opens existing pipe
      0,                          // default attributes
      nullptr                     // no template file
    );

    if (hPipe == INVALID_HANDLE_VALUE) {
      // If pipe is busy, wait for it
      if (GetLastError() == ERROR_PIPE_BUSY) {
        // Wait up to 5 seconds for the pipe to become available
        if (WaitNamedPipeA(pipeName.c_str(), 5000)) {
          hPipe = CreateFileA(
            pipeName.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0,
            nullptr,
            OPEN_EXISTING,
            0,
            nullptr
          );
        }
      }

      if (hPipe == INVALID_HANDLE_VALUE) {
        return dfl::SafeHandle::Invalid();
      }
    }

    // Set the pipe to message-read mode (optional, depends on your needs)
    DWORD dwMode = PIPE_READMODE_BYTE;
    BOOL success = SetNamedPipeHandleState(
      hPipe,      // pipe handle
      &dwMode,    // new pipe mode
      nullptr,    // don't set maximum bytes
      nullptr     // don't set maximum time
    );

    if (!success) {
      CloseHandle(hPipe);
      return dfl::SafeHandle::Invalid();
    }

    return dfl::SafeHandle(hPipe);
  }
}

namespace dfl {
  NamedPipeClientStream::NamedPipeClientStream(const std::string &name)
    : NamedPipeStream(name, OpenNamedPipe(name)) {
  }
}

#endif
