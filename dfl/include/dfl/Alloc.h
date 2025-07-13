#pragma once

namespace dfl {
  [[nodiscard, gnu::malloc(free), gnu::alloc_size(1), gnu::alloc_align(2)]]
  void *alloc(size_t size, size_t align);

  void free(void *ptr);
} // dfl
