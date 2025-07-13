#pragma once

#include <cstdint>
#include <variant>

namespace dfl {
  using u8 = uint8_t;
  using u16 = uint16_t;
  using u32 = uint32_t;
  using u64 = uint64_t;

  using i8 = int8_t;
  using i16 = int16_t;
  using i32 = int32_t;
  using i64 = int64_t;

  using f32 = float;
  using f64 = double;

  static_assert(sizeof(f32) == 4);
  static_assert(sizeof(f64) == 8);

  using utf8 = char8_t;
  using utf16 = char16_t;
  using utf32 = char32_t;


  template <typename... T>
  using Enum = std::variant<T...>;

  template <typename... T>
  struct Match : T... {
    using T::operator()...;
  };

  template <typename... T>
  Match(T...) -> Match<T...>;

  template<typename T, typename ...Visitor>
  void match(T&& t, Visitor&&... visitors) {
    std::visit(Match{ visitors... }, t);
  }

  template <typename T, typename E>
  using Result = Enum<T, E>;

  struct None {
#ifdef __GNUG__
    int __dummy[0];
#elifndef ALLOW_SIZED_NONE
#warning GNU extension is required to make None zero-sized struct; To disable this message, use ALLOW_SIZED_NONE.
#endif
  };

  template <typename T>
  using Option = Enum<T, None>;
}
