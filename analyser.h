#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>

struct TToken {
    //"сырой" признак
    std::string Body;

    TToken(std::string bod) {
        Body = bod;
    }
};

struct TExample {
    //необработанные данные по экземпл€ру классф
    std::vector<std::string> ClassID;
    std::vector<TToken> Tokens;

    TExample(std::vector<std::string> cid, std::vector<TToken> tks) {
        ClassID = cid;
        Tokens = tks;
    }
};

struct TFeature {
    long Count; //суммарное кол-во элементов
    std::vector<int> CountInTypes; // кол-во по классам

    TFeature() {
        Count = 0;
    }

    ~TFeature() {
        CountInTypes.clear();
    }

    TFeature(int count) {
        Count = count;
        CountInTypes = std::vector<int>();
    }

    int addType(int typeCount);
    int IncInType(int typeId);
    int CountInType(int typeId);
    int SaveInFile(std::ofstream& out);
    int LoadFromFile(std::ifstream& in);
};

struct TClassRep {
    double Prob;
    double Comp;
    int NotFound;
    int MatchedAttributes;

    TClassRep() {
        Prob = 1;
        Comp = 0;
        NotFound = 0;
        MatchedAttributes = 0;
    }

    TClassRep(double prob) {
        Prob = prob;
        Comp = 0;
        NotFound = 0;
        MatchedAttributes = 0;
    }
};

struct TReport {
    //отчет о работе анализатора
    std::string AlgoUsed;
    std::string MostProbableClass; //предполагаемый класс
    double Compatibility; //совместимость документа с анализатором (какой процент слов в документе был распознан)
    double Confidence; //"уверенность" анализатора, складывающа€с€ из совместимости документа и отностительной разницы веро€тностей принадл€жности к наиболее веро€тным классам
    std::map<std::string, TClassRep> RM; //побочные величины дл€ подробного отчета

    TReport(std::string err) {
        AlgoUsed = err;
        MostProbableClass = "";
        Compatibility = 0;
        Confidence = 0;
        RM = std::map<std::string, TClassRep> ();
    }
        
    TReport(std::string algo, std::string mpc, double comp, double confidence, std::map<std::string, TClassRep> prob) {
        AlgoUsed = algo;
        MostProbableClass = mpc;
        Compatibility = comp;
        Confidence = confidence;
        RM = prob;
    }
};

class TAnalyser {
private:
    std::map<std::string, std::pair<int/*индекс*/, int/*кол-во*/>> Classes; //дл€ подсчета частотности классов
    std::map<std::string, TFeature*> AllFeatures;
    int ExamplesSummary;
    int CurId;
public:

    TAnalyser() {
        CurId = 0;
        ExamplesSummary = 0;
    }

    ~TAnalyser() {
        Classes.clear();
        for (auto a : AllFeatures) {
            delete a.second;
        }
        AllFeatures.clear();
    }

    int SaveToFile(std::ofstream& out);
    int LoadFromFile(std::ifstream& in);
    int Learn(std::vector<TExample> array);
    TReport Analyse(std::vector<TToken> inputAttributes, std::string algorithm);
};