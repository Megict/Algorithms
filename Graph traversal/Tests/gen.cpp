#include <iostream>
#include <vector>
#include <time.h>

#define DEBUG false

int main(int argc, char** argv) {
    int n = 0s;
    if(argc != 2) {
        return -1;
    }
    else {
        n = atoi(argv[1]);
    }
    std::cerr << n << std::endl;
    printf("%d %d\n", n, n);
    for(int i=0;i<n; ++i) {
        int a = rand()%n;
        int b = rand()%n;

        while(b == a) {
                b = rand()%n;
        }

        printf("%d %d\n", a, b);
    }
    return 0;
}