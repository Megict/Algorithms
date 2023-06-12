#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cmath>
#include <fstream>
#include "analyser.h"
#define DEBUG false


int TFeature::addType(int typeCount) {
    for (int i = 0; i < typeCount; ++i) {
        CountInTypes.push_back(0);
    }
    return 0;
}

    

int TFeature::IncInType(int typeId) {
    //auto elm = CountInTypes.find(typeId);
    if (typeId < CountInTypes.size() && typeId >= 0) {
        CountInTypes[typeId]++;
        //добавили наличие токена в этом классе
        return 0;
    }
    else
        if (typeId >= CountInTypes.size()) {
            CountInTypes.resize(typeId + 1, 0);
            CountInTypes[typeId]++;
            return 1;
        }
        else
        {
            return -1;
        }
}

int TFeature::CountInType(int typeId) {
    if (typeId >= CountInTypes.size()) {
        return 0;
    }
    else
        if (typeId >= 0) {
            return CountInTypes[typeId];
        }
        else {
            return -1;
        }
}

int TFeature::SaveInFile(std::ofstream& out) {
    out.write((char*)&Count, sizeof(Count));
    size_t size = (int)CountInTypes.size();
    out.write((char*)&size, sizeof(size_t));
    for (int s : CountInTypes) {
        out.write((char*)&s, sizeof(s));
    }

    return 0;
}

int TFeature::LoadFromFile(std::ifstream& in) {
    in.read((char*)&Count, sizeof(Count));
    size_t size;
    in.read((char*)&size, sizeof(size_t));
    if (DEBUG) std::cout << "\tfeat at all: " << Count << "\n\tsize: " << size << "\n\t";
    for (int i = 0; i < size; ++i) {
        int s;
        in.read((char*)&s, sizeof(int));
        if (DEBUG) std::cout << s << " ";
        CountInTypes.push_back(s);
    }
    if (DEBUG) std::cout << std::endl;
    return 0;
}



int TAnalyser::SaveToFile(std::ofstream& out) {
    size_t size = Classes.size();
    out.write((char*)&size, sizeof(size_t));
    for (auto s : Classes) {
        size_t sz = s.first.size();
        if (DEBUG) std::cout << sz << " <<-\n";
        out.write((char*)&sz, sizeof(size_t));
        const char* tw = s.first.c_str();
        out.write((char*)&tw[0], sz);
        out.write((char*)&s.second.first, sizeof(int));
        out.write((char*)&s.second.second, sizeof(int));
    }
    out.write((char*)&ExamplesSummary, sizeof(ExamplesSummary));
    size = (int)AllFeatures.size();
    out.write((char*)&size, sizeof(size_t));
    for (auto s : AllFeatures) {
        int sz = (int)s.first.size();
        out.write((char*)&sz, sizeof(size_t));
        const char* tw = s.first.c_str();
        out.write((char*)&tw[0], sz);
        s.second->SaveInFile(out);
    }
    out.write((char*)&CurId, sizeof(CurId));

    return 0;
}

int TAnalyser::LoadFromFile(std::ifstream& in) {
    size_t size;
    in.read((char*)&size, sizeof(size_t));
    if (DEBUG) std::cout << "classes: " << size << " |\n";
    for (int i = 0; i < size; ++i) {
        int s1;
        in.read((char*)&s1, sizeof(size_t));
        if (DEBUG) std::cout << s1 << "<<-\n";
        std::string str;

        char* tw = (char*)malloc(s1 + 1);
        in.read(&tw[0], s1);
        tw[s1] = 0;
        str = std::string(tw);
        free(tw);
        int sf, ss;
        in.read((char*)&sf, sizeof(int));
        in.read((char*)&ss, sizeof(int));
        if (DEBUG) std::cout << str << " " << sf << " " << ss << std::endl;
        Classes.emplace(std::make_pair(str, std::make_pair(sf, ss)));
    }
    in.read((char*)&ExamplesSummary, sizeof(ExamplesSummary));
    in.read((char*)&size, sizeof(size_t));
    if (DEBUG) std::cout << "examples sum: " << ExamplesSummary << "\nfeatCount: " << size << std::endl;
    for (int i = 0; i < size; ++i) {
        int s1;
        in.read((char*)&s1, sizeof(size_t));
        if (DEBUG) std::cout << s1 << "<<-\n";
        std::string str;

        char* tw = (char*)malloc(s1 + 1);
        in.read(&tw[0], s1);
        tw[s1] = 0;
        str = std::string(tw);
        free(tw);

        TFeature* ipt = new TFeature();
        ipt->LoadFromFile(in);
        AllFeatures.emplace(std::make_pair(str, ipt));
    }
    in >> CurId;
    if (DEBUG) std::cout << CurId << std::endl;

    return 0;
}

