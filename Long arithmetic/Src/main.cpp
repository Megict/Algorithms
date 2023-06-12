#include <vector>
#include <string>
#include <iostream>
#include <cmath>
#define DEBUG false
#define CELLLEN 7
const long long CELLSIZE = (long long)pow(10, CELLLEN);

namespace my {
    class TVeryLong {
    public:
        std::vector<long long> cells;
        size_t len;

        TVeryLong() {
            len = 0;
        }

        TVeryLong(size_t preSize) {
            len = 0;
            cells = std::vector<long long>(preSize);
        }


        TVeryLong(std::vector<long long> inp) {
            //cells = inp; 
            for (size_t i = 0; i < inp.size(); ++i) {
                cells.push_back(inp[inp.size() - 1 - i]);
            }
            len = inp.size();
        }


        //friend std::ofstream& const operator<< (std::ofstream& out, const my::TVeryLong& ptb);
        friend my::TVeryLong operator+ (const my::TVeryLong& lhs, const my::TVeryLong& rhs);
        friend my::TVeryLong operator- (const my::TVeryLong& lhs, const my::TVeryLong& rhs);
        friend my::TVeryLong operator* (const my::TVeryLong& lhs, const my::TVeryLong& rhs);
        friend my::TVeryLong operator^ (const my::TVeryLong& lhs, const my::TVeryLong& rhs);
        friend my::TVeryLong operator^ (const my::TVeryLong& lhs, const long long& rhs);
        friend my::TVeryLong operator/ (const my::TVeryLong& lhs, const my::TVeryLong& rhs);
        friend my::TVeryLong operator/ (const my::TVeryLong& lhs, const long long& rhs);
        friend std::ostream& operator<< (std::ostream& out, const my::TVeryLong& ptb);

        friend bool operator> (const my::TVeryLong& lhs, const my::TVeryLong& rhs);
        friend bool operator< (const my::TVeryLong& lhs, const my::TVeryLong& rhs);
        friend bool operator== (const my::TVeryLong& lhs, const my::TVeryLong& rhs);
    };



    void print(const TVeryLong& ptb);
    void shrinkLeadNulls(TVeryLong& ptb);
    TVeryLong sum(const TVeryLong& left, const TVeryLong& right);
    TVeryLong dif(const TVeryLong& left, const TVeryLong& right);
    TVeryLong comp(const TVeryLong& left, const TVeryLong& right);
    TVeryLong div(const TVeryLong& left, const TVeryLong& right);
    TVeryLong div(const TVeryLong& left, const long long& right); 
    TVeryLong longDiv(const TVeryLong& left, const TVeryLong& right);
    //TVeryLong del(const TVeryLong& left, const TVeryLong& right) {

    //}
}
namespace my {

    void shrinkLeadNulls(TVeryLong& ptb) {
        for (size_t i = ptb.len; i > 0; --i) {
            if (i > 1 && ptb.cells[i - 1] == 0) {
                ptb.cells.pop_back();
                ptb.len--;
            }
            else {
                break;
            }
        }
    }

    my::TVeryLong operator+(const my::TVeryLong& lhs, const my::TVeryLong& rhs)
    {
        my::TVeryLong res = my::sum(lhs, rhs);
        my::shrinkLeadNulls(res);
        return res;
    }

    my::TVeryLong operator-(const my::TVeryLong& lhs, const my::TVeryLong& rhs)
    {
        if (lhs < rhs) {
            throw (-1);
        }

        my::TVeryLong res = my::dif(lhs, rhs);
        my::shrinkLeadNulls(res);
        return res;
    }

    my::TVeryLong operator*(const my::TVeryLong& lhs, const my::TVeryLong& rhs)
    {
        my::TVeryLong res = my::comp(lhs, rhs);
        my::shrinkLeadNulls(res);
        return res;
    }

