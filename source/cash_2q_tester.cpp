#include "RLogSU/logger.hpp"
// #include "RLogSU/error_handler.hpp"

#include "Cash2QLib/casher.hpp"
#include "Cash2QLib/page_list.hpp"


int SlowGetPage(int key)
{
    RLSU_INFO("called SlowGetPage");\

    // RLSU_ASSERT(false);
    return 7;
}

int main()
{
    RLSU_INFO("START!");

    Cash2QLib::PageList<std::string> pl;

    int capa = 0;
    std::cin >> capa;
    
    int num = 0;
    std::cin >> num;
    
    Cash2QLib::Casher2Q<int> casher(capa, SlowGetPage);
    
    int kit_counter = 0;

    for (int i = 0; i < num; i++)
    {
        int key = 0;
        std::cin >> key;
        casher.GetPage(key);

        if (casher.last_cashe_hitted)
            kit_counter++;
    }

    std::cout << kit_counter << std::endl;

    RLSU_INFO("END!");
}