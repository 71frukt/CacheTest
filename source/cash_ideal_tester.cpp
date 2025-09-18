#include "RLogSU/logger.hpp"

#include "CashIdeal/casher.hpp"
#include <cstddef>
#include <ostream>
#include <vector>


int SlowGetPage(int key)
{
    RLSU_INFO("called SlowGetPage");\

    return 7;
}

int main()
{
    RLSU_INFO("START!");

    int capa = 0;
    std::cin >> capa;
    
    int num = 0;
    std::cin >> num;
    
    std::vector<int> accessed_keys;

    for (int i = 0; i < num; i++)
    {
        int key = 0;
        std::cin >> key;
        accessed_keys.push_back(key);
    }

    CashIdeal::CasherIdeal<int> casher(capa, SlowGetPage, accessed_keys);
    
    size_t hit_counter = 0;

    for (int i = 0; i < num; i++)
    {
        casher.GetPage(accessed_keys[i]);

        if (casher.last_cashe_hitted)
            hit_counter++;
    }

    std::cout << hit_counter << std::endl;

    RLSU_INFO("END!");
}