    my::TVeryLong operator^(const my::TVeryLong& lhs, const my::TVeryLong& rhs)
    {
        if (lhs == my::TVeryLong(std::vector<long long>({ 0 })) && rhs == my::TVeryLong(std::vector<long long>({ 0 }))) {
            throw (-1);
        }

        //my::TVeryLong res = my::TVeryLong(std::vector<long long>({ 1 }));
        my::TVeryLong res = my::TVeryLong(std::vector<long long>({ 1 }));
        my::TVeryLong pre = lhs;
        my::TVeryLong deg = rhs;
        
        while (deg > my::TVeryLong(std::vector<long long>({ 0 }))) {
            /*printf("res\t"); my::print(res); printf("\n");
            printf("pre\t"); my::print(pre); printf("\n");
            printf("deg\t"); my::print(deg); printf("\n");*/
            if (deg.cells[0] % 2 == 1) {
                res = pre * res;
                deg = deg - my::TVeryLong(std::vector<long long>({ 1 }));
            }
            else {
                pre = pre * pre;
                deg = deg / 2;
            }

        }

        return res;
    }

    my::TVeryLong operator^(const my::TVeryLong& lhs, const long long& rhs)
    {
        if (lhs == my::TVeryLong(std::vector<long long>({ 0 })) && rhs == 0) {
            throw (-1);
        }

        my::TVeryLong res = lhs;

        for (int i = 1; i < rhs; ++i) {
            res = res * lhs;
        }

        return res;
    }

    my::TVeryLong operator/(const my::TVeryLong& lhs, const my::TVeryLong& rhs)
    {
        if (rhs == my::TVeryLong(std::vector<long long>({ 0 }))) {
            throw(-1);
        }

        if(DEBUG) printf("i\n");
        my::TVeryLong res = longDiv(lhs, rhs);
        my::shrinkLeadNulls(res);
        return res;
    }

    my::TVeryLong operator/(const my::TVeryLong& lhs, const long long& rhs)
    {
        if (rhs == 0) {
            throw(-1);
        }

        if (DEBUG) printf("r\n");
        return div(lhs, rhs);
    }

    std::ostream& operator<< (std::ostream& out, const my::TVeryLong& ptb) {
        for (size_t i = ptb.len; i > 0; --i) {
            if ((ptb.cells[i - 1] < CELLSIZE / 10) && i != ptb.len) {
                long long mk = ptb.cells[i - 1];
                mk == 0 ? mk = 1 : mk = mk;
                while (mk < CELLSIZE/10) {
                    mk *= 10;
                    out << '0';
                }
            }
            out << ptb.cells[i - 1];
        }
        return out;
    }



    bool operator> (const my::TVeryLong& lhs, const my::TVeryLong& rhs) {
        if (lhs.len > rhs.len) {
            return true;
        }
        else {
            if (lhs.len < rhs.len) {
                return false;
            }
            else {
                for (int i = lhs.len; i > 0; --i) {
                    if (rhs.cells[i - 1] > lhs.cells[i - 1]) {
                        return false;
                    }

                    if (rhs.cells[i - 1] < lhs.cells[i - 1]) {
                        break;
                    }

                    if (i == 1 && rhs.cells[i - 1] >= lhs.cells[i - 1]) {
                        return false;
                    }
                }
                return true;
            }
        }
    }
 
    bool operator< (const my::TVeryLong& lhs, const my::TVeryLong& rhs) {
        if (lhs.len < rhs.len) {
            return true;
        }
        else {
            if (lhs.len > rhs.len) {
                return false;
            }
            else {
                for (int i = lhs.len; i > 0; --i) {
                    if (rhs.cells[i - 1] < lhs.cells[i - 1]) {
                        return false;
                    }

                    if (rhs.cells[i - 1] > lhs.cells[i - 1]) {
                        break;
                    }

                    if (i == 1 && rhs.cells[i - 1] <= lhs.cells[i - 1]) {
                        return false;
                    }
                }
                return true;
            }
        }
    }

    bool operator== (const my::TVeryLong& lhs, const my::TVeryLong& rhs) {
        if (rhs.len > lhs.len) {
            return false;
        }
        else {
            if (rhs.len < lhs.len) {
                return false;
            }
            else {
                for (int i = rhs.len; i > 0; --i) {
                    if (lhs.cells[i - 1] != rhs.cells[i - 1]) {
                        return false;
                    }
                }
                return true;
            }
        }
    }


