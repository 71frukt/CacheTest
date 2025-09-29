#pragma once

#include <list>
#include <stdexcept>
#include <unordered_map>
#include <iterator>
#include <utility>

#include "RLogSU/logger.hpp"
#include "RLogSU/error_handler.hpp"

namespace Cash2QLib {

template<typename PageT, typename KeyT = int>
class PageList
{
public:
    struct PageListElemT {KeyT key; PageT page;};
    using KeyPageListT = std::list<PageListElemT>;

    PageListElemT& Get(const KeyT& key);

    KeyPageListT::iterator& Iterator(const KeyT& key);

    void Erase       (const KeyT& key);

    void PushBack    (const PageT& page, const KeyT key);
    void PushFront   (const PageT& page, const KeyT key);
    void PopFront();
    void PopBack ();

    
    PageListElemT&         Front() { return list_.front(); }
    PageListElemT&         Back () { return list_.back (); }
    KeyPageListT::iterator Begin() { return list_.begin(); }
    KeyPageListT::iterator End  () { return list_.end  (); }
        
    auto Size () { return list_.size(); }
    
    
    void MoveBeforePos(const KeyT& key, const KeyPageListT::iterator& pos);
    void MoveToFront  (const KeyT& key);
    void MoveToBack   (const KeyT& key);
    void Splice       (const KeyPageListT::iterator& page_after_pos, PageList& page_list, const KeyT& cur_page_key);

    bool Contains(const KeyT key);


private:
    KeyPageListT list_;
    std::unordered_map<KeyT, typename KeyPageListT::iterator> hash_map_;
};



template <typename PageT, typename KeyT>
PageList<PageT, KeyT>::PageListElemT& PageList<PageT, KeyT>::Get(const KeyT& key)
{
    RLSU_ASSERT(Contains(key), "Incorrect key: value doesnot exists");
    
    return *(hash_map_.at(key));
}


template <typename PageT, typename KeyT>
PageList<PageT, KeyT>::KeyPageListT::iterator& PageList<PageT, KeyT>::Iterator(const KeyT& key)
{
    RLSU_ASSERT(Contains(key), "Incorrect key: value doesnot exists");
    
    return hash_map_.at(key);
}


template<typename PageT, typename KeyT>
void PageList<PageT, KeyT>::Erase(const KeyT& key)
{
    RLSU_ASSERT(Contains(key),  "Incorrect key: value doesnot exists");

    auto map_iterator = hash_map_.find(key);
    auto erased_page_iterator = map_iterator->second;

    list_    .erase(erased_page_iterator);
    hash_map_.erase(map_iterator);  
}


template<typename PageT, typename KeyT>
void PageList<PageT, KeyT>::PopFront()
{
    KeyT page_key = list_.front().key;

    list_.pop_front();
    hash_map_.erase(page_key);    
}


template<typename PageT, typename KeyT>
void PageList<PageT, KeyT>::PopBack()
{
    KeyT page_key = list_.back().key;

    list_.pop_back();
    hash_map_.erase(page_key);
}


template<typename PageT, typename KeyT>
void PageList<PageT, KeyT>::PushBack(const PageT& page, const KeyT key)
{
    RLSU_VERIFY(!Contains(key));

    list_.push_back({key, page});
    hash_map_[key] = std::prev(list_.end());
}


template<typename PageT, typename KeyT>
void PageList<PageT, KeyT>::PushFront(const PageT& page, const KeyT key)
{
    RLSU_VERIFY(!Contains(key));

    RLSU_INFO("push front, key = {}", key); // TODO delete

    list_.push_front({key, page});
    hash_map_[key] = list_.begin();
}


template <typename PageT, typename KeyT>
void PageList<PageT, KeyT>::MoveBeforePos(const KeyT& moved_page_key, const KeyPageListT::iterator& pos)
{
    RLSU_VERIFY(Contains(moved_page_key));

    auto moved_page_iterator = hash_map_.find(moved_page_key)->second;
    
    if (moved_page_iterator == pos || std::next(moved_page_iterator) == pos) 
        return;

    list_.splice(pos, list_, moved_page_iterator);    
}



template<typename PageT, typename KeyT>
void PageList<PageT, KeyT>::Splice(const KeyPageListT::iterator& page_after_pos, PageList& page_cur_list, const KeyT& cur_page_key)
{
    RLSU_VERIFY(page_cur_list.Contains(cur_page_key));

    auto moved_page_iterator = ERROR_HANDLE(page_cur_list.Iterator(cur_page_key));
    
    if (moved_page_iterator == page_after_pos || std::next(moved_page_iterator) == page_after_pos)
    {
        RLSU_WARNING("Attempt to splice page on it's page");
        return;
    }

    this->list_.splice(page_after_pos, page_cur_list.list_, moved_page_iterator);
    
    this->hash_map_[cur_page_key] = moved_page_iterator;
    
    page_cur_list.hash_map_.erase(cur_page_key);
}


template <typename PageT, typename KeyT>
void PageList<PageT, KeyT>::MoveToFront(const KeyT& moved_page_key)
{
    RLSU_ASSERT(Contains(moved_page_key));

    auto moved_page_iterator = hash_map_.find(moved_page_key)->second;

    if (moved_page_iterator == list_.begin())
        return;

    return ERROR_HANDLE(MoveBeforePos(moved_page_key, list_.begin()));
}


template <typename PageT, typename KeyT>
void PageList<PageT, KeyT>::MoveToBack(const KeyT& moved_page_key)
{
    RLSU_ASSERT(Contains(moved_page_key));

    auto moved_page_iterator = hash_map_.find(moved_page_key)->second;

    if (moved_page_iterator == std::prev(list_.end()))
        return;

    return ERROR_HANDLE(MoveBeforePos(moved_page_key, list_.end()));
}


template <typename PageT, typename KeyT>
bool PageList<PageT, KeyT>::Contains(const KeyT key)
{
    bool hash_map_contains = hash_map_.count(key);

    bool list_contains = false;

    for (auto it = Begin(); it != End(); it++)
    {
        if (it->key == key)
            list_contains = true;
    }

    return hash_map_contains && list_contains;
}


}