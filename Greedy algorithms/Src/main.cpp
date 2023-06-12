#include <iostream>
#include <vector>
#include <map>
#include <time.h>
#include <chrono>

#define DEBUG false 

struct TMatrix {
    std::vector<std::vector<double>> Body;
    int M, N;

    TMatrix(int m, int n) : M(m), N(n)
    {
        Body = std::vector<std::vector<double>>(m);

        for (int i = 0; i < m; ++i) {
            Body[i] = std::vector<double>(n , 0);
        }
    }

    void FillMatrix(std::multimap<double, int> &inter/*заполняется между заполнениями рядов*/) {
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                std::cin >> Body[i][j];
            }

            double intV;
            std::cin >> intV;
            inter.emplace(std::pair<double, int>(intV, i));
        }
    }

    int Echelonise() {
        int startRow = 0;
        for (int k = 0; k < N && startRow < M; ++k) {

            if (Body[startRow][k] == 0) {//замена строки, если начинается на 0
                for (int j = startRow; j < M; ++j) {
                    if (Body[j][k] != 0) {
                        std::vector<double> f = Body[startRow];
                        Body[startRow] = Body[j];
                        Body[j] = f;
                        break;
                    }
                }
            }

            if (Body[startRow][k] == 0) {
                //продолжаем, но startRow не меняем
                if (DEBUG) std::cout << "[k|sc: " << k << " | " << startRow << " continuing\n";
                continue;
            }


            for (int i = startRow + 1; i < M; i++) { //основной цикл
                double factor = Body[i][k] / Body[startRow][k];
                if(DEBUG) std::cout << "[i|k|sc: " << i << " | " << k << " | " << startRow << " factor: " << factor << " sizes: " << M << " " << N << std::endl;
                for (int j = k; j < N; ++j) {
                    Body[i][j] -= Body[startRow][j] * factor;
                }
            }

            startRow++;
        }

        int lastRow = M - 1;
        for (int i = 0; i < N; ++i) {
            if (Body[lastRow][i] > 0.00001 || Body[lastRow][i] < -0.00001) {
                //система лнз
                return 0;
            }
        }

        //система лз
        return 1;
    }
};

int main() {

    int m, n;
    std::cin >> m >> n;

    std::multimap<double, int> costs;
    std::map<int, int> res;
    TMatrix test(m,n);

    test.FillMatrix(costs);

    /*
        1. выбрали строку с мин. стоимостью
        2. выбираем строку ЛНЗ с выбранными ранее строками с мин. стоимостью
        3. пока не останется строк для выбора.
        4. если система имеет одно решение, то нашли подходящий набор. Иначе – решений нет.
    */

    long long cost = 0;
    TMatrix resMatr = TMatrix(0, test.N);

    if (test.N == 0) {
        std::cout << "-1" << std::endl;
        return 0;
    }

    auto tm1 = std::chrono::steady_clock::now();

    for (std::pair<int, double> iter : costs) {
        //берем следующую по цене добавку

        resMatr.Body.push_back(test.Body[iter.second]);
        resMatr.M++;
        //добавляем ее в матрицу   

        TMatrix checkMatr = resMatr;
        int LinDep = checkMatr.Echelonise();

        if (LinDep == 0) {
            //если получилась линейно независимая система, то оставляем строку
            cost += iter.first;
            res.emplace(std::pair<int, int>(iter.second, iter.second));
        }
        else {
            //если нет, то убираем
            resMatr.M--;
            resMatr.Body.pop_back();
        }


        if (resMatr.M == resMatr.N) {
            //если собрали решаемую систему лин. уравнений, то останавливаемся.
            break;
        }
    }

    if (resMatr.M != resMatr.N) {
        std::cout << "-1" << std::endl;
            
        auto tm2 = std::chrono::steady_clock::now();
        auto delt = tm2 - tm1;
        std::cerr << "test complete| " << std::chrono::duration_cast<std::chrono::milliseconds>(delt).count()<< "ms" <<std::endl;
        return 0;
    }

    for (auto iter : res) {
        std::cout << iter.first + 1 << " ";
    }
    std::cout << std::endl;
    
    auto tm2 = std::chrono::steady_clock::now();
    auto delt = tm2 - tm1;
    std::cerr << "test complete| " << std::chrono::duration_cast<std::chrono::milliseconds>(delt).count()<< "ms" <<std::endl;
}