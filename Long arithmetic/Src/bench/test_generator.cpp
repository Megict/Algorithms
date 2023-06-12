#pragma warning(disable : 4996)
#include <vector>
#include <string>
#include <iostream>
#include <time.h>

int main(int argc, char** argv) {
    srand(time(NULL));

    for(int i = 0; i< (argc > 1 ? atoi(argv[1]) : 100000) ; ++i) {//5х0
        for(long long j = 0;j < 5;++j) {
            long long r = rand()%100000000; //9х0
            printf("%lld",r);
        }
        printf("\n");
        for(long long j = 0;j < 5;++j) {
            long long r = rand()%10000; //5х0
            printf("%lld",r);
        }
        printf("\n");

        int p = rand()%4 + 1;
        if(p == 1) {
            printf("+\n");
        }
        if(p == 2) {
            printf("-\n");
        }
        if(p == 3) {
            printf("*\n");
        }
        if(p == 4) {
            printf("/\n");
        }
        if(p == 5) {
            printf(">\n");
        }
        if(p == 6) {
            printf("<\n");
        }
        if(p == 7) {
            printf("=\n");
        }
        if(p == 8) {
            printf("+\n");
        }
    }
}