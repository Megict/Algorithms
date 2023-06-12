#include <iostream>

int main(int argc, char** argv) {
    int n = atoi(argv[1]);
    int m = n + rand()%n;
    std::cout << m << " " << n << std::endl;
    for(int j=0; j<m;++j) {
        for(int i=0;i<n;++i) {
            std::cout << rand()%100 << " ";
        }
        std::cout << rand()%100 << "\n";
    }
}