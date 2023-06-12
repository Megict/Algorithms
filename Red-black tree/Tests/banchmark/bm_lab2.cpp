#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <map>
#include <chrono>

class Key {
public:
    char* key;

    Key(char* inp) {
        key = (char*)malloc(260);
        strcpy(key,inp);
    }

    // ~Key() {
    //     free(key);
    // }
};


int strequal (const char* lhs, const char* rhs) { //операторы регистронезависимого сравненияя для std::string
    //printf("%s|%s\n",lhs,rhs);
    if(lhs == nullptr || rhs == nullptr) {
        if(lhs == nullptr) {
            return -1;
        }
        if(rhs == nullptr) {
            return 1;
        }
    }

    size_t i=0;
    while (lhs[i]!=0 || rhs[i]!=0) {
        if (lhs[i] == rhs[i]) {
            ++i;
            continue;
        }
        else {
            char cmp_l = lhs[i];
            char cmp_r = rhs[i];
            if(lhs[i] >= 'A' && lhs[i] <= 'Z') {
                cmp_l = lhs[i] + ('a' - 'A');
            } 
            if(rhs[i] >= 'A' && rhs[i] <= 'Z') {
                cmp_r = rhs[i] + ('a' - 'A');
            } 
            
            if(cmp_l < cmp_r) {
                return -1;
            } else 
            if(cmp_l > cmp_r) {
                return 1;
            } else {
                ++i;
                continue;
            }
        }
    }
    if(lhs[i]!=0 || rhs[i]!=0)
    {
        if(lhs[i]==0) {
            return -1;
        } 
        if(rhs[i]==0) {
            return 1;
        }
    }

    return 0;
}

struct Compare {
    bool operator()(const Key lhs,const Key rhs) const {
        return (strequal(lhs.key,rhs.key) == -1);
    }
};

int main() {
    std::map<Key,unsigned long long,Compare> maintree; 

    char *inpstr = (char*)calloc(260,sizeof(char)); 
    char *word = (char*)calloc(260,sizeof(char)); 
    unsigned long long inpval;

    if(inpstr == NULL || inpstr == nullptr || word == NULL || word == nullptr) {
        printf("ERROR: allocation error\n");
        return -1;
    }

    auto tm0 = std::chrono::steady_clock::now();
    while (scanf("%s", inpstr) != EOF) {
        if(strlen(inpstr)>256) {
            printf("ERROR: uncorrect input\n");
            continue;
        }
        if (inpstr[0] == '+') {
            if(scanf("%s %llu", word, &inpval) == EOF) {
                break;
            }
            if(strlen(word)>256) {
                printf("ERROR: uncorrect input\n");
                continue;
            }
            auto mrk = maintree.insert(std::pair<Key,unsigned long long>(Key(word), inpval));
            if(mrk.second) {
                printf("OK\n");
            }
            else {
                printf("Exist\n");
            }
        }
        else 
        if (inpstr[0] == '-') {
            if(scanf("%s", word) == EOF) {
                break;
            }
            if(strlen(word)>256) {
                printf("ERROR: uncorrect input\n");
                continue;
            }
            auto mrk = maintree.erase(Key(word));
            if(mrk == 0) {
                printf("NoSuchWord\n");
            }
            else {
                printf("OK\n");
            }
        }
        else {
            auto res = maintree.find(Key(inpstr));
            if (res != maintree.end()) {
                printf("OK: %llu\n",res->second);
            }
            else {
                printf("NoSuchWord\n");
            }
        }
    }
    auto tm2 = std::chrono::steady_clock::now();
    maintree.insert(std::pair<Key,unsigned long long>(Key("word"), 10000));
    auto tm1 = std::chrono::steady_clock::now();
    auto delt = tm1 - tm2;
    std::cerr << "cast complete| " << std::chrono::duration_cast<std::chrono::milliseconds>(delt).count()<< "ms" <<std::endl;
    auto delt = tm1 - tm0;
    std::cerr << "inp complete| " << std::chrono::duration_cast<std::chrono::milliseconds>(delt).count()<< "ms" <<std::endl;

    free(inpstr);
    free(word);

    return 0;
}

/*
rm asdfghjli && 
rm asdfghjkliasdfhjkliasdfghjkliasdfghjkliasdfghjkliasdfghjkliasdfghjkliasdfghjkliasdfghjkliasdfghjkli && 
rm i && ./gen.out && ./gen-2.out && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt 
./lab2ex < test_of_absolutia > res-test&& tail -n5 valgrind-out.txt && 
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt 
./lab2ex < second_test_of_absolutia > res-test && tail -n5 valgrind-out.txt 
*/


/*
rm asdfghjli && rm asdfghjkliasdfhjkliasdfghjkliasdfghjkliasdfghjkliasdfghjkliasdfghjkliasdfghjkliasdfghjkliasdfghjkli && rm i && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./lab2ex < test_of_absolutia > res-test&& tail -n5 valgrind-out.txt && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./lab2ex < second_test_of_absolutia > res-test && tail -n5 valgrind-out.txt 
*/