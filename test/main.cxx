#include <iostream>
#include <bits/ostream.tcc>
#include <dfl/Types.h>

using namespace dfl;

using TestEnum = Enum<i32, f32>;

i32 main() {
  TestEnum te = f32(1.5f);
  match(
    te,
    [](const f32 f) {
      std::cout << "float " << f << std::endl;
    },
    [](const i32 &i) {
      std::cout << "int " << i << std::endl;
    }
  );
}
