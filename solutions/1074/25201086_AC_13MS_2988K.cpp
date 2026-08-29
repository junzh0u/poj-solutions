// POJ 1074 - Parallel Expectations
// Model: claude-sonnet-5
//
// DP over (i,j) = instructions executed of A, B. Each instruction is an
// affine map on a small state vector (shared variables + 2 registers per
// program); we track the unnormalized U(i,j)=P(i,j)*E[state|reach(i,j)],
// which propagates linearly since affine maps commute with expectation,
// and P(nA,nB)=1 so U(nA,nB) is the final answer directly. The coin flip
// is fair only while both programs have instructions left (board #92808:
// interleavings are not uniformly likely). Variable names fold to
// lowercase (case-insensitive per board #118030); tokens may be jammed
// with zero whitespace so a hand-rolled tokenizer replaces whitespace
// splitting; blank lines may appear anywhere (board #97091) and are
// absorbed as whitespace. A tiny negative expected value rounding to
// "-0.0000" is a real answer, not a bug (board #350193/#360643) - never
// clamp near-zero or sign.
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

static string gbuf;
static size_t gpos;

static void initBuf() {
    gbuf.clear();
    char buf[1 << 16];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), stdin)) > 0) {
        gbuf.append(buf, n);
    }
    gpos = 0;
}

struct Token {
    int type; // 0=ident,1=num,2=assign,3=plus,4=minus,5=eof
    string s;
    int v;
};

static Token nextToken() {
    Token t;
    while (gpos < gbuf.size() && isspace((unsigned char)gbuf[gpos])) gpos++;
    if (gpos >= gbuf.size()) { t.type = 5; return t; }
    char c = gbuf[gpos];
    if (isalpha((unsigned char)c)) {
        size_t start = gpos;
        while (gpos < gbuf.size() && isalnum((unsigned char)gbuf[gpos])) gpos++;
        t.type = 0;
        t.s = gbuf.substr(start, gpos - start);
        for (size_t i = 0; i < t.s.size(); i++) t.s[i] = (char)tolower((unsigned char)t.s[i]);
        return t;
    } else if (isdigit((unsigned char)c)) {
        size_t start = gpos;
        while (gpos < gbuf.size() && isdigit((unsigned char)gbuf[gpos])) gpos++;
        t.type = 1;
        t.v = atoi(gbuf.substr(start, gpos - start).c_str());
        return t;
    } else if (c == ':') {
        gpos++;
        if (gpos < gbuf.size() && gbuf[gpos] == '=') gpos++;
        t.type = 2;
        return t;
    } else if (c == '+') {
        gpos++; t.type = 3; return t;
    } else if (c == '-') {
        gpos++; t.type = 4; return t;
    } else {
        gpos++;
        return nextToken();
    }
}

struct Operand {
    bool isConst;
    int constVal;
    string varName;
};

struct Command {
    string target;
    Operand op1, op2;
    char oper;
};

static Operand readOperand(const Token &t) {
    Operand o;
    if (t.type == 1) { o.isConst = true; o.constVal = t.v; o.varName = ""; }
    else { o.isConst = false; o.constVal = 0; o.varName = t.s; }
    return o;
}

static vector<Command> readProgram() {
    vector<Command> cmds;
    while (true) {
        Token t = nextToken();
        if (t.type == 0 && t.s == "end") break;
        Command cmd;
        cmd.target = t.s;
        nextToken(); // ":="
        Token o1 = nextToken();
        cmd.op1 = readOperand(o1);
        Token opTok = nextToken();
        cmd.oper = (opTok.type == 3) ? '+' : '-';
        Token o2 = nextToken();
        cmd.op2 = readOperand(o2);
        cmds.push_back(cmd);
    }
    return cmds;
}

struct Instr {
    int target;
    int src1; double c1;
    int src2; double c2;
    double konst;
};

// Generous headroom past the stated 100-instruction/10-variable bounds,
// in case test data exceeds them (a known POJ pitfall pattern).
static const int MAXI = 161;   // instruction-count dimension (>100 needed)
static const int MAXS = 28;    // state slots: variables + 4 registers
static double prob[MAXI][MAXI];
static double vecArr[MAXI][MAXI][MAXS];

