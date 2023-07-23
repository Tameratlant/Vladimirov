//  Для рализации LRU кеша будем пользоваться следующими сущностями:
//  1. list в качестве двусвязного списка для добавления и удаления элеменитов
//  за О(1)
//  2. Хеш таблица для поиска за О(1)

#include "cach.hpp"
#include <cassert>
#include <iostream>

int slow_get_page_int(int key) { return key; }

int main() {
  int n, size;
  std::cin >> n >> size;
  int hits = 0;
  caches::cache_t<int> cache{size};
  int key;
  for (int i = 0; i < n; ++i) {
    std::cin >> key;
    if (cache.lookup_update(key, slow_get_page_int))
      hits += 1;
  }
  std::cout << hits << std::endl;
}