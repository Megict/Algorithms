#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <time.h>

int main(int argc, char** argv) {
    if(argc < 3) {
        printf("NEA\n");
        return -2;
    }

    FILE* fTest = fopen(argv[1],"r+");
    FILE* fRes = fopen(argv[2],"r+");

    std::cout << argv[1] << " " << argv[2] << std::endl;

    if(fTest == NULL || fRes == NULL) {
        printf("WA\n");
        return -1;
    }

    long long p1;
    long long p2;
    char trash;
    char opr;
    printf("beg\n");
    long long counter = 0;
    while (fscanf(fTest, "%lld",&p1) != EOF) {
        counter++;
        printf("%lld ",counter);

        fscanf(fTest, "%c",&trash);
        fscanf(fTest, "%lld",&p2);
        fscanf(fTest, "%c",&trash);
        fscanf(fTest, "%c",&opr);
        fscanf(fTest, "%c",&trash);

        long long res;
        fscanf(fRes, "%lld",&res);


        if(opr == '+') {
            if(p1 + p2 == res) {
                printf("OK\n");
            }
            else {
                printf("ERROR\n");
        printf("args:\n %lld %lld\n have res: %lld\n",p1,p2,res);
                printf("expected: %lld\n",p1 + p2);
                return 0;
            }
        }

        if(opr == '-') {
            if(p1 - p2 == res) {
                printf("OK\n");
            }
            else {
                printf("ERROR\n");
        printf("args:\n %lld %lld\n have res: %lld\n",p1,p2,res);
                printf("expected: %lld\n",p1 - p2);
                return 0;
            }
        }

        if(opr == '*') {
            if(p1 * p2 == res) {
                printf("OK\n");
            }
            else {
                printf("ERROR\n");
        printf("args:\n %lld %lld\n have res: %lld\n",p1,p2,res);
                printf("expected: %lld\n",p1 * p2);
                return 0;
            }
        }

        if(opr == '/') {
            if(p1 / p2 == res) {
                printf("OK\n");
            }
            else {
                printf("ERROR\n");
        printf("args:\n %lld %lld\n have res: %lld\n",p1,p2,res);
                printf("expected: %lld\n",p1 / p2);
                return 0;
            }
        }
    }
}