    void print(const TVeryLong& ptb) {
        for (size_t i = ptb.len; i > 0; --i) {
            if ((ptb.cells[i - 1] < CELLSIZE / 10) && i != ptb.len) {
                long long mk = ptb.cells[i - 1];
                mk == 0 ? mk = 1 : mk = mk;
                while (mk < CELLSIZE/10) {
                    mk *= 10;
                    printf("0");
                }
            }
            printf("%lld|", ptb.cells[i - 1]);
        }
    }

    TVeryLong sum(const TVeryLong& left, const TVeryLong& right) {
        unsigned long maxof = left.len > right.len ? left.len : right.len;
        unsigned long minof = left.len > right.len ? right.len : left.len;
        TVeryLong res;
        long long pathadd = 0;
        for (unsigned int i = 0; i < maxof; ++i) {
            //printf("summing... %lld\n", (left.cells[i] + right.cells[i] + pathadd) % CELLSIZE);
            if (right.len > left.len) {
                res.cells.push_back(i < minof ? (left.cells[i] + right.cells[i] + pathadd) % CELLSIZE : (right.cells[i] + pathadd) % CELLSIZE);
                ++res.len;

            }
            else
            if (right.len < left.len) {
                res.cells.push_back(i < minof ? (left.cells[i] + right.cells[i] + pathadd) % CELLSIZE : (left.cells[i] + pathadd) % CELLSIZE);
                ++res.len;
                
            }
            else {
                //printf("summing sim\n");
                res.cells.push_back((left.cells[i] + right.cells[i] + pathadd) % CELLSIZE);
                ++res.len;
            }

            if (i < minof) {
                if (left.cells[i] + right.cells[i] + pathadd >= CELLSIZE) {
                    pathadd = 1;
                }
                else {
                    pathadd = 0;
                }

                if (i + 1 == maxof && pathadd > 0) {
                    res.cells.push_back(pathadd);
                    ++res.len;
                }
            }
            else 
            if (right.len > left.len) {
                if(i >= minof && right.cells[i] + pathadd >= CELLSIZE) {
                    pathadd = 1;
                }
                else {
                    pathadd = 0;
                }
            }
            else
            if (right.len < left.len) {
                
                if(i >= minof && left.cells[i] + pathadd >= CELLSIZE) {
                    pathadd = 1;
                }
                else {
                    pathadd = 0;
                }
            }
            else{
                pathadd = 0;
            }
        }

        return res;
    }

    TVeryLong dif(const TVeryLong& left, const TVeryLong& right) {
        TVeryLong res(left.len);
        if (left.len > right.len || (left.len == right.len && left.cells[left.len - 1] >= right.cells[right.len - 1])) {
            for (unsigned int i = left.len; i > right.len; --i) {
                res.cells[i - 1] = left.cells[i - 1];
            }

            for (unsigned int i = right.len; i > 0; --i) {
                if (left.cells[i - 1] >= right.cells[i - 1]) {
                    res.cells[i - 1] = left.cells[i - 1] - right.cells[i - 1];
                }
                else {
                    res.cells[i - 1] = left.cells[i - 1] + CELLSIZE - right.cells[i - 1];

                    for(size_t j = i; j < res.len - 1; ++j) {
                        if(res.cells[j] == 0) {
                            res.cells[j] = CELLSIZE - 1;
                        }
                        else {
                            res.cells[j]--;
                            break;
                        }
                        
                    }
                }
            }
        }
        else {
            throw(-1);
        }

        res.cells.shrink_to_fit();
        res.len = res.cells.size();

        return res;
    }

    TVeryLong comp(const TVeryLong& left, const TVeryLong& right) {

        TVeryLong res(left.len + right.len);
        //printf("%d %d\n%d\n", left.cells.size(), right.cells.size(), res.cells.size());
        for (size_t i = 0; i < right.len; ++i) {
            for (size_t j = 0; j < left.len; ++j) {
                //printf("%d\n", i + j + 1);

                res.cells[j + i] += left.cells[j] * right.cells[i] % CELLSIZE;
                if (res.cells[j + i] >= CELLSIZE) {
                    res.cells[j + i + 1] += res.cells[j + i] / CELLSIZE;
                    res.cells[j + i] = res.cells[j + i] % CELLSIZE;
                }
                res.cells[j + i + 1] += left.cells[j] * right.cells[i] / CELLSIZE;

            }

        }

        res.cells.shrink_to_fit();
        res.len = res.cells.size();

        return res;
    }

