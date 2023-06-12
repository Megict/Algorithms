#pragma warning(disable : 4996)
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <fstream>


//без дополнительного вывода
bool SILENT = true;
//с подробным отчетом о работе анализатора
bool ADVANCED = false;
#include "analyser.h"
#include "reader.h"

std::string mode = "";
std::string algo = "";

int main(int argc, char** argv) {
    std::string inpFName = "";
    std::string outFName = "";
    std::string stsFName = "";

    //парсинг аргументов
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--input") == 0) {
            if (i + 1 == argc) {
                std::cerr << "input filename not given\n";
                return -1;
            }

            inpFName = argv[i + 1];
            i += 1; 
            continue;
        }

        if (strcmp(argv[i], "--output") == 0) {
            if (i + 1 == argc) {
                std::cerr << "output filename not given\n";
                return -2;
            }

            outFName = argv[i + 1];
            i += 1;
            continue;
        }

        if (strcmp(argv[i],"--stats") == 0) {
            if (i + 1 == argc) {
                std::cerr << "stats filename not given\n";
                return -3;
            }

            stsFName = argv[i + 1];
            i += 1;
            continue;
        }

        if (strcmp(argv[i],"learn") == 0) {
            mode = "learn";
            continue;
        }
        
        if (strcmp(argv[i],"classify") == 0) {
            mode = "classify";
            continue;
        }

        if (strcmp(argv[i], "--verbal") == 0) {
            SILENT = false;
            continue;
        }

        if (strcmp(argv[i], "--advanced") == 0) {
            ADVANCED = true;
            continue;
        }

        std::cerr << "unknown argument " << argv[i] << std::endl;
        int l = 0;
        while (argv[i][l] != 0) {
            printf("[%d|%c]\n", argv[i][l], argv[i][l]);
            ++l;
        }
        return -10;
    }

    //проверка корректности комбинаций аргументов
    if (mode == "") {
        std::cerr << "mode not given\n";
        return -20;
    }
    if (inpFName == "" || outFName == "") {
        std::cerr << "i/o filename not given\n";
        return -21;
    }
    if (mode == "classify" && stsFName == "") {
        std::cerr << "i/o filename not given\n";
        return -21;
    }

    //открытие файлов
    std::ifstream inp;
    std::ofstream out;
    std::ifstream sts;
    inp.open(inpFName, std::ios_base::in);
    
    if (mode == "learn") {
        out.open(outFName, std::ios_base::out | std::ios_base::binary);
    }
    else {
        out.open(outFName, std::ios_base::out);
        sts.open(stsFName, std::ios_base::out | std::ios_base::binary);
    }

    //проверка корректности открытия файлов
    if (!inp.is_open()) {
        std::cerr << "error opening inp file | name: " << inpFName << std::endl;
        return -30;
    }
    else {
        if (!SILENT) std::cout << "openting inp file complete" << std::endl;
    }

    if (!out.is_open()) {
        std::cerr << "error opening out file | name: " << outFName << std::endl;
        return -31;
    }
    else {
        if (!SILENT) std::cout << "openting out file complete" << std::endl;
    }

    if (mode == "classify") {
        if (!sts.is_open()) {
            std::cerr << "error opening stats file | name: " << stsFName << std::endl;
            return -32;
        }
        else {
            if (!SILENT) std::cout << "openting stats file complete" << std::endl;
        }
    }

    if (mode == "learn") {
        //чтение входного файла
        /*
            <Количество строк в вопросе [n]>
            <Тег 1>,<Тег 2>,...,<Тег m>
            <Заголовок вопроса>
            <Текст вопроса [n строк]>
        */
        int countL;
        TAnalyser analyser;
        std::string word;
        bool reachedEnd = false;

        while (inp.peek() != EOF) {
            std::vector<TToken> tokensOfExample;
            std::vector<TExample> learnData;
            std::vector<std::string> tags;
            inp >> countL;

            //считываем первую строку (теги)
            reachedEnd = ReadLine(&tags, inp, false);

            if (!SILENT) {
                std::cout << "Tags read\n\t";
                for (int i = 0; i < tags.size(); ++i) {
                    std::cout << tags[i] << " ";
                }
                std::cout << "\n";
            }


            //первая строка счтана
            //считываем все признаки (остальные строки)
            //вторую строку (заголовок) не записываем в признаки
            for (int i = 0; i < countL + 1; ++i) {
                //считываем строки
                reachedEnd = ReadLine(&tokensOfExample, inp, (i == 0 ? true : false));

                if (reachedEnd) {
                    break;
                }
            }

            if (!SILENT) {
                std::cout << "Words read:\n\t";
                for (int i = 0; i < tokensOfExample.size(); ++i) {
                    std::cout << tokensOfExample[i].Body << " ";
                }
                std::cout << "\n";
            }

            //завершили чтение обучающего запроса.
            //отправляем его на обучение анализатору.

            learnData.push_back(TExample(tags, tokensOfExample)); //добавили экземпляр в массив данных для обучения

            analyser.Learn(learnData); //запустили обучение
            if (!SILENT) std::cout << "example learned\n";
        }

        if (!SILENT) std::cout << "learning complete\n";
        analyser.SaveToFile(out);
        inp.close();
        out.close();
        return 0;
    }
    else {
        if (mode == "classify") {
            int countT;
            TAnalyser analyser;
            bool reachedEnd = false;
            analyser.LoadFromFile(sts);


            while (inp.peek() != EOF) {
                inp >> countT;
                std::vector<TToken> inpExp;
                std::vector<std::string> header;

                reachedEnd = ReadLine(&header, inp, false);

                for (int i = 0; i < countT; ++i) {
                    //считываем тестовый экземпляр

                    reachedEnd = ReadLine(&inpExp, inp, false);

                    if (reachedEnd) {
                        break;
                    }
                }

                if (!SILENT) {
                    std::cout << "Request read:\n\t";
                    for (int i = 0; i < inpExp.size(); ++i) {
                        std::cout << inpExp[i].Body << " ";
                    }
                    std::cout << "\n";
                }

                //алгоритмы:
                //classic
                //стандартный алгоритм наивного Байесовского анализатора
                //не может классифицировать примеры, содержащие уникальные особенности сразу нескольких классов
                //central
                //"центральный" алгоритм (определяет класс не учитывая уникальные для какого-либо класса особенности) 
                //работает с любыми входными данными, однако надежность результата чильно срадает
                //adaptive
                //по умолчанию использует classc (так как у него наибольшая наденость результата), однако, если classic не может классифицировать экземпляр применяет central
                TReport res = analyser.Analyse(inpExp, "adaptive");

                for (int l = 0; l < header.size(); ++l) {
                    if(!SILENT) std::cout << header[l] << " ";
                    out << header[l] << " ";
                }
                if (!SILENT) std::cout << std::endl;
                out << std::endl;

                if (!SILENT) std::cout << "|< ANALYSER: ";
                if (res.MostProbableClass == "") {
                    if (!SILENT) std::cout << "unable to identify class\n";
                    out << "unable to identify class\n";
                }
                else {
                    if (!SILENT) std::cout << (SILENT ? "" : "\nEstimated document class: ") << res.MostProbableClass << std::endl;
                    out << "\tEstimated document class: " << res.MostProbableClass << std::endl;
                }

                if (ADVANCED) {
                    if (!SILENT) {
                        std::cout << "\t---------------------------------------\n" << "\t|advanced report:\n";
                        std::cout << "\t| Algorithm used: " << res.AlgoUsed << std::endl;
                        std::cout << "\t| Compatibility of sample: " << res.Compatibility << std::endl;
                        if (res.AlgoUsed == "central") std::cout << "\t| Central Compatibility: " << res.RM.begin()->second.Comp << std::endl;
                        if (res.AlgoUsed == "central") std::cout << "\t| Confidence: " << res.Confidence << std::endl;
                        for (auto i : res.RM) {
                            std::cout << "\t| Class " << i.first << "\t| Sample difference: " << i.second.Prob << "\t| Words matched: " << i.second.MatchedAttributes << " of " << inpExp.size();
                            if (res.AlgoUsed != "central") std::cout << "\t| Class compatibility: " << i.second.Comp << std::endl;
                            else std::cout << std::endl;
                        }
                        std::cout << "\t---------------------------------------\n";
                    }

                    out << "\t---------------------------------------\n" << "\t|advanced report:\n";
                    out << "\t| Algorithm used: " << res.AlgoUsed << std::endl;
                    out << "\t| Compatibility of sample: " << res.Compatibility << std::endl;
                    if (res.AlgoUsed == "central") out << "\t| Central Compatibility: " << res.RM.begin()->second.Comp << std::endl;
                    if (res.AlgoUsed == "central") out << "\t| Confidence: " << res.Confidence << std::endl;
                    for (auto i : res.RM) {
                        out << "\t| Class " << i.first << "\t| Sample difference: " << i.second.Prob << "\t| Words matched: " << i.second.MatchedAttributes << " of " << inpExp.size();
                        if (res.AlgoUsed != "central") out << "\t| Class compatibility: " << i.second.Comp << std::endl;
                        else out << std::endl;
                    }
                    out << "\t---------------------------------------\n";
                }

            }

            return 0;
        }

        inp.close();
        out.close();
        sts.close();
    }
}

//условите для идеальной работы - все слова должны входить в оба классса