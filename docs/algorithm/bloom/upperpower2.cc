#include <iostream>
#include <cstdint>

inline uint64_t upperpower2(uint64_t x) {
  x--;
  x |= x >> 1;
  x |= x >> 2;
  x |= x >> 4;
  x |= x >> 8;
  x |= x >> 16;
  x |= x >> 32;
  x++;
  return x;
}

int main() {
  for (unsigned i = 0; i < 20; ++i) {
    std::cout << "i: " << i
              << ", upperpower(i): " << upperpower2(i) << std::endl;
  }
  return 0;
}