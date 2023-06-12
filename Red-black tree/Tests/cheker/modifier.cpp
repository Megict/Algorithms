#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <ctime>

const char BLACK = 1;
const char RED = 2;

int main() {
    std::ifstream fin("second_test_of_absolutia", std::ios::out);
    std::ofstream fout("second_test_of_absolutia_mod_two", std::ios::out);

    for(unsigned long long i=0;i<(unsigned long long)10000;++i) {
        char a;
        fin.read(&a,1); 
        int mrk = 0;
        if(a != '!') {
            mrk = 1;
        }
        while(a != '\n') {
            if(mrk == 1) {
                fout.write(&a,1);
                fin.read(&a,1);
            }
            else 
            {
                fin.read(&a,1);
            }
        }
        if(mrk == 1) {
            fout.write(&a,1);
        }
    }
    std::cout<<"done\n";
    return 0;
}