#pragma once

// // $$$ cash cash cash

#include <cstddef>
#include <cstdio>
#include <functional>
#include <vector>

#include <unordered_map>
#include <stack>

#include "RLogSU/error_handler.hpp"
#include "Cash2QLib/page_list.hpp"

namespace CashIdeal {
    
template <typename PageT, typename KeyT = int>
class CasherIdeal
{
public:
    CasherIdeal(size_t capacity, std::function<PageT(KeyT)> get_uncashed_page, const std::vector<KeyT>& accessed_keys);

    PageT GetPage(KeyT key);

    bool last_cashe_hitted = false;

private:
    const size_t capacity_;
    Cash2QLib::PageList<PageT, KeyT> cashe_;

    const std::vector<KeyT>& accessed_keys_;

    std::unordered_map<KeyT, std::stack<size_t>> key_access_ids;
    void FillKeyAcessIds_();

    KeyT GetRarestCashedKey_();

    PageT GetPageUncashed_ (const KeyT& key);
    std::function<PageT(KeyT)> UserGetPageUncashed_;
};



template <typename PageT, typename KeyT>
CasherIdeal<PageT, KeyT>::CasherIdeal(size_t capacity, std::function<PageT(KeyT)> get_uncashed_page, const std::vector<KeyT>& accessed_keys)
    : capacity_(capacity)
    , accessed_keys_(accessed_keys)
    , UserGetPageUncashed_(get_uncashed_page)
{
    ERROR_HANDLE(FillKeyAcessIds_());
}


template <typename PageT, typename KeyT>
PageT CasherIdeal<PageT, KeyT>::GetPage(KeyT key)
{
    if (!key_access_ids[key].empty())
        key_access_ids[key].pop();

    if (cashe_.Contains(key))
    {
        last_cashe_hitted = true;
        return cashe_.Get(key).page;
    }

    else
    {
        last_cashe_hitted = false;

        return ERROR_HANDLE(GetPageUncashed_(key));
    }
}


template <typename PageT, typename KeyT>
void CasherIdeal<PageT, KeyT>::FillKeyAcessIds_()
{
    for (int i = accessed_keys_.size() - 1; i >= 0; i--)
    {
        key_access_ids[accessed_keys_[i]].push(i);
    }
}

template <typename PageT, typename KeyT>
PageT CasherIdeal<PageT, KeyT>::GetPageUncashed_(const KeyT& key)
{
    if (cashe_.Size() == capacity_)
    {
        KeyT deleting_from_cash_key = ERROR_HANDLE(GetRarestCashedKey_());
        ERROR_HANDLE(cashe_.Erase(deleting_from_cash_key));
    }

    PageT cashed_page = UserGetPageUncashed_(key);
    ERROR_HANDLE(cashe_.PushFront(cashed_page, key));

    return cashed_page;
}


template <typename PageT, typename KeyT>
KeyT CasherIdeal<PageT, KeyT>::GetRarestCashedKey_()
{
    RLSU_ASSERT(cashe_.Size() != 0);

    KeyT rarest = cashe_.Front().key;
    size_t max_dist_to_access = 0;

    for (auto it = cashe_.Begin(); it != cashe_.End(); it++)
    {
        if (key_access_ids[it->key].empty())
            return it->key;

        size_t cur_key_dist_to_access = key_access_ids.at(it->key).top();

        if (cur_key_dist_to_access > max_dist_to_access)
        {
            max_dist_to_access = cur_key_dist_to_access;
            rarest = it->key;
        }
    }

    return rarest;
}

}