int TAnalyser::Learn(std::vector<TExample> array) {
    //признаками считаем все слова
    //в данной версии array содержит 1 экземпляр и набор классов, к которым он относится

    //1. подсчитать число классов, число экземпляров и частотность классов.

    for (int i = 0; i < array[0].ClassID.size(); ++i) {//для прибывших в array классов подсчитывем новое количество экземпляров
        auto elm = Classes.find(array[0].ClassID[i]);
        ExamplesSummary++;
        if (elm == Classes.end()) {
            Classes.emplace(std::make_pair(array[0].ClassID[i], std::make_pair(CurId, 1)));
            CurId++;
            if (DEBUG) std::cout << "class: " << array[0].ClassID[i] << " learned\n";
            //добавили наличие токена в этом классе
        }
        else {
            if (DEBUG) std::cout << "class: " << array[0].ClassID[i] << " aded example\n";
            elm->second.second++;
        }
    }

    //пункт 1 завершен
    //------------------------------------------------

    //заполняем массив всех признаков
    for (int k = 0; k < array.size(); ++k) { //по идее должен проходить лишь одну итерацию
        if (DEBUG) std::cout << "\tinputing exmp " << k << std::endl;
        for (int i = 0; i < array[k].Tokens.size(); ++i) {
            if (DEBUG) std::cout << "\ttoken " << array[k].Tokens[i].Body << std::endl;
            auto elm = AllFeatures.find(array[k].Tokens[i].Body);
            if (elm != AllFeatures.end()) {
                if (DEBUG) std::cout << "\t\tthis feat exsts\n";
                //есть такая фича
                elm->second->Count += array[0].ClassID.size();

                for (int j = 0; j < array[k].ClassID.size(); ++j) {
                    if (elm->second->IncInType(Classes[array[k].ClassID[j]].first) == -1) {//добавляем вохождение в текущий класс

                        std::cerr << "ERROR: in fill of allFeatures: class with index: " << array[k].ClassID[j] << " out of range of classes" << std::endl;
                        //вообще исходя из предыдущего кода можно считать, что вывод этого сообщения невозможен, но мало ли... 110% невозможен. УДАЛИТЬ
                    }
                }
            }
            else {
                if (DEBUG) std::cout << "\t\tthis feat not exsts\n";
                //надо добавить
                TFeature* adb = new TFeature(1);
                for (int j = 0; j < array[k].ClassID.size(); ++j) {
                    if (adb->IncInType(Classes[array[k].ClassID[j]].first) == -1) {//добавляем вохождение в текущий класс
                        std::cerr << "ERROR: in fill of allFeatures: class with index: " << array[k].ClassID[j] << " out of range of classes" << std::endl;
                        //вообще исходя из предыдущего кода можно считать, что вывод этого сообщения невозможен, но мало ли...
                    }
                }
                AllFeatures.emplace(std::make_pair(array[k].Tokens[i].Body, adb));
            }
        }
    }

    //у нас есть массив фич, у каждой из них указано общеее число во всех текстах и кол-во упоминаний в текстах каждого класса по отдельности
    //можно работать с этим.

    //при ображении к анализатору с текстом мы:
    //  1. разбиваем текст на набор фич
    //  2. перемножаем число каждой фичи на частоту ее в предполагаемом классе и умножаем это на частоту самого класса
    //  3. смотрим, какое из произведений имеет наибольшее значение

    if (DEBUG) {
        for (const auto& p : AllFeatures) {
            std::cout << "\tdeb | " << p.first << " | " << p.second->Count << " | ";
            for (int j = 0; j < p.second->CountInTypes.size(); ++j) {
                std::cout << p.second->CountInTypes[j] << " ";
            }
            std::cout << "\n";
        }
    }

    return 0;
}

