//#pragma warning(disable : 4996)
#include <iostream>
#include <vector>
#include <cstring>

#define DEBUG false

class TTrieNode {
private:
    std::vector<TTrieNode*> children;
    unsigned long litera = 0;
    TTrieNode* suffix;
    TTrieNode* parent;
    long long term = 0;
public:
    TTrieNode(unsigned long inp) {
        litera = inp;
        term = 0;
        suffix = nullptr;
        parent = nullptr;
    }

    ~TTrieNode() {
        for (unsigned long i = 0; i < children.size(); i++) {
            delete(children[i]);
        }
    }

    void setSuff(TTrieNode* suff) {
        suffix = suff;
    }

    void setPar(TTrieNode* par) {
        parent = par;
    }

    unsigned long liter() {
        return litera;
    }

    std::vector<TTrieNode*> child() {
        return children;
    }

    bool hasChild(unsigned long a) {
        for (unsigned long i = 0; i < children.size(); ++i) {
            if (children[i]->litera == a) {
                return true;
            }
        }
        return false;
    }

    unsigned long isTerm() {
        return term;
    }

    void addChild(TTrieNode* adb) {
        adb->setPar(this);
        children.push_back(adb);
    }

    void setterm(unsigned long count) {
        term = count;
    }

    TTrieNode* child(unsigned long a) {
        for (unsigned long i = 0; i < children.size(); ++i) {
            if (children[i]->litera == a) {
                return children[i];
            }
        }
        return nullptr;
    }

    TTrieNode* back() {
        return suffix;
    }

    TTrieNode* Parent() {
        return parent;
    }
};

void printtrie(TTrieNode* root,int depth) {
    for (int i = 0; i < depth; ++i) {
        printf("\t");
    }
    if (root->isTerm()) {
        printf("!");
    }
    printf("|%lu|[]", root->liter());
    std::vector<TTrieNode*> next = root->child();
    printf("%lu>\n", next.size());
    for (unsigned long i = 0; i < next.size(); ++i) {
        printtrie(next[i],depth+1);
    }
}

std::vector<unsigned long> reconstruct(TTrieNode* root, TTrieNode* end) {
    std::vector<unsigned long> word;

    while(end != root) {
        word.push_back(end->liter());
        end = end->Parent();
    }

    std::vector<unsigned long> res(word.size());
    for(unsigned long i=0;i<res.size();++i) {
        res[i] = word[word.size()-i-1];
    }
    
    return res;
}

TTrieNode* correlate(TTrieNode* root, std::vector<unsigned long> word) {
    for (unsigned long i = 1; i < word.size(); ++i) {
        TTrieNode* cur = root;
        for (unsigned long j = i; j < word.size();++j) {
            if (cur->child(word[j]) == nullptr) {
                break;
            }
            else {
                cur = cur->child(word[j]);

                if (j + 1 == word.size()) {
                    return cur;
                }
            }
        }
    }
    return root;
}

void select(TTrieNode* root, TTrieNode* cur, std::vector<unsigned long> word) {
    if (cur != root) {
        word.push_back(cur->liter());
        
        if(DEBUG)printf("word found:\n");
        for (unsigned long i = 0; i < word.size(); ++i) {
            if(DEBUG)printf("  |%lu\n", word[i]);
        }
        word = reconstruct(root,cur);
        TTrieNode* suff = correlate(root, word);
        // ////////if (DEBUG) {
        //     if (suff != root) {
        //         printf("suffix leads to: %lu, []\n", suff->liter());
        //     }
        //     else {
        //         printf("suffix leads to root\n");
        //     }
        // }
        cur->setSuff(suff);
    }

    std::vector<TTrieNode*> next = cur->child();
    for (unsigned long i = 0; i < next.size(); ++i) {
        select(root, next[i], word);
    }
}

int reconst(TTrieNode* begin, TTrieNode* end, int depth) {
    
    if (begin == end) {
        return depth;
    }

    std::vector<TTrieNode*> next = begin->child();
    for (unsigned long i = 0; i < next.size(); ++i) {
        int tmp = reconst(next[i], end, depth + 1);
        if (tmp != -1) {
            return tmp;
        }
    }

    return -1;
}

TTrieNode* sufGet(TTrieNode* root, TTrieNode* forWhom) {
    if(forWhom->back() != nullptr) {
        return forWhom->back();
    }
    else {
        std::vector<unsigned long> word = reconstruct(root,forWhom);
        TTrieNode* res = correlate(root, word);
        forWhom->setSuff(res);
        return res;
    }
}


