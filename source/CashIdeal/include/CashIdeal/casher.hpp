#pragma once

// // $$$ cash cash cash

#include <cstddef>
#include <cstdio>
#include <functional>
#include <vector>

#include "Cash2QLib/casher.hpp"
#include "RLogSU/logger.hpp"
#include "RLogSU/error_handler.hpp"
#include "Cash2QLib/page_list.hpp"

namespace CashIdeal {
    
template <typename PageT, typename KeyT = int>
class CasherIdeal
{
public:
    CasherIdeal(size_t capacity, std::function<PageT(KeyT)> get_uncashed_page, std::vector<KeyT>& accessed_keys);

    PageT GetPage(KeyT key);

    bool last_cashe_hitted = false;

private:
    const size_t capacity_;
    Cash2QLib::PageList<PageT, KeyT> cashe_;

    struct AccessNum { KeyT key; size_t num = 0; };
    std::vector<AccessNum> sorted_keys_access_nums_;

    const std::vector<KeyT>& accessed_keys_;

    void  FillAndSortPageAccessNums_();
    PageT CashPageIfFreq_(const KeyT& key);

    PageT& GetPageUncashed_ (const KeyT& key);
    std::function<PageT(KeyT)> UserGetPageUncashed_;
};



template <typename PageT, typename KeyT>
CasherIdeal<PageT, KeyT>::CasherIdeal(size_t capacity, std::function<PageT(KeyT)> get_uncashed_page, std::vector<KeyT>& accessed_keys)
    : capacity_(capacity)
    , accessed_keys_(accessed_keys)
    , UserGetPageUncashed_(get_uncashed_page)
{
    FillAndSortPageAccessNums_();
}


template <typename PageT, typename KeyT>
void CasherIdeal<PageT, KeyT>::FillAndSortPageAccessNums_()
{
    RLSU_ASSERT(sorted_keys_access_nums_.size() == 0);

    const size_t accessed_keys_size = accessed_keys_.size();
    
    for (size_t accessed_keys_i = 0; accessed_keys_i < accessed_keys_size; accessed_keys_i++)
    {
        const size_t sorted_keys_access_nums_size = sorted_keys_access_nums_.size();
        
        size_t sorted_keys_access_nums_i = 0;

        for ( ; sorted_keys_access_nums_i < sorted_keys_access_nums_size; sorted_keys_access_nums_i++)
        {
            if (sorted_keys_access_nums_[sorted_keys_access_nums_i].key == accessed_keys_[accessed_keys_i])
                break;
        }

        if (sorted_keys_access_nums_i == sorted_keys_access_nums_size)
            sorted_keys_access_nums_.push_back({accessed_keys_[accessed_keys_i], 0});

        sorted_keys_access_nums_[sorted_keys_access_nums_i].num++;
    }

    RLSU_INFO("sorted_keys_access_nums_ = ");
    for (int i = 0;  i < sorted_keys_access_nums_.size(); i++)
        RLSU_LOG("{} ", sorted_keys_access_nums_[i].key);

    RLSU_LOG("\n");

    std::sort(sorted_keys_access_nums_.begin(), sorted_keys_access_nums_.end(), 
        [](const AccessNum& a, const AccessNum& b) 
        { 
            return a.num > b.num; 
        } );
}


template <typename PageT, typename KeyT>
PageT CasherIdeal<PageT, KeyT>::CashPageIfFreq_(const KeyT& key)
{
    const size_t sorted_keys_access_nums_size = sorted_keys_access_nums_.size();

    size_t i = 0;

    for ( ; i < capacity_ && i < sorted_keys_access_nums_size; i++)
    {
        if (sorted_keys_access_nums_[i].key == key)
            break;
    }

    if (i != sorted_keys_access_nums_size && cashe_.Size() < capacity_)
    {
        PageT hot_page = UserGetPageUncashed_(key);

        cashe_.PushBack(hot_page, key);
        return cashe_.Back().page;
    }

    else
    {
        return UserGetPageUncashed_(key);
    }    
}


template <typename PageT, typename KeyT>
PageT CasherIdeal<PageT, KeyT>::GetPage(KeyT key)
{
    if (cashe_.Contains(key))
    {
        last_cashe_hitted = true;
        return cashe_.Get(key).page;
    }

    else
    {
        last_cashe_hitted = false;

        return CashPageIfFreq_(key);
    }
}

}