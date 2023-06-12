#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <ctime>
#include <chrono>

const char BLACK = 1;
const char RED = 2;

int main(int argc, char** argv) {
    if(argc < 3) {
        return 0;
    }

    unsigned long long tmc = atoi(argv[1]);
    unsigned long long wcc = atoi(argv[2]);
    
    srand(time(NULL));
    

    std::ofstream fout("test_a", std::ios::out);
    for(unsigned long long i=0;i < tmc;++i) {

        char kate = rand()%4;
        if(kate >0)
        {
            fout << (unsigned long long)rand()%(unsigned long long)20;

            fout << " ";
        } else 
        if(kate == 0)
        {
            fout << "? ";
        }
    }

    
    auto tm0 = std::chrono::steady_clock::now();//замеряется время на генерацию текста

    char kate = rand()%4;

    for(unsigned long long i = 0; i < wcc; ++i) {

        kate = rand()%4;
        if(kate > 0)
        {
              fout << (unsigned long long)rand()%(unsigned long long)20;

            fout << " ";
        } else 
        if(kate == 0)
        {
            fout << "\n";
        }
    }

    auto tm1 = std::chrono::steady_clock::now();
    auto delt = tm1 - tm0;
    std::cerr << "done| " << std::chrono::duration_cast<std::chrono::milliseconds>(delt).count()<< "ms" <<std::endl;
    return 0;
}