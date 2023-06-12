#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

#define DEBUG false

struct vertCon {
    bool marked;
    std::vector<int> connectsTo;

    vertCon() {
        marked = false;
    }
};

int rad(int digit, int pos) {
    int del = (int)pow(10, pos);
    int dif = (int)pow(10, pos - 1);

    if (digit % dif == digit) {
        return -1;
    }

    int res = (digit % del) / dif;
    return res;
}

void radixSort(std::vector<int>& input) { 
    std::vector<int> prom(input.size());
    int countArr[11];
    for (int i = 0; i < 11; ++i)
        countArr[i] = 0;

    int digit = 1;

    while (countArr[0] != (int)input.size()) {


        for (int i = 0; i < 11; ++i)
            countArr[i] = 0;

        for (int i = 0; i < (int)input.size(); ++i) {
            countArr[rad(input[i], digit) != -1 ? rad(input[i], digit) + 1 : 0] += 1;
        }

        int k = 0;
        int flp = 0;
        for (int i = 0; i < 11; ++i) {
            k = countArr[i];
            countArr[i] = flp;
            flp = k;
        }
        for (int i = 2; i < 11; ++i) {
            countArr[i] += countArr[i - 1];
        }

        for (int i = 0; i < (int)input.size(); ++i) {
            int rdn = rad(input[i], digit);
            prom[countArr[rdn != -1 ? rdn + 1 : 0]] = input[i];
            countArr[rdn != -1 ? rdn + 1 : (0)] ++;
        }

        input = prom;
        for(size_t i = 0; i < prom.size(); ++i) {
            prom[i] = 0;
        }

        digit++;
    }
}

std::vector<int> passComponent(int start, std::vector<vertCon>& vs) {
    std::vector<int> res = std::vector<int>();
    res.push_back(start);

    vs[start].marked = true;

    for (int i = 0; i < (int)vs[start].connectsTo.size(); ++i) {
        if (!vs[vs[start].connectsTo[i]].marked) {            
            std::vector<int> next = passComponent(vs[start].connectsTo[i], vs);
            for(int a : next) {
                res.push_back(a);
            }
        }
    }

    return res;
}

int main() {
    auto tm1 = std::chrono::steady_clock::now();
    int vertCount, pathCount;
    std::cin >> vertCount >> pathCount;

    std::vector<vertCon> verts(vertCount + 1);

    int vertA, vertB;
    for (int i = 0; i < pathCount; ++i) {
        std::cin >> vertA >> vertB;

        verts[vertB].connectsTo.push_back(vertA);
        verts[vertA].connectsTo.push_back(vertB);
    }

    std::vector<std::vector<int>> components;
    
    for (int i = 1; i < vertCount + 1; ++i) {

        if (verts[i].marked) {
            continue;
        }

        std::vector<int> component = passComponent(i, verts);

        components.push_back(component);
    }

    std::cerr << "comps sum: " << components.size() << std::endl;
    for (size_t i = 0; i < components.size(); ++i) {
        radixSort((components[i]));
        for (size_t j = 0; j < components[i].size(); ++j) {
            printf("%d ", ((components[i]))[j]);
        }
        printf("\n");
    }
    auto tm2 = std::chrono::steady_clock::now();
    auto delt = tm2 - tm1;
    std::cerr << "test complete| " << std::chrono::duration_cast<std::chrono::milliseconds>(delt).count()<< "ms" <<std::endl;
}