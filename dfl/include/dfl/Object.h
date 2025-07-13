#pragma once

namespace dfl {
  class Object {
  public:
    Object(const Object &) noexcept = delete;
    Object &operator=(const Object &) noexcept = delete;

  protected:
    Object() = default;
    ~Object() = default;
  };
}