    TVeryLong div(const TVeryLong& left, const TVeryLong& right) {
        TVeryLong res(left.len);
        res.len = left.len;
        long long rest = 0;
        if (right.len == 1) {
            for (size_t i = left.len; i > 0; --i) {
                res.cells[i - 1] = (left.cells[i - 1] + rest * CELLSIZE) / right.cells[0];
                rest = (left.cells[i - 1] + rest * CELLSIZE) % right.cells[0];
            }
        }

        return res;
    }

    TVeryLong div(const TVeryLong& left, const long long& right) {
        TVeryLong res(left.len);
        res.len = left.len;
        long long rest = 0;
        if (right < CELLSIZE) {
            for (size_t i = left.len; i > 0; --i) {
                res.cells[i - 1] = (left.cells[i - 1] + rest * CELLSIZE) / right;
                rest = (left.cells[i - 1] + rest * CELLSIZE) % right;
            }
        }

        return res;
    }

    long long uniteCells(long long first, long long second) {
        return first * CELLSIZE + second;
    }

    TVeryLong sTake(const TVeryLong& wich, const TVeryLong& with) { //˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ T ˜˜˜˜˜ wich, ˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜˜ ˜˜ ˜˜˜ > with
        int optNum = with.len;

        if (with > wich) {
            return TVeryLong(std::vector <long long>({ 0 }));
        }

        std::vector<long long> prom;
        for (int i = wich.len - 1; i >= (int)wich.len - optNum; i--) {
            prom.push_back(wich.cells[i]);
            if (DEBUG)printf("\t\t sTake: pushed: %lld (%d)\n", wich.cells[i], i);
        }

        TVeryLong mod(prom);

        if (DEBUG) { printf("main part done |"); my::print(mod); printf("--|--"); my::print(with); printf("\n"); }
        int i = 1;
        while (with > mod && (int)wich.len >= optNum + i) {
            prom.push_back(wich.cells[(int)wich.len - optNum - i]);
            if (DEBUG)printf("\t\t sTake: afterpushed: %lld\n", wich.cells[(int)wich.len - optNum - 1]);
            mod = TVeryLong(prom);
        }

        return mod;
    }

    long long findMult(const TVeryLong& res, const TVeryLong& by) {

        if (res == TVeryLong(std::vector <long long>({ 0 }))) {
            return 0;
        }

        TVeryLong r0 = res;

        if (DEBUG)printf("\t\t expected: %lld \n", uniteCells(r0.cells[r0.len - 1], r0.cells[r0.len - 2]));

        if (res.len > by.len) { //˜˜˜˜ by ˜˜ ˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜ res, ˜˜ ˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜ r0 (xelllen ˜˜˜˜˜˜ ˜˜˜˜ ˜˜˜˜˜ ˜˜˜˜˜˜ max(long long)
            r0.cells[r0.len - 2] = uniteCells(r0.cells[r0.len - 1],r0.cells[r0.len - 2]);
            r0.cells.pop_back();
            r0.len--;
        }

        if (DEBUG)printf("\t\t posM: %lld\n", r0.cells[r0.cells.size() - 1]);

        long long multmax = r0.cells[r0.len - 1] / by.cells[by.len - 1] + 1;
        long long multmin = r0.cells[r0.len - 1] / (by.cells[by.len - 1] + 1);

        
        long long mult = (multmax + multmin)/2;

        if ((by * TVeryLong(std::vector<long long>({ mult })) < res || (by * TVeryLong(std::vector<long long>({ mult })) == res)) && by * TVeryLong(std::vector<long long>({ mult + 1 })) > res) {
            return mult;
        }


        while (!(((by * TVeryLong(std::vector<long long>({ mult })) < res) || ((by * TVeryLong(std::vector<long long>({ mult })) == res ))) && (by * TVeryLong(std::vector<long long>({ mult + 1 })) > res))) {
            if (DEBUG) { printf("\t\t|findMult: %lld,", mult); my::print(by * TVeryLong(std::vector<long long>({ mult }))); printf(" --|-- "); my::print(res); printf("\n"); }
            if (by * TVeryLong(std::vector<long long>({ mult })) < res) {
                multmin = mult;
                mult = (multmax + mult)/2;
            }
            else {
                multmax = mult;
                mult = (mult + multmin)/2;
            }
        }

        return mult;
    }