class TTrie {
private:
    TTrieNode* root;
    unsigned long maxSize;
    long long count = 0;
public:
    TTrie() {
        root = new TTrieNode(0);
        maxSize = 0;
        count = 0;
        root->setSuff(root);
    }

    ~TTrie() {
        delete root;
    }

    unsigned long Count() {
        return count;
    }

    long long add(std::vector<unsigned long> word) {
        if (word.size() > maxSize) {
            maxSize = word.size();
        }

        TTrieNode* cur = root;

        for (unsigned long i = 0; i < word.size(); ++i) {
            if (cur->hasChild(word[i])) {
                cur = cur->child(word[i]);
                ////////if (DEBUG)printf("passed (%lu)\n", cur->liter());
            }
            else {
                TTrieNode* adb = new TTrieNode(word[i]);
                cur->addChild(adb);
                cur = cur->child(word[i]);
                ////////if (DEBUG)printf("aded (%lu)\n", cur->liter());
            }
        }

        if (cur->isTerm()) {
            return cur->isTerm();
        }

        count++;
        cur->setterm(count);
        return count;
    }

    bool contains(char* word) const {
        TTrieNode* cur = root;

        for (unsigned long i = 0; i < strlen(word); ++i) {
            if (cur->hasChild(word[i])) {
                cur = cur->child(word[i]);
            }
            else {
                return false;
            }
        }

        return true;
    }

    void print() {
        printtrie(root, 0);
    }

    void suffixate() {
        std::vector<unsigned long> word;
        select(root, root, word);
    }

    TTrieNode* Root() {
        return root;
    }
};

class Tresult {
public:
    long long numberOfPattern; 
    unsigned long startPos;
    unsigned long endPos;

    Tresult(long long num, unsigned long beg, unsigned long end) {
        numberOfPattern = num;
        startPos = beg;
        endPos = end;
    }
};

std::vector<Tresult> search(TTrie* trie,std::vector<unsigned long> text) {
    std::vector<Tresult> ret;
    TTrieNode* cur = trie->Root();

    for (unsigned long i = 0; i < text.size(); ++i) {

        TTrieNode* next = cur->child(text[i]);
        if (next == nullptr) {
            
            next = sufGet(trie->Root(),cur);

            if (cur != trie->Root()) {
                --i;
            }
        }
        cur = next;

        if (cur->isTerm()) {
            ret.push_back(Tresult(cur->isTerm(), i - reconst(trie->Root(),cur,0) + 2, i + 1));
        }
    }

    return ret;
}


std::vector<Tresult> collect(std::vector<Tresult> preRes, unsigned long maxSize, std::vector<long long> templ, unsigned long max, unsigned long countOfLetters) {
    unsigned long beg = 0;
    unsigned long end = 0;
    std::vector<Tresult> startedCollection;
    std::vector<Tresult> finalRes;
    unsigned long templPos = 0;

    unsigned long start = 0;
    if (templ[templPos] == -1) {
        while (templPos < templ.size() && templ[templPos] == -1) {
            ++start;
            ++templPos;
        }
    }

    
    for (unsigned long i = 0; i < preRes.size(); ++i) {
        templPos = 0;

        if ((preRes[i].numberOfPattern == templ[start]) && (preRes[i].startPos - 1 >= start)) {
            unsigned long incCount = 1;
            unsigned long space = 0;
            templPos = start + 1;
            beg = preRes[i].startPos - 1 - start;
            end = preRes[i].endPos;

            if (incCount == maxSize) {
              if (templPos < templ.size()) {
                    if (preRes[i].endPos + (templ.size() - (templPos)) > max) {

                        //////if (DEBUG)printf("\tfinal J chek failed\n");
                        break;
                    }
                    
                }

                finalRes.push_back(Tresult(0, beg + 1, preRes[i].endPos + (templ.size() - (templPos))));
                //////if (DEBUG)printf("\tinclusion confirmed\n");
                continue;
            }

            
            space = 0;
            for (unsigned long j = i + 1; j < preRes.size(); ++j) {
                
                if(preRes[j].endPos - beg > countOfLetters) {
                    break;
                }
                
                if (incCount < maxSize) {

                    if (templ[templPos] == -1) { 
                        while (templ[templPos] == -1 && templPos < templ.size()) {
                            ++space;
                            ++templPos;
                        }
                    }


                    if (preRes[j].numberOfPattern == templ[templPos] && preRes[j].startPos == end + space + 1) { //���������, �������� �� ��������� ��� �����������
                        //////if (DEBUG)printf("\t\t\t\tpattern OK\n");
                        end = preRes[j].endPos;
                        incCount++;
                        space = 0;
                        if (incCount < maxSize) {
                            templPos++;
                        }
                    }
                    else { //���� �� ��������� �� ���������� ���������
                        //////if (DEBUG)printf("\t\t\t\tWRONG pattern: starts from |-> %lu|%lu <-| should start from\n",preRes[j].startPos, end + 1 + space);
                        continue;
                    }
                }

                if (incCount == maxSize) {//��������� ���������, ���� ������� ���������
                    
                    if (templPos + 1 != templ.size()) {
                        if (preRes[j].endPos + (templ.size() - (templPos + 1)) > max) {

                            //////if (DEBUG)printf("\tfinal J chek failed\n");
                            break;
                        }

                        //////if (DEBUG)printf("\tfinal J chek passed\n");
                    }

                    finalRes.push_back(Tresult(0, beg+1, preRes[j].endPos + (templ.size() - (templPos + 1))));
                    //////if (DEBUG)printf("\tinclusion confirmed\n");
                    break;
                }
            }
        }
    }

    
        
    return finalRes;
}

