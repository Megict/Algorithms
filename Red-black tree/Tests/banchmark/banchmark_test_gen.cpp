#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <ctime>

const char BLACK = 1;
const char RED = 2;

int main() {
    srand(time(NULL));
    char filename_1[10];
    char filename_2[100];
    char filename_3[2];


    std::ofstream fout("test_of_absolutia", std::ios::out);
    for(unsigned long long i=0;i<(unsigned long long)1000000;++i) {
        char kate = rand()%4;
        if(kate >1)
        {
            fout << "+ ";
            for(int j=0;j<2;++j) {
                fout << (char)(rand()%('z'-'a') + 'a');
            }
            fout << " ";
            fout << (unsigned long long)rand()%(unsigned long long)18446744073709551615;
            fout << "\n";
        } else 
        if(kate == 0)
        {
            fout << "- ";
            for(int j=0;j<2;++j) {
                fout << (char)(rand()%('z'-'a') + 'a');
            }
            fout << "\n";
        } else 
        if(kate == 1)
        {
            for(int j=0;j<2;++j) {
                fout << (char)(rand()%('z'-'a') + 'a');
            }
            fout << "\n";
        }
    }
    std::cout<<"done\n";
    return 0;
}