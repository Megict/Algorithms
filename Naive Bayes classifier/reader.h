#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#define DEBUG false

template <class T>
int ReadLine(std::vector<T>* adr, std::ifstream& inp, bool blank) {
    bool reachedEnd = false;
    std::string word;
    for (;;) { //считываем строки
        char liter;

        if (inp.peek() == EOF) {
            reachedEnd = true;
            break;
        }

        inp.get(liter);

        if (DEBUG) std::cout << "|ur:";
        while (liter == ' ' || liter == '\n' || liter == '\t') {
            if (DEBUG) std::cout << liter;

            if (inp.peek() == EOF) {
                reachedEnd = true;
                break;
            }

            inp.get(liter);
        }
        if (DEBUG) std::cout << ":ur|";

        while (liter != ' ' && liter != '\n' && liter != '\t') {
            //заполнение слова
            if (DEBUG) std::cout << liter;
            word.push_back(liter);

            if (inp.peek() == EOF) {
                reachedEnd = true;
                break;
            }

            inp.get(liter);
        }

        if (DEBUG) std::cout << "|\n";
        //заполнение массива токенов (сырых признаков)
        if (word.size() > 0) {
            if (!blank) adr->push_back(T(word));
        }
        word.clear();

        if (liter == '\n') {
            if (DEBUG) std::cout << "EOL reached\n";
        }
        if (liter == '\n' || reachedEnd) {
            return reachedEnd;
            //если остановили считываение, дойдя на сноса строки вернем false
            //если дошли до конца файла - true
        }
    }
}