    size_t delt = 0;

    TVeryLong longDiv(const TVeryLong& left, const TVeryLong& right) {
        long long pos = left.len;
        TVeryLong del = left;
        std::vector<long long> preRes;

        if (DEBUG) { printf("del = "); my::print(del); printf("\n"); }
        size_t step = 0;
        while(pos > 0) {
            TVeryLong pr = sTake(del, right); //˜˜˜˜˜˜˜ ˜˜˜˜˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜˜

            if(pr.len - delt > 1 && step > 0) {
                for(size_t i = 1; i < pr.len - delt; ++i) {
                    preRes.push_back(0);
                }
            }

            if (DEBUG) { printf("\t step %ld. s = ", step); my::print(pr); printf("\n"); } 

            if (DEBUG) { printf("\t pr taken (len %ld). delt: %ld. 0 blocks added: %ld\n",pr.len, delt, pr.len - delt); }

            long long multPR = findMult(pr, right); //˜˜˜˜˜˜˜ ˜˜˜˜˜, ˜˜ ˜˜˜˜˜˜˜ ˜˜˜˜ ˜˜˜˜˜˜˜˜ right ˜˜˜˜˜ ˜˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜˜˜˜˜ ˜˜˜˜˜ ˜˜˜ ˜˜˜˜˜˜ pr

            if (DEBUG) {
                printf("\t  dividing... %lld", multPR); printf("\n");
                my::print(my::TVeryLong(std::vector<long long>({ multPR })) * right); printf("\n");
                printf("\t min (%lld %lu) = ", pos, pr.len); my::print(my::TVeryLong(std::vector<long long>({ multPR })) * right * (my::TVeryLong(std::vector<long long>({ CELLSIZE })) ^ my::TVeryLong(std::vector<long long>({ (long long)(pos - pr.len) })))); printf("\n");
                printf("ch! > %lld\n", std::vector<long long>({ (long long)(pos - pr.len) })[0]);
                printf("ch! > "); my::print((my::TVeryLong(std::vector<long long>({ CELLSIZE })))); printf(" !||! "); my::print(my::TVeryLong(std::vector<long long>({ (long long)(pos - pr.len) }))); printf(" !||! "); my::print((my::TVeryLong(std::vector<long long>({ CELLSIZE })) ^ my::TVeryLong(std::vector<long long>({ (long long)(pos - pr.len) })))); printf("\n");
            }

            size_t pastlen = del.len;
            del = del - (my::TVeryLong(std::vector<long long>({ multPR })) * right * (my::TVeryLong(std::vector<long long>({ CELLSIZE })) ^ my::TVeryLong(std::vector<long long>({ (long long)(pos - pr.len) })))); //˜˜˜˜˜˜˜ ˜˜˜˜˜˜˜
            size_t newlen = del.len;

            delt = pr.len - (pastlen - newlen);

            if (DEBUG) {
                printf("\t new del = "); my::print(del); printf("\n");
                printf("going to nexty step\n");
            }

            // if(pr.len > right.len) {
            //     for(size_t i = 0; i < pr.len - right.len; ++i) {
            //         preRes.push_back(0);
            //     }
            // }

            preRes.push_back (multPR);


            if (del < right) {
                if (DEBUG) {
                    printf("%lld |%lu %lu\n", pos, left.len, right.len);
                }
                /*while (pos < left.len) {
                    preRes.push_back(0);
                    pos ++;
                }
                break;*/
                TVeryLong finRes = TVeryLong(preRes);
                finRes = finRes * (my::TVeryLong(std::vector<long long>({ CELLSIZE })) ^ my::TVeryLong(std::vector<long long>({ (long long)(pos - pr.len) })));
                return finRes;
            }

            pos = del.len;
            ++step;
        }
        return TVeryLong(preRes);
    }

    
}

