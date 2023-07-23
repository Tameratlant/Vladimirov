#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>

namespace caches {

template <typename T, typename KeyT = int> struct cache_t {

  size_t size_;
  std::list<std::pair<KeyT, T>>
      cache_; //Двусвязный список, в котором хранятся сами элементы и клюи
  using ListIt = typename std::list<std::pair<KeyT, T>>::iterator;
  std::unordered_map<KeyT, ListIt>
      hash_; //Таблица с итераторами для доступа к элементам кеша за О(1)

  cache_t(size_t size) : size_(size) {}

  bool full() const {
    if (cache_.size() == size_)
      return 1;
    return 0;
  }

  template <typename F> bool lookup_update(KeyT key, F slow_get_page) {

    auto hit = hash_.find(key);
    // если такого нет, возращается итератор конца
    if (hit == hash_.end()) {
      if (full()) {
        //тогда удаляем последний элемент, добавляем этот в начало, то же самое
        //с таблицей
        hash_.erase(cache_.back().first);
        cache_.pop_back();
      }
      cache_.emplace_front(key, slow_get_page(key));
      hash_.emplace(key, cache_.begin());
      return false;
    } else {
      //тогда добавляем в начало, удаляя из прежнего места. Обновляем данные в
      //таблице
      auto elem = hit->second;
      if (elem != cache_.begin()) {
        cache_.splice(cache_.begin(), cache_, elem,
                      std::next(elem)); //Удаляем из текущего положения в начало
      }
    }
  }
};
} // namespace caches