#pragma once

#include <new>

namespace dfl {
  [[nodiscard, gnu::malloc(free), gnu::alloc_size(1), gnu::alloc_align(2)]]
  void *alloc(size_t size, size_t align);

  template<typename T>
  T *alloc(const size_t size) {
    if (auto p = alloc(sizeof(T) * size, alignof(T))) {
      return static_cast<T*>(p);
    }

    throw std::bad_alloc();
  }

  void free(void *ptr);
} // dfl