my::TVeryLong configurate(std::string inp) {
    std::vector<long long> rv;
    long long part = 0;

    int cdn = inp.size() % CELLLEN;
    if (DEBUG) { std::cout << inp << std::endl; }
    if (DEBUG) { printf("%lu|%d\n",inp.size(),cdn); }
    int ii = 0;
    for (size_t i = 0; i < inp.size(); ++i, ++ii) {
        if (DEBUG) { printf("foc st %lu | %c\n", i, inp[i]); }

        part *= 10;
        part += ((long long)inp[i] - '0');
        
        if ((int)(i + 1) == cdn) {
        //    printf("first point reached\n");
            rv.push_back(part);
            ii = -1;
            part = 0;
            cdn = -1;
            continue;
        }

        if ((ii + 1) % CELLLEN == 0) {
          //  printf("flush\n");
            rv.push_back(part);
            part = 0;
        }
    }


    my::TVeryLong res(rv);
    return res;
}

int main() {


    char inpC;
    int mode = 0;
    my::TVeryLong op1;
    my::TVeryLong op2;
    my::TVeryLong res;
    std::string former;
    char trash = 0;
    int cheeeeeeeee = 0;

    if (DEBUG) printf(">");
    while (scanf("%c", &inpC) != EOF) {

        if (mode == 0) {

            if (inpC >= '0' && inpC <= '9') {
                former.push_back(inpC);
            }
            else 
            if (inpC == '\n') {
                mode = 1;
                op1 = configurate(former);
                my::shrinkLeadNulls(op1);
                if (DEBUG) { printf("\t"); my::print(op1); printf("\n"); }
                if (DEBUG) { std::cout << op1 << "\n"; }
                former.clear();
            }
        }
        else 
        if (mode == 1) {
            if (inpC >= '0' && inpC <= '9') {
                former.push_back(inpC);
            }
            else
            if (inpC == '\n') {
                mode = 2;
                op2 = configurate(former);
                my::shrinkLeadNulls(op2);
                if (DEBUG) { printf("\t"); my::print(op2); printf("\n"); }
                if (DEBUG) { std::cout << op2 << "\n"; }
                former.clear();
            }
        }
        else 
        if (mode == 2) {
            if (inpC == '+') {
                res = op1 + op2;
            }
            else
            if (inpC == '-') {
                try {
                    res = op1 - op2;
                }
                catch (int err) {
                    cheeeeeeeee = scanf("%c", &trash);
                    printf("Error\n");
                    mode = 0;
                    continue;
                }
            }
            else
            if (inpC == '*') {
                res = op1 * op2;
            }
            else
            if (inpC == '/') {

                if (DEBUG) printf("\n");

                try {
                    res = op1 / op2;
                }
                catch (int err) {
                    cheeeeeeeee = scanf("%c", &trash);
                    printf("Error\n");
                    mode = 0;
                    continue;
                }
            }
            else
            if (inpC == '^') {
                try {
                    res = op1 ^ op2;
                }
                catch (int err) {
                    cheeeeeeeee = scanf("%c", &trash);
                    printf("Error\n");
                    mode = 0;
                    continue;
                }
            }
            else
            if (inpC == '>') {
                if (op1 > op2) {
                    printf("true\n");
                }
                else {
                    printf("false\n");
                }
                cheeeeeeeee = scanf("%c", &trash);
                mode = 0;
                continue;
            }
            else
            if (inpC == '<') {
                if (op1 < op2) {
                    printf("true\n");
                }
                else {
                    printf("false\n");
                }
                cheeeeeeeee = scanf("%c", &trash);
                mode = 0;
                continue;
            }
            else
            if (inpC == '=') {
                if (op1 == op2) {
                    printf("true\n");
                }
                else {
                    printf("false\n");
                }
                cheeeeeeeee = scanf("%c", &trash);
                mode = 0;
                continue;
            }
            else {
                cheeeeeeeee = scanf("%c", &trash);
                printf("Error\n");
                mode = 0;
                continue;
            }

            cheeeeeeeee = scanf("%c", &trash);

            my::shrinkLeadNulls(res);
            if (DEBUG) {printf("\t"); my::print(res); printf("\n");}
            std::cout << res << "\n";
            mode = 0;

            if(cheeeeeeeee == 0) {
                continue;
            }

        }
        if (DEBUG) printf(">");
    }
    return 0;

}
