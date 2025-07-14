#ifdef _WIN32

#include <windows.h>
#include <climits>
#include <cstring>

#include "io/SafeHandleStream.h"

namespace dfl {
  bool SafeHandleStream::poll(std::chrono::milliseconds timeout) {
    HANDLE handle = _handle.native_handle();

    const auto timeout_long = timeout.count();
    DWORD timeout_dword = static_cast<DWORD>(timeout_long);
    if (timeout_long > ULONG_MAX) {
      dfl_warning(timeout_long > ULONG_MAX, "SafeHandleStream::poll(): timeout is too big");
      timeout_dword = ULONG_MAX;
    }

    DWORD result = WaitForSingleObject(handle, timeout_dword);

    if (result == WAIT_FAILED) {
      dfl_warning(false, "WaitForSingleObject(): Error {}", GetLastError());
      return false;
    }

    return result == WAIT_OBJECT_0;
  }

  ssize_t SafeHandleStream::write(const std::span<u8> src) {
    HANDLE handle = _handle.native_handle();
    DWORD bytes_written = 0;

    DWORD bytes_to_write = static_cast<DWORD>(src.size());
    if (src.size() > ULONG_MAX) {
      dfl_warning(src.size() > ULONG_MAX, "SafeHandleStream::write(): size too large for single write");
      bytes_to_write = ULONG_MAX;
    }

    BOOL success = WriteFile(handle, src.data(), bytes_to_write, &bytes_written, nullptr);

    if (!success) {
      dfl_warning(false, "WriteFile(): Error {}", GetLastError());
      return -1;
    }

    return static_cast<ssize_t>(bytes_written);
  }

  ssize_t SafeHandleStream::read(std::span<u8> dst) {
    HANDLE handle = _handle.native_handle();
    DWORD bytes_read = 0;

    DWORD bytes_to_read = static_cast<DWORD>(dst.size());
    if (dst.size() > ULONG_MAX) {
      dfl_warning(dst.size() > ULONG_MAX, "SafeHandleStream::read(): size too large for single read");
      bytes_to_read = ULONG_MAX;
    }

    BOOL success = ReadFile(handle, dst.data(), bytes_to_read, &bytes_read, nullptr);

    if (!success) {
      DWORD error = GetLastError();
      if (error == ERROR_BROKEN_PIPE || error == ERROR_HANDLE_EOF) {
        return 0;
      }
      dfl_warning(false, "ReadFile(): Error {}", error);
      return -1;
    }

    return static_cast<ssize_t>(bytes_read);
  }

  void SafeHandleStream::flush() {
    HANDLE handle = _handle.native_handle();

    BOOL success = FlushFileBuffers(handle);

    if (!success) {
      dfl_warning(false, "FlushFileBuffers(): Error {}", GetLastError());
    }
  }
}

#endif