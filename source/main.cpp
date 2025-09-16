#include "RLogSU/logger.hpp"
// #include "RLogSU/error_handler.hpp"

#include "CashLib/casher.hpp"
#include "CashLib/page_list.hpp"


int SlowGetPage(int key)
{
    RLSU_INFO("called SlowGetPage");\

    // RLSU_ASSERT(false);
    return 7;
}

int main()
{
    RLSU_INFO("START!");

    CashLib::PageList<std::string> pl;
    // pl.Front();

    // pl.PushBack("first", 2);
    // pl.PushFront("second", 3);

    // std::cout << pl.Get(2).page;
    // std::cout << pl.Get(3).page;

    
    // ERROR_HANDLE(casher.GetPage(1));
    // casher.Dump();
    // ERROR_HANDLE(casher.GetPage(2));
    // casher.Dump();
    // ERROR_HANDLE(casher.GetPage(3));
    // casher.Dump();
    // ERROR_HANDLE(casher.GetPage(1));
    // casher.Dump();
    // ERROR_HANDLE(casher.GetPage(1));
    // casher.Dump();
    // ERROR_HANDLE(casher.GetPage(3));
    // casher.Dump();
    
    int capa = 0;
    std::cin >> capa;
    
    int num = 0;
    std::cin >> num;
    
    CashLib::Casher2Q<int> casher(capa, SlowGetPage);
    
    int kit_counter = 0;

    for (int i = 0; i < num; i++)
    {
        int key = 0;
        std::cin >> key;
        casher.GetPage(key);

        if (casher.last_cashe_hitted)
            kit_counter++;
    }

    std::cout << kit_counter;

    RLSU_INFO("END!");
}