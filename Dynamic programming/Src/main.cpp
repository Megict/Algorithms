#include <iostream>
#include <vector>
#include <list>

const bool DEBUG = true;

struct TCell {
    int Len; //длина последовательности нулей нач. в этой ячейке
    int Vlen; //длина верт. последовательности нулей, в которую входит ячейка 


    TCell() {
        Len = 0;
        Vlen = 0;
    }

    TCell(int l, int v) {
        Len = l;
        Vlen = v;
    }
};

int adcSteps = 0;

int main() {
    int m, n;
    std::cin >> n >> m;
    if(m == 0 || n == 0) {
        printf("0\n");
        return 0;
    }
    std::vector<std::vector<char>> matrix(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            char a = 0;
            std::cin >> a;
            if (a == '0') {
                a = 0;
            }
            else {
                a = 1;
            }
            matrix[i].push_back(a);
        }
    }

    //заполнили
    //разбираем матрицу по рядам
    auto tm1 = std::chrono::steady_clock::now();

    //заполняем нулевой ряд
    std::vector<TCell> prev(m, TCell());

    if(m > 0)prev[m - 1] = TCell();
    for (int i = m - 2; i >= 0; --i) {
        prev[i] = TCell();
    }
    
    int Sm = 0;
    //обработка рядов
    for (int k = 0; k < n; ++k) {
        std::vector<TCell> next(m, TCell());
        if(m > 0) next[m - 1] = ((matrix[k][m - 1] == 1) ? TCell() : TCell(1, 1));
        for (int i = m - 2; i >= 0; --i) { //заполнение форм.ряда

            if (matrix[k][i] == 0) {
                if (next[i + 1].Len == 0) {
                    next[i] = TCell(1, 1);
                }
                else {
                    next[i] = next[i + 1];
                    next[i].Len++;
                }

            }
            else {
                next[i] = TCell();
            }

        }

        /*
            есть карта высот, идем по ней, записываем в массив число последовательно идущих высот, покуда некая из них не прервет свое действие
            коль высота прервала свое действие, вычислим площадь прямоугольника, образованного ею и ежели она больше, чем площадь наибольшего из ранее образованных
            сей высотой прямоугольников, запишем ее.
            Продолжаем, покуда все высоты не пркратят свое действие.
            Взглянем на список площадей и вычислим наибольшую из них.

            Оптимизации:
                1. Будем записывать площади не для каждой высоты по отдельности, а для всех сразу.
                2. Чую я, можно сделать за О(m) но не вразумию как
        */
        //проход по ряду
        
        std::vector<int> heighths(n + 1, 0);
        std::list<int> hm;
        if(DEBUG) printf("next row! (Sn = %d)\n", Sm);
        for (int i = m - 1; i >= 0; --i) {//обработка с учетом предыдущего ряда

            if (next[i].Len > 0 && prev[i].Len > 0) {
                next[i].Vlen = prev[i].Vlen + 1;
            }

            if (i == m - 1) {
                if (heighths[next[i].Vlen] == 0 && next[i].Vlen != 0) {
                    hm.push_back(next[i].Vlen);
                }
                heighths[next[i].Vlen]++;
                continue;
            }

            //считаем площади мы только на конце поля действия высоты
            if (next[i].Vlen >= next[i + 1].Vlen && next[i].Vlen != 0) {
                if (heighths[next[i].Vlen] == 0) {
                    hm.push_back(next[i].Vlen);
                }
                heighths[next[i].Vlen]++;
            }
            else {
                if (DEBUG) printf("\t |fall of height from %d to %d\n", next[i + 1].Vlen, next[i].Vlen);
                if (heighths[next[i + 1].Vlen] * next[i + 1].Vlen > Sm) {
                    Sm = heighths[next[i + 1].Vlen] * next[i + 1].Vlen;
                }
                //текущую высоту мы проверили, нашли площадь, теперь добавляем ее кол-во к кол-ву высоты пред. (оно в ней не учтено)
                //текущую высоту (число ее столбцов) зануляем

                if(hm.size() != 0) {
                    hm.pop_back();
                    if(DEBUG)if(hm.size() > 0)printf("\t|new top = %d\n", hm.back());
                }
                
                bool adedNow = false;
                if(hm.size() != 0 && hm.back() > next[i].Vlen) { //передаем кол-во вхожд. срезанной высоты
                    heighths[hm.back()] += heighths[next[i + 1].Vlen];
                }
                else {
                    heighths[next[i].Vlen] += heighths[next[i + 1].Vlen];
                    adedNow = true;
                }

                heighths[next[i + 1].Vlen] = 0;
                //проверка всех высот, срезанных вместе с первой
                while (!hm.empty() && hm.back() > next[i].Vlen) {

                    adcSteps++;
                    if (heighths[hm.back()] * hm.back() > Sm) {
                        Sm = heighths[hm.back()] * hm.back();
                    }

                    int j = hm.back();
                    //зануляем проверенную высоту, идем дальше
                    hm.pop_back();
                    if (!hm.empty() && hm.back() > next[i].Vlen) {
                        //если в списке активных высот еще остались высоты, которые будут срезаны, то 
                        //количество срезанной сейчас высоты перейдет к ним.
                        heighths[hm.back()] += heighths[j];
                        if(DEBUG) printf("\t |count transfered from %d to %d\n",j,hm.back());
                    }
                    else {
                        //если таких высот больше не осталось, то надо сохранить количество оставшихся высот.
                        if(DEBUG)if (!hm.empty()) printf("\t |count transfered from %d to %d\n",j,next[i].Vlen);
                        heighths[next[i].Vlen] += heighths[j];
                        adedNow = true;
                    }
                    heighths[j] = 0;
                }
                
                //добпаляем текущую высоту
                if ((heighths[next[i].Vlen] == 0 || adedNow) && next[i].Vlen != 0 && hm.back()!=next[i].Vlen) {
                    hm.push_back(next[i].Vlen);
                }
                heighths[next[i].Vlen]++;
            }

            if(DEBUG) {
                for(int v=0;v<m+1;++v) {
                    printf("%d ",heighths[v]);
                }
                printf("||%d(%d)\n",(int)hm.size(),hm.size() != 0 ? hm.back() : -1);
            }

        }

        if (DEBUG) {
            printf("|");
            for (int j = 0; j < n + 1; ++j) {
                printf("%d ", heighths[j]);
            }
            printf("|\n");
            if (!hm.empty()) printf("| %d <hmm|\n", hm.back());
            printf("row fin (Sn = %d,hm-B = %d)\n", Sm, (!hm.empty() ? hm.back() : -1));
        }

        //закончили ряд, если остались высоты, значит, они все срезаются границей матрицы
        //учтем их всех
        while (!hm.empty()) {

            adcSteps++;
            if (heighths[hm.back()] * hm.back() > Sm) {
                Sm = heighths[hm.back()] * hm.back();
            }

            int j = hm.back();
            //зануляем проверенную высоту, идем дальше
            hm.pop_back();
            if(!hm.empty()) heighths[hm.back()] += heighths[j];
            heighths[j] = 0;
        }
        
        prev = next;
    }

    auto tm2 = std::chrono::steady_clock::now();
    auto delt = tm2 - tm1;
    std::cerr << "test complete| " << std::chrono::duration_cast<std::chrono::milliseconds>(delt).count()<< "ms" <<std::endl;


    printf("%d\n",Sm);
    if(DEBUG)printf("ad: %d\n", adcSteps);

}


