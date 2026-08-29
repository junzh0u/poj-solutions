// POJ 1684 - Dynamic Declaration Language (DDL)
// Model: claude-sonnet-5
// Approach: Direct simulation of the DDL interpreter. Each variable (a
// single case-sensitive letter, up to 52 possible) carries: hasDecl
// (permanently true once the first successful Dcl for it executes),
// referenced (reset on each successful (re)declaration, set true whenever
// the variable is used in an assignment, a conditional goto's test, inc,
// or dec), and value. A Dcl is erroneous (error 1) iff hasDecl is already
// true and referenced is false; the erroneous statement leaves the prior
// declaration untouched and simply falls through to the next line. A
// reference to a variable with hasDecl false is error 2 and also falls
// through. Goto/End otherwise transfer control as specified. The problem
// guarantees no syntax errors and guaranteed termination via End, so a
// straightforward line-pointer simulation suffices. Tokens are separated
// by (possibly several) blanks; an extra space is inserted around '=' so
// an assignment with no surrounding blanks would still tokenize correctly,
// though the statement implies blanks are always present.
// Keywords (Dcl/Goto/Inc/Dec/End) are matched case-insensitively; the
// single-letter variable id itself is case-sensitive per the statement.

#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
using namespace std;

int main(){
    int n;
    if(!(cin >> n)) return 0;
    for (int tc = 1; tc <= n; ++tc) {
        int s;
        cin >> s;
        cin.ignore(1000000, '\n');
        vector<string> lines(s);
        for (int i = 0; i < s; ++i) {
            getline(cin, lines[i]);
        }
        bool hasDecl[256];
        bool referenced[256];
        int value[256];
        for (int i = 0; i < 256; ++i) { hasDecl[i] = false; referenced[i] = false; value[i] = 0; }

        printf("%d\n", tc);

        int line = 1;
        while (line >= 1 && line <= s) {
            const string &raw = lines[line-1];
            string t;
            t.reserve(raw.size() + 4);
            for (size_t i = 0; i < raw.size(); ++i) {
                if (raw[i] == '=') t += " = ";
                else t += raw[i];
            }
            istringstream iss(t);
            vector<string> tok;
            string w;
            while (iss >> w) tok.push_back(w);
            if (tok.empty()) { line++; continue; }
            string kw = tok[0];
            for (size_t i = 0; i < kw.size(); ++i) kw[i] = (char)toupper((unsigned char)kw[i]);

            if (kw == "DCL") {
                unsigned char id = (unsigned char)tok[1][0];
                if (!hasDecl[id]) {
                    hasDecl[id] = true;
                    value[id] = 0;
                    referenced[id] = false;
                } else if (!referenced[id]) {
                    printf("%d 1\n", line);
                } else {
                    value[id] = 0;
                    referenced[id] = false;
                }
                line++;
            } else if (kw == "GOTO") {
                if (tok.size() == 2) {
                    line = atoi(tok[1].c_str());
                } else {
                    unsigned char id = (unsigned char)tok[1][0];
                    int label = atoi(tok[2].c_str());
                    if (!hasDecl[id]) {
                        printf("%d 2\n", line);
                        line++;
                    } else {
                        referenced[id] = true;
                        line = (value[id] > 0) ? label : (line + 1);
                    }
                }
            } else if (kw == "INC" || kw == "DEC") {
                unsigned char id = (unsigned char)tok[1][0];
                if (!hasDecl[id]) {
                    printf("%d 2\n", line);
                } else {
                    referenced[id] = true;
                    value[id] += (kw == "INC") ? 1 : -1;
                }
                line++;
            } else if (kw == "END") {
                break;
            } else {
                unsigned char id = (unsigned char)tok[0][0];
                int val = atoi(tok[2].c_str());
                if (!hasDecl[id]) {
                    printf("%d 2\n", line);
                } else {
                    value[id] = val;
                    referenced[id] = true;
                }
                line++;
            }
        }
    }
    return 0;
}