int main() {
    TTrie mainTrie = TTrie();

    unsigned long inp = 0;

    char sq;
    int lastq = -1;
    std::vector<unsigned long> inpword;
    std::vector<unsigned long> textmodula;
    std::vector<long long> jPattern;
    bool writeReady = false;
    unsigned long count = 0; 
    while (scanf("%c", &sq)) {
        if ((sq >= '0') && (sq <= '9')) {
            if(writeReady) {
                inpword.push_back(inp);
                ++count;
                inp = 0;

                writeReady = false;
            }
            inp *= 10;
            inp += sq - '0';
            lastq = 0;
        }
        else
        if (sq == ' ' && lastq == 0) {
            writeReady = true;
        }
        else
        if (sq == '?') {
            ++count;
            if (lastq == 0) {
                if(writeReady) {
                    inpword.push_back(inp);
                    ++count;
                    inp = 0;
                    
                    writeReady = false;
                }

                unsigned long ii = mainTrie.add(inpword,count);
                inpword.clear();
                

                jPattern.push_back(ii);
                jPattern.push_back(-1);
            }
            else {
                jPattern.push_back(-1);
            }
            lastq = 1;
        }
        else
        if (sq == '\n') {
            if (lastq == 0) {
                inpword.push_back(inp);
                ++count;
                unsigned long ii = mainTrie.add(inpword);
                inpword.clear();
                
                jPattern.push_back(ii);
            }
            break;
        }
    }

    unsigned long ms = 0;
    for (unsigned long i = 0; i < jPattern.size(); ++i) {
        if (jPattern[i] == -1) {
            //////if (DEBUG)printf("j");
        }
        else {
            //////if (DEBUG)printf("%lld", jPattern[i]);
            ++ms;
        }
    }

    inpword.clear();
    unsigned long textSize = 0;
    
    inp = 0;
    int spx = 0;
    writeReady = false;
    textmodula.push_back(1);
    while (scanf("%c", &sq) != EOF) {
        if ((sq >= '0') && (sq <= '9')) {
            if(writeReady) {
                inpword.push_back(inp);
                inp = 0;
                textSize++;
                //writeReady = false;
            }

            inp *= 10;
            inp += sq - '0';
            lastq = 0;
            
            spx = 1;
        }
        else
        if (sq == ' ' && spx == 1) {
            inpword.push_back(inp);
                inp = 0;
                spx = 0;
                textSize++;
                //writeReady = false;
            spx = 0;
        }
        if (sq == '\n') {
            if(spx == 1) {
                inpword.push_back(inp);
                inp = 0;
                spx = 0;
                textSize++;
                //writeReady = false;
            }
            
            textmodula.push_back(textSize + 1);
        }
    }


    std::vector<Tresult> preRes = search(&mainTrie, inpword);
    std::vector<Tresult> finalRes = collect(preRes, ms, jPattern, textSize, count);

    for (unsigned long i = 0; i < finalRes.size(); ++i) {
        for (unsigned long j = 0; j < textmodula.size(); ++j) {
            if (DEBUG == true) printf("newstr: %lu\n", textmodula[j]);
            if (finalRes[i].startPos >= textmodula[j]) {
                if(j + 1 == textmodula.size() || finalRes[i].startPos < textmodula[j+1]) {
                    printf("%lu, %lu\n", j+1, finalRes[i].startPos - textmodula[j] + 1);
                    break;
                }
            }
        }
    }
    
    return 0;
}