#include <iostream>

int main (int argc, char** argv) {
    srand(time(NULL));

    if(argc!= 2) {
        return -1;
    }

    for(int i = 0; i < 1; ++i) {
        printf("%c", rand() % ('z' - 'a') + 'a');
    }
    printf("\n");
    for(int i = 0; i < atoi(argv[1]); ++i) {
        printf("%c", rand() % ('z' - 'a') + 'a');
    }
    printf("\n");

    return 0;
}