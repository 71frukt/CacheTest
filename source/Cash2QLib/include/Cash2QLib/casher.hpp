#pragma once

// // $$$ cash cash cash

#include <cstddef>
#include <cstdio>
#include <iostream>
#include <functional>

#include "Cash2QLib/page_list.hpp"

#include "RLogSU/logger.hpp"
#include "RLogSU/error_handler.hpp"

namespace Cash2QLib {
    
template <typename PageT, typename KeyT = int>
class Casher2Q
{
public:
    Casher2Q(size_t capacity, std::function<PageT(KeyT)> get_uncashed_page);

    PageT GetPage(KeyT key);

    bool last_cashe_hitted = false;

    void Dump();
    
private:
    const size_t capacity_;
    std::function<PageT(KeyT)> UserGetPageUncashed_;

    PageList<PageT, KeyT> cashe_in_;
    PageList<PageT, KeyT> cashe_hot_;
    PageList<bool , KeyT> cashe_history_;

    const size_t cashe_hot_capa_;
    const size_t cashe_in_capa_;
    const size_t cashe_history_capa_;

    PageT GetPageCashedHot_   (const KeyT& key);
    PageT GetPageCashedIn_    (const KeyT& key);
    PageT GetPageFromHistory_ (const KeyT& key);
    PageT GetPageUncashed_    (const KeyT& key);

    void RemoveExtraPageToHistory_(PageList<PageT, KeyT>& page_list);
};



template <typename PageT, typename KeyT>
Casher2Q<PageT, KeyT>::Casher2Q(size_t capacity, std::function<PageT(KeyT)> get_page_uncashed)
    : UserGetPageUncashed_(get_page_uncashed)
    , capacity_           (capacity)
    , cashe_hot_capa_     ((capacity * 0.75) > 0 ? (capacity * 0.75) : 1)
    , cashe_in_capa_      (capacity - cashe_hot_capa_)
    , cashe_history_capa_ (capacity * 0.5)
{
    RLSU_ASSERT(cashe_hot_capa_ + cashe_in_capa_ == capacity_, "cashe_hot_capa_ = {}, cashe_in_capa_ = {}, capacity_ = {}", cashe_hot_capa_, cashe_in_capa_, capacity_);

    RLSU_INFO("cashe_hot_capa_ = {}, cashe_in_capa_ = {}, cashe_history_capa_ = {}", cashe_hot_capa_, cashe_in_capa_, cashe_history_capa_);

    if (capacity < 3)
        RLSU_ERROR("capacity < 3! 2Q hash doesn't make sense.");
}


template <typename PageT, typename KeyT>
PageT Casher2Q<PageT, KeyT>::GetPage(KeyT key)
{
    Dump();

    if      (cashe_hot_    .Contains(key)) return ERROR_HANDLE(GetPageCashedHot_  (key));

    else if (cashe_in_     .Contains(key)) return ERROR_HANDLE(GetPageCashedIn_   (key));
    
    else if (cashe_history_.Contains(key)) return ERROR_HANDLE(GetPageFromHistory_(key));

    else                                   return ERROR_HANDLE(GetPageUncashed_   (key));
}


template <typename PageT, typename KeyT>
PageT Casher2Q<PageT, KeyT>::GetPageCashedHot_(const KeyT& key)
{
    RLSU_ASSERT(cashe_hot_.Contains(key));
    RLSU_INFO("cashe hit (hot), key = {}", key);
    last_cashe_hitted = true;

    ERROR_HANDLE(cashe_hot_.MoveToFront(key));

    return ERROR_HANDLE(cashe_hot_.Get(key).page);
}


template <typename PageT, typename KeyT>
PageT Casher2Q<PageT, KeyT>::GetPageCashedIn_(const KeyT& key)
{
    RLSU_ASSERT(cashe_in_.Contains(key));
    RLSU_INFO("cashe hit (in), key = {}", key);
    last_cashe_hitted = true;

    ERROR_HANDLE(cashe_hot_.Splice(cashe_hot_.Begin(), cashe_in_, key));

    while (cashe_hot_.Size() > cashe_hot_capa_)
        ERROR_HANDLE(RemoveExtraPageToHistory_(cashe_hot_));

    return ERROR_HANDLE(cashe_hot_.Get(key).page);
}


template <typename PageT, typename KeyT>
PageT Casher2Q<PageT, KeyT>::GetPageFromHistory_(const KeyT& key)
{
    RLSU_ASSERT(cashe_history_.Contains(key));
    RLSU_INFO("cashe miss (history), key = {}", key);
    last_cashe_hitted = false;

    PageT page = UserGetPageUncashed_(key);
    ERROR_HANDLE(cashe_hot_.PushFront(page, key));
    cashe_history_.Erase(key);

    while (cashe_hot_.Size() > cashe_hot_capa_)
        ERROR_HANDLE(RemoveExtraPageToHistory_(cashe_hot_));

    return ERROR_HANDLE(cashe_hot_.Get(key).page);
}


template <typename PageT, typename KeyT>
PageT Casher2Q<PageT, KeyT>::GetPageUncashed_(const KeyT& key)
{
    RLSU_ASSERT(!cashe_hot_.Contains(key) && !cashe_in_.Contains(key) && !cashe_history_.Contains(key));
    RLSU_INFO("cashe miss (fully), key = {}", key);
    last_cashe_hitted = false;

    PageT page = UserGetPageUncashed_(key);
    ERROR_HANDLE(cashe_in_.PushFront(page, key));

    while (cashe_in_.Size() > cashe_in_capa_)
        ERROR_HANDLE(RemoveExtraPageToHistory_(cashe_in_));

    return ERROR_HANDLE(cashe_in_.Get(key).page);
}


template <typename PageT, typename KeyT>
void Casher2Q<PageT, KeyT>::RemoveExtraPageToHistory_(PageList<PageT, KeyT>& page_list)
{
    RLSU_INFO("removing last");

    KeyT extra_page_key = page_list.Back().key;

    ERROR_HANDLE(cashe_history_.PushFront(true, extra_page_key));
    while (cashe_history_.Size() > cashe_history_capa_)
        ERROR_HANDLE(cashe_history_.PopBack());

    ERROR_HANDLE(page_list.PopBack());    
}


template <typename PageT, typename KeyT>
void Casher2Q<PageT, KeyT>::Dump()
{
    RLSU_DUMP("\n");

    RLSU_LOG("cashe_hot_:    \t");

    for (auto it = cashe_hot_.Begin(); it != cashe_hot_.End(); it++)
    {
        RLSU_ASSERT(cashe_hot_.Contains(it->key));
        RLSU_LOG("'{}' ", it->key);
    }


    RLSU_LOG("\ncashe_in_:    \t");

    for (auto it = cashe_in_.Begin(); it != cashe_in_.End(); it++)
    {
        RLSU_ASSERT(cashe_in_.Contains(it->key));
        RLSU_LOG("'{}' ", it->key);
    }

    RLSU_LOG("\ncashe_history_:\t");

    for (auto it = cashe_history_.Begin(); it != cashe_history_.End(); it++)
    {
        RLSU_ASSERT(cashe_history_.Contains(it->key));
        RLSU_LOG("'{}' ", it->key);
    }
    RLSU_LOG("\n\n");
}

}