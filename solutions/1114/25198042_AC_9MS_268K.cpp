// POJ 1114 - Chemical Reactions
// Model: claude-sonnet-5
// Recursive-descent parser over the chemical-formula grammar; each formula
// is reduced to a map<string,int> of element -> total occurrence count,
// and two formulas are equivalent iff their maps compare equal (map::operator==
// checks same key set and same values, which is exactly "same elements with
// same totals"; keys with count 0 are pruned so an element multiplied by an
// explicit coefficient of 0 does not spuriously appear as a mismatched key).
// Ambiguity note: the BNF states <number> cannot have a leading 0, but a
// board post claims a coefficient "can be 0" -- defensively the number
// parser also accepts a leading 0, and any resulting zero counts are
// dropped before comparison, so this costs nothing if the claim is false.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <map>
#include <cctype>
using namespace std;

string S;
int idx_, n_;

long parseNumber() {
    if (idx_ < n_ && isdigit((unsigned char)S[idx_])) {
        long v = 0;
        while (idx_ < n_ && isdigit((unsigned char)S[idx_])) {
            v = v * 10 + (S[idx_] - '0');
            idx_++;
        }
        return v;
    }
    return 1;
}

map<string,int> parseSequence(); // fwd

map<string,int> parseElement() {
    map<string,int> m;
    if (idx_ < n_ && S[idx_] == '(') {
        idx_++; // consume '('
        m = parseSequence();
        if (idx_ < n_ && S[idx_] == ')') idx_++; // consume ')'
    } else {
        string name;
        name += S[idx_++];
        if (idx_ < n_ && islower((unsigned char)S[idx_])) {
            name += S[idx_++];
        }
        m[name] = 1;
    }
    return m;
}

map<string,int> parseSequence() {
    map<string,int> result;
    while (idx_ < n_ && S[idx_] != '+' && S[idx_] != ')') {
        map<string,int> em = parseElement();
        long mult = parseNumber();
        for (map<string,int>::iterator it = em.begin(); it != em.end(); ++it) {
            result[it->first] += it->second * (int)mult;
        }
    }
    return result;
}

map<string,int> parseFormula(const string& f) {
    S = f;
    idx_ = 0;
    n_ = (int)S.size();
    map<string,int> total;
    while (idx_ < n_) {
        long coeff = parseNumber();
        map<string,int> seq = parseSequence();
        for (map<string,int>::iterator it = seq.begin(); it != seq.end(); ++it) {
            total[it->first] += it->second * (int)coeff;
        }
        if (idx_ < n_ && S[idx_] == '+') idx_++;
    }
    // prune zero-count entries so an absent element and an explicit-zero
    // element compare equal
    for (map<string,int>::iterator it = total.begin(); it != total.end(); ) {
        if (it->second == 0) {
            map<string,int>::iterator del = it++;
            total.erase(del);
        } else {
            ++it;
        }
    }
    return total;
}

string trimLine(char* buf) {
    string s(buf);
    while (!s.empty() && (s[s.size()-1] == '\n' || s[s.size()-1] == '\r' || s[s.size()-1] == ' '))
        s.erase(s.size()-1);
    return s;
}

int main() {
    char buf[1024];
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    string left = trimLine(buf);
    map<string,int> leftMap = parseFormula(left);

    int n = 0;
    if (fgets(buf, sizeof(buf), stdin)) {
        n = atoi(buf);
    }

    for (int i = 0; i < n; i++) {
        if (!fgets(buf, sizeof(buf), stdin)) break;
        string right = trimLine(buf);
        map<string,int> rightMap = parseFormula(right);
        printf("%s%s%s\n", left.c_str(), (leftMap == rightMap) ? "==" : "!=", right.c_str());
    }
    return 0;
}
