#include "2Q-cache.hpp"
#include <cassert>
#include <iostream>

int slow_get_page_int(int key) { return key; }

int main() {
  int n, size;
  std::cin >> n >> size;
  int hits = 0;
  caches::cache_t <int>cache(size);
  int key;
  for (int i = 0; i < n; ++i) {
    std::cin >> key;
    if (cache.lookup_update(key, slow_get_page_int))
      hits += 1;
  }
  std::cout << hits << std::endl;
}