#include "io/Stream.h"

namespace dfl {
  ssize_t Stream::copy_to(Stream &dst) {
    thread_local std::array<u8, 81920> buffer;

    ssize_t total = 0;
    while (true) {
      const auto size = read(buffer);
      if (size < 0) {
        return -total;
      }
      if (size == 0) {
        break;
      }

      for (ssize_t i = 0; i < size; ) {
        const auto tmp = dst.write(buffer);
        if (tmp < 0) {
          return -total;
        }
        if (tmp == 0) {
          dfl_warning(tmp != 0, "write(): couldn't write any bytes");
        }

        i += tmp;
      }

      total += size;
    }

    return total;
  }
}
