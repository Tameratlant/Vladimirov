#include <iterator>
#include <list>
#include <unordered_map>
#include <vector>

namespace caches {

template <typename T> struct elem {
    T value;
    int type;
};



template <typename KeyT, typename U> struct lists {
    std::list<std::pair<KeyT, U>> in_;
    std::list<std::pair<KeyT, U>> out_;
    std::list<std::pair<KeyT, U>> hot_;
    int in_size;
    int out_size;
    int hot_size;
    
};

template <typename T, typename KeyT = int> struct cache_t {
    lists <KeyT, elem<T>> list;
    cache_t(size_t size) {
        if (size < 5) {
            printf("Size is too small\n");
            return;
        } else {
            size = (int) ((size / 5) * 5);              //делаем size кратным 5
            list.in_size = size * 0.2;
            list.out_size = size * 0.6;
            list.hot_size = size * 0.2;   
        }
    }
    using ListIt = typename std::list<std::pair<KeyT, elem<T>>>::iterator;
    std::unordered_map<KeyT, ListIt> hash;
    //std::unordered_map <KeyT, elem<T>> hash;
    
    template <typename F> bool lookup_update(KeyT key, F slow_get_page) {
        auto hit = hash.find(key);
        //4 случая:
        //Элемент найден в in
        //Элементнайден в out
        //Элемент найден в hot
        //Элемент не найден
        if (hit == hash.end()) {                       //Не найден

            if (list.in_size == list.in_.size()) {
                if (list.out_.size() == list.hot_size) {
                    hash.erase(list.out_.back().first);
                    list.out_.pop_back();
                    printf("Deleted out's last elem\n");
                }
                list.out_.emplace_front(list.in_.back());
                //*((list.hot_.begin())->second).type = 1;
                (*(list.out_.begin())).second.type = 1;
                hash.emplace(list.in_.back().first, list.out_.begin());
                printf("Added in's last elem to out\n");
                hash.erase(list.in_.back().first);
                list.in_.pop_back();
                printf("Deleted in's last elem\n");
            }
            elem<T> node;
            node.type = 0;
            node.value = slow_get_page(key);
            list.in_.emplace_front(key, node);
            hash.emplace(key, list.in_.begin());
            printf("New elem added to in\n");
            return false;
        } else if ((*(hit->second)).second.type == 0) {     //Найдено в in
            printf("Finded in in\n");
            auto el = hit->second;
            if (el != list.in_.begin()) {
                list.in_.splice(list.in_.begin(), list.in_, el, std::next(el)); //Удаляем из текущего положения в начало
            }
        } else if((*(hit->second)).second.type == 1) {      //найдено в out
            if (list.hot_size == list.hot_.size()) {
                hash.erase(list.hot_.back().first);
                list.hot_.pop_back();
                printf("Deleted hot's last elem\n");
            }
            hash.erase(hit);
            list.out_.erase(hit->second);
            printf("Deleted from out\n");
            list.hot_.emplace_front(*(hit->second));
            (*(list.hot_.begin())).second.type = 2;
            hash.emplace(key, list.hot_.begin());
            printf("Added to hot\n");
        } else if((*(hit->second)).second.type == 2) { 
            printf("Finded in hot\n");
            auto el = hit->second;
            if (el != list.in_.begin()) {
                list.hot_.splice(list.hot_.begin(), list.hot_, el, std::next(el)); //Удаляем из текущего положения в начало
            }
        }
        return true;
    }
    
};
} // namespace caches