#include <iostream>
#include <vector>
#include <list>
#include <time.h>


int main(int argc, char** argv) {
    srand(time(NULL));
    for(int k =0; k <1; ++k) {
        int m = rand()%(atoi(argv[1])/10) + atoi(argv[1]);
        int n = rand()%(atoi(argv[1])/10) + atoi(argv[1]);
        printf("%d %d\n",m,n);

        for(int i =0; i<m; ++i) {
            for(int j=0; j <n; ++j) {
                printf("%d ", rand()%2);
            }
            printf("\n");
        }
    }
    return 0;
}