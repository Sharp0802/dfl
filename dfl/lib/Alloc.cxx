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

#ifdef _WIN32
    return _aligned_malloc(size, align);
#else
    return std::aligned_alloc(align, size);
#endif
  }

  void free(void *ptr) {
#ifdef _WIN32
    _aligned_free(ptr);
#else
    std::free(ptr);
#endif
  }
} // dfl
