#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>

struct TToken {
    //"�����" �������
    std::string Body;

    TToken(std::string bod) {
        Body = bod;
    }
};

struct TExample {
    //�������������� ������ �� ���������� ������
    std::vector<std::string> ClassID;
    std::vector<TToken> Tokens;

    TExample(std::vector<std::string> cid, std::vector<TToken> tks) {
        ClassID = cid;
        Tokens = tks;
    }
};

struct TFeature {
    long Count; //��������� ���-�� ���������
    std::vector<int> CountInTypes; // ���-�� �� �������

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
    //����� � ������ �����������
    std::string AlgoUsed;
    std::string MostProbableClass; //�������������� �����
    double Compatibility; //������������� ��������� � ������������ (����� ������� ���� � ��������� ��� ���������)
    double Confidence; //"�����������" �����������, �������������� �� ������������� ��������� � �������������� ������� ������������ �������������� � �������� ��������� �������
    std::map<std::string, TClassRep> RM; //�������� �������� ��� ���������� ������

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
    std::map<std::string, std::pair<int/*������*/, int/*���-��*/>> Classes; //��� �������� ����������� �������
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