TReport TAnalyser::Analyse(std::vector<TToken> inputAttributes, std::string algorithm) {
    if (algorithm != "central" && algorithm != "adaptive" && algorithm != "classic") {
        //неверно задан алгоритм
        return TReport("INCORRECT ALGO");
    }

    if (ExamplesSummary == -1) {
        //анализатор не обучен
        return TReport("ANATYSER NOT LEARNED");
    }

    //для анализа корректности результата работы центрального алгоритма

    //на вход поступает набор признаков
    //алгоритм поочередно смотрит вероятность нахождения каждого из признаков в каждом известном ему классе
    //произведение этих вероятностей записывается в отдельный массив, дополнительно учитывается частотф встречаемости каждого из классов в данных для обучения
    //результат возвращается в специальносй структуре, содержащей все данные анализа.
    std::map<std::string, TClassRep> reportMap;
    double compatibility = 0;
    long long unfound = 0;

    for (std::pair<std::string, std::pair<int, int>> k : Classes) {
        unfound = 0;

        auto cur = reportMap.find(k.first);
        if (cur == reportMap.end()) {
            reportMap.emplace(std::make_pair(k.first, TClassRep(-log((double)k.second.second / (double)ExamplesSummary))));
            cur = reportMap.find(k.first);
        }
        else {
            cur->second.Prob *= -log((double)k.second.second / (double)ExamplesSummary);
        }
        //определили cur. теперь обращаемся к отчету по классу через него.
        if (DEBUG) std::cout << "class: " << k.first << std::endl;
        for (int i = 0; i < inputAttributes.size(); ++i) {
            //находим признак в памяти анализатора
            auto elm = AllFeatures.find(inputAttributes[i].Body);
            if (elm != AllFeatures.end()) {
                if (DEBUG) std::cout << "\tfound feat: " << inputAttributes[i].Body << " in this type: " << elm->second->CountInType(k.second.first) << " of: " << elm->second->Count << std::endl;
                if (elm->second->CountInType(k.second.first) == 0 || elm->second->CountInType(k.second.first) == elm->second->Count) {
                    if (algorithm == "central") {
                        //при работе по центральному алгоритму не учитываем признаки, уникальные для одного класса
                        cur->second.NotFound++;
                        if (DEBUG) std::cout << "\tnextSt (central)\n";
                        continue;
                    }
                }

                //заполнение контрольных значений для классического алгоритма
                if (elm->second->CountInType(k.second.first) != 0) {
                    cur->second.MatchedAttributes++;
                }
                else {
                    cur->second.NotFound++;
                }

                //увеличение вероятности
                cur->second.Prob += -log(((double)(elm->second->CountInType(k.second.first)) / ((double)(elm->second->Count))));
            }
            else {
                if (DEBUG) std::cout << "\tnot found feat: " << inputAttributes[i].Body << std::endl;
                //такого признака в массиве нет
                //unfound инкрементируем только при первом проходе, так как при последующих мы не найдем ровно те же признаки
                unfound++;

                cur->second.NotFound++;

                continue;
            }

        }
    }

    //рассчет контрольных величин
    //общая совместимость
    compatibility = (double)(inputAttributes.size() - unfound) / (double)inputAttributes.size();
    for (auto i : reportMap) {
        //совместимости по классам
        auto cur = reportMap.find(i.first);
        cur->second.Comp = (double)((double)inputAttributes.size() - (double)i.second.NotFound) / (double)inputAttributes.size();
    }

    if (DEBUG) {
        std::cout << "\tfeat in input: " << inputAttributes.size() << std::endl;
        std::cout << "\tfeat not found: " << unfound << std::endl;
    }

    double maxP = reportMap.begin()->second.Prob;
    double nextP = (reportMap.begin()++)->second.Prob;
    std::string maxPi = "";


    for (auto k : reportMap) {
        if (DEBUG) std::cout << " comp: " << k.second.Comp << std::endl;
        //поиск наиболее вероятного класса документа
        if (k.second.Prob <= maxP) {
            nextP = maxP;
            maxP = k.second.Prob;
            maxPi = k.first;
        }
    }

    if (maxP == INFINITY) {
        maxPi = "";
    }

    if (maxPi == "" && algorithm == "adaptive") { //если не смогли определеить класс, то возвращаем класс, определенный центральным алгоритмом
        return Analyse(inputAttributes, "central");
    }

    if (algorithm == "adaptive") {
        algorithm = "classic";
    }

    return TReport(algorithm, maxPi, compatibility, abs(1 - nextP / maxP) * compatibility, reportMap);
}