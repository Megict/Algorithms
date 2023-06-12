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

    /*for(int j=0;j<10;++j) {
        filename_1[j] = (char)(rand()%('z'-'a') + 'a');
    }
    filename_1[9] = 0;


    for(int j=0;j<100;++j) {
        filename_2[j] = (char)(rand()%('z'-'a') + 'a');
    }
    filename_2[99] = 0;

    for(int j=0;j<2;++j) {
        filename_3[j] = (char)(rand()%('z'-'a') + 'a');
    }
    filename_3[1] = 0;*/

    strcpy(filename_1,"asdfghjli");
    strcpy(filename_2,"asdfghjkliasdfhjkliasdfghjkliasdfghjkliasdfghjkliasdfghjkliasdfghjkliasdfghjkliasdfghjkliasdfghjkli");
    strcpy(filename_3,"i");


    std::ofstream fout("second_test_of_absolutia", std::ios::out);
    for(unsigned long long i=0;i<(unsigned long long)10000;++i) {
        char kate = rand()%4;
        if(kate == 10)
        {
            fout << "+ ";
            for(int j=0;j<2;++j) {
                fout << (char)(rand()%('z'-'a') + 'a');
            }
            fout << " ";
            fout << (unsigned long long)rand()%(unsigned long long)18446744073709551615;
            fout << "\n";
        } else 
        if(kate == 2 || kate == 0)
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

        if(rand()%20 == 1) {
            fout << "! Save " << filename_1 << "\n";
        }

        if(rand()%20 == 1) {
            fout << "! Load " << filename_1 << "\n";
        }

        if(rand()%50 == 1) {
            fout << "! Save " << filename_2 << "\n";
        }

        if(rand()%50 == 1) {
            fout << "! Load " << filename_2 << "\n";
        }

        if(rand()%100 == 1) {
            fout << "! Save " << filename_3 << "\n";
        }

        if(rand()%100 == 1) {
            fout << "! Save " << filename_3 << "\n";
        }
    }
    std::cout<<"done\n";
    return 0;
}