int main() {
    initBuf();
    Token tt = nextToken();
    int T = tt.v;
    bool firstCase = true;
    for (int tc = 0; tc < T; tc++) {
        vector<Command> progA = readProgram();
        vector<Command> progB = readProgram();

        map<string, int> varIdx;
        vector<string> varNames;
        for (int p = 0; p < 2; p++) {
            vector<Command> &prog = (p == 0 ? progA : progB);
            for (size_t i = 0; i < prog.size(); i++) {
                Command &cmd = prog[i];
                if (varIdx.find(cmd.target) == varIdx.end()) {
                    varIdx[cmd.target] = (int)varNames.size();
                    varNames.push_back(cmd.target);
                }
                if (!cmd.op1.isConst && varIdx.find(cmd.op1.varName) == varIdx.end()) {
                    varIdx[cmd.op1.varName] = (int)varNames.size();
                    varNames.push_back(cmd.op1.varName);
                }
                if (!cmd.op2.isConst && varIdx.find(cmd.op2.varName) == varIdx.end()) {
                    varIdx[cmd.op2.varName] = (int)varNames.size();
                    varNames.push_back(cmd.op2.varName);
                }
            }
        }
        int numVars = (int)varNames.size();
        int R1A = numVars + 0, R2A = numVars + 1, R1B = numVars + 2, R2B = numVars + 3;
        int stateSize = numVars + 4;

        vector<Instr> instrA, instrB;
        for (int p = 0; p < 2; p++) {
            vector<Command> &prog = (p == 0 ? progA : progB);
            vector<Instr> &instrList = (p == 0 ? instrA : instrB);
            int R1 = (p == 0 ? R1A : R1B), R2 = (p == 0 ? R2A : R2B);
            for (size_t i = 0; i < prog.size(); i++) {
                Command &cmd = prog[i];
                Instr ins1;
                ins1.target = R1; ins1.src1 = -1; ins1.c1 = 0; ins1.src2 = -1; ins1.c2 = 0; ins1.konst = 0;
                if (cmd.op1.isConst) ins1.konst = cmd.op1.constVal;
                else { ins1.src1 = varIdx[cmd.op1.varName]; ins1.c1 = 1.0; }
                instrList.push_back(ins1);

                Instr ins2;
                ins2.target = R2; ins2.src1 = -1; ins2.c1 = 0; ins2.src2 = -1; ins2.c2 = 0; ins2.konst = 0;
                if (cmd.op2.isConst) ins2.konst = cmd.op2.constVal;
                else { ins2.src1 = varIdx[cmd.op2.varName]; ins2.c1 = 1.0; }
                instrList.push_back(ins2);

                Instr ins3;
                ins3.target = R1; ins3.src1 = R1; ins3.c1 = 1.0; ins3.src2 = R2;
                ins3.c2 = (cmd.oper == '+') ? 1.0 : -1.0; ins3.konst = 0;
                instrList.push_back(ins3);

                Instr ins4;
                ins4.target = varIdx[cmd.target]; ins4.src1 = R1; ins4.c1 = 1.0; ins4.src2 = -1; ins4.c2 = 0; ins4.konst = 0;
                instrList.push_back(ins4);
            }
        }
        int nA = (int)instrA.size(), nB = (int)instrB.size();

        prob[0][0] = 1.0;
        for (int k = 0; k < stateSize; k++) vecArr[0][0][k] = 0.0;

        for (int i = 0; i <= nA; i++) {
            for (int j = 0; j <= nB; j++) {
                if (i == 0 && j == 0) continue;
                double contribA[MAXS], contribB[MAXS];
                for (int k = 0; k < stateSize; k++) { contribA[k] = 0.0; contribB[k] = 0.0; }
                double pA = 0.0, pB = 0.0;
                if (i > 0) {
                    double wA = (j < nB) ? 0.5 : 1.0;
                    double prevP = prob[i - 1][j];
                    Instr &ins = instrA[i - 1];
                    for (int k = 0; k < stateSize; k++) contribA[k] = wA * vecArr[i - 1][j][k];
                    double val = ins.konst * prevP;
                    if (ins.src1 >= 0) val += ins.c1 * vecArr[i - 1][j][ins.src1];
                    if (ins.src2 >= 0) val += ins.c2 * vecArr[i - 1][j][ins.src2];
                    contribA[ins.target] = wA * val;
                    pA = wA * prevP;
                }
                if (j > 0) {
                    double wB = (i < nA) ? 0.5 : 1.0;
                    double prevP = prob[i][j - 1];
                    Instr &ins = instrB[j - 1];
                    for (int k = 0; k < stateSize; k++) contribB[k] = wB * vecArr[i][j - 1][k];
                    double val = ins.konst * prevP;
                    if (ins.src1 >= 0) val += ins.c1 * vecArr[i][j - 1][ins.src1];
                    if (ins.src2 >= 0) val += ins.c2 * vecArr[i][j - 1][ins.src2];
                    contribB[ins.target] = wB * val;
                    pB = wB * prevP;
                }
                prob[i][j] = pA + pB;
                for (int k = 0; k < stateSize; k++) vecArr[i][j][k] = contribA[k] + contribB[k];
            }
        }

        vector<pair<string, int> > sortedVars;
        for (int i = 0; i < numVars; i++) sortedVars.push_back(make_pair(varNames[i], i));
        sort(sortedVars.begin(), sortedVars.end());

        if (!firstCase) printf("\n");
        firstCase = false;
        for (size_t i = 0; i < sortedVars.size(); i++) {
            double val = vecArr[nA][nB][sortedVars[i].second];
            printf("%.4f\n", val);
        }
    }
    return 0;
}
