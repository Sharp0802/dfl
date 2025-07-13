#include <cstdlib>

#include "Alloc.h"
#include "Types.h"

namespace dfl {
  constexpr u8 Allocated = 0xCA;
  constexpr u8 Freed = 0x53;

  void *alloc(const size_t size, const size_t align) {
    if (size == 0 || align == 0 || size % align != 0) {
      return nullptr;
    }

    return std::aligned_alloc(align, size);
  }

  void free(void *ptr) {
    std::free(ptr);
  }
} // dfl
