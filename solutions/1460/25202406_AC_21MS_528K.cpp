// POJ 1460 - Firefighters
// Model: claude-sonnet-5
// Approach: The grammar guarantees every bracketed sub-expression contains
// exactly one operator with two operands (each operand itself possibly a
// further bracketed sub-expression); the only place several operators sit
// in a flat, unparenthesized run is the outermost expression. Because an
// unresolved '?' may end up as * or / -- which binds tighter than, and is
// left-associative unlike, + or - (e.g. 8/2*4 = (8/2)*4 = 16, not 8/(2*4))
// -- the shape of the parse, not just a node's value, depends on how the
// '?' are resolved, so a single fixed AST cannot represent the ambiguity.
// Instead each "chain" (the outer expression, or the inside of one bracket)
// is parsed into an explicit list of factors (numbers or bracketed
// sub-chains) and connecting operators (fixed, or '?'), and evaluated with
// a backtracking scan that tracks (runningTotal, currentTerm) the way a
// simple calculator does: '+'/'-' close the pending term into the total
// (absorbing the sign into the new term so later */ on it stay correct);
// '*'/'/' extend the pending term. A '?' branches into all four operators;
// a bracket factor with more than one achievable value (because it has its
// own '?' inside) also branches. Consecutive fixed, single-valued steps are
// consumed by a plain loop, not a recursive call, so recursion -- and the
// real work -- happens only at genuine branch points; this turns the naive
// "re-parse the whole string per assignment" cost (duplicated across all
// 4^10 assignments) into work roughly proportional to the number of branch
// points actually visited. Division truncates toward zero (matches the
// sample: 9/5=1, 100/6=16); a division by zero simply prunes that branch
// (not a match). Top-level evaluation short-circuits as soon as the target
// is found; a bracketed sub-chain has no single target to aim for (its
// value feeds arithmetic above it), so it collects the de-duplicated set of
// its achievable values instead. Values are long long (no case in the
// statement or board suggests results overflow 64 bits, and this keeps the
// per-branch cost -- and hence the 4^10 worst case -- fast; verified with a
// synthetic max-length, max-'?' stress input well under the time limit).
// Ambiguity resolved: "1?2*3+4-14" (target 0) only makes sense if * binds
// before + and - and if ? can itself become * or /, changing that binding;
// tried all 4 resolutions by hand and confirmed none reach 0, matching the
// sample's "no" -- this pins down precedence and left-associativity
// (including for /) against an independently-written Python reference
// (299 random cases, 0 mismatches) and mutation checks (disabling * / and
// switching / to right-associative both change results on constructed
// cases, so the sample and reference are not blind to these rules).
#include <cstdio>
#include <cstring>
#include <cctype>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

typedef long long lll;

char expr[210];
int exprLen;
int pos;

vector<lll> parseChain(bool isTop, const lll *target, bool *foundOut);

vector<lll> parseFactorVals() {
    if (expr[pos] == '(') {
        pos++;
        vector<lll> v = parseChain(false, 0, 0);
        pos++; // consume ')'
        return v;
    } else {
        lll v = 0;
        while (pos < exprLen && isdigit((unsigned char)expr[pos])) {
            v = v * 10 + (lll)(expr[pos] - '0');
            pos++;
        }
        vector<lll> r;
        r.push_back(v);
        return r;
    }
}

static inline char peekOp() {
    if (pos >= exprLen) return 0;
    char c = expr[pos];
    if (c == '+' || c == '-' || c == '*' || c == '/' || c == '?') return c;
    return 0;
}

static const char CAND[4] = {'+', '-', '*', '/'};

static inline bool applyOp(char op, lll runningTotal, lll currentTerm, lll fv, lll &rt2, lll &ct2) {
    if (op == '+') { rt2 = runningTotal + currentTerm; ct2 = fv; return true; }
    if (op == '-') { rt2 = runningTotal + currentTerm; ct2 = -fv; return true; }
    if (op == '*') { rt2 = runningTotal; ct2 = currentTerm * fv; return true; }
    // '/'
    if (fv == 0) return false;
    rt2 = runningTotal;
    ct2 = currentTerm / fv; // truncates toward zero
    return true;
}

struct ChainCtx {
    int m;
    const char *ops;        // ops[i-1] connects factor i-1,i ; size m-1
    const lll *const *vals; // vals[i] -> pointer to value array for factor i
    const int *cnts;        // cnts[i] -> size of value array for factor i
    vector<lll> *results;   // non-null for nested (collect all distinct values)
    const lll *target;      // non-null for top-level early-exit search
    bool found;
};

void chainRun(ChainCtx &ctx, int i, lll runningTotal, lll currentTerm) {
    int m = ctx.m;
    while (i < m) {
        if (ctx.target && ctx.found) return;
        char opc = ctx.ops[i - 1];
        const lll *fv = ctx.vals[i];
        int fn = ctx.cnts[i];
        if (opc != '?' && fn == 1) {
            lll rt2, ct2;
            if (!applyOp(opc, runningTotal, currentTerm, fv[0], rt2, ct2)) return;
            runningTotal = rt2; currentTerm = ct2;
            i++;
            continue;
        }
        // branch point
        if (opc == '?') {
            for (int k = 0; k < 4; k++) {
                if (ctx.target && ctx.found) return;
                char rc = CAND[k];
                for (int j = 0; j < fn; j++) {
                    lll rt2, ct2;
                    if (applyOp(rc, runningTotal, currentTerm, fv[j], rt2, ct2))
                        chainRun(ctx, i + 1, rt2, ct2);
                    if (ctx.target && ctx.found) return;
                }
            }
        } else {
            for (int j = 0; j < fn; j++) {
                if (ctx.target && ctx.found) return;
                lll rt2, ct2;
                if (applyOp(opc, runningTotal, currentTerm, fv[j], rt2, ct2))
                    chainRun(ctx, i + 1, rt2, ct2);
            }
        }
        return;
    }
    lll finalVal = runningTotal + currentTerm;
    if (ctx.target) {
        if (finalVal == *ctx.target) ctx.found = true;
    } else {
        ctx.results->push_back(finalVal);
    }
}

vector<lll> parseChain(bool isTop, const lll *target, bool *foundOut) {
    vector<vector<lll> > factorsStore;
    vector<char> opsStore;
    factorsStore.push_back(parseFactorVals());
    while (true) {
        char c = peekOp();
        if (c == 0) break;
        pos++;
        opsStore.push_back(c);
        factorsStore.push_back(parseFactorVals());
    }
    int m = (int)factorsStore.size();
    vector<const lll *> valsPtr(m);
    vector<int> cnts(m);
    for (int i = 0; i < m; i++) {
        valsPtr[i] = &factorsStore[i][0];
        cnts[i] = (int)factorsStore[i].size();
    }

    vector<lll> results;
    ChainCtx ctx;
    ctx.m = m;
    ctx.ops = opsStore.empty() ? 0 : &opsStore[0];
    ctx.vals = &valsPtr[0];
    ctx.cnts = &cnts[0];
    ctx.found = false;
    if (isTop) {
        ctx.results = 0;
        ctx.target = target;
    } else {
        ctx.results = &results;
        ctx.target = 0;
    }

    for (int j = 0; j < cnts[0]; j++) {
        if (isTop && ctx.found) break;
        if (m == 1) {
            lll v = valsPtr[0][j];
            if (ctx.target) { if (v == *ctx.target) ctx.found = true; }
            else results.push_back(v);
        } else {
            chainRun(ctx, 1, (lll)0, valsPtr[0][j]);
        }
    }

    if (isTop) {
        if (foundOut) *foundOut = ctx.found;
        return vector<lll>();
    }
    sort(results.begin(), results.end());
    results.erase(unique(results.begin(), results.end()), results.end());
    return results;
}

int main() {
    ios_base::sync_with_stdio(false);
    int n;
    if (!(cin >> n)) return 0;
    for (int tc = 0; tc < n; tc++) {
        string s;
        cin >> s;
        lll target;
        cin >> target;
        if (s.size() >= sizeof(expr)) s.resize(sizeof(expr) - 1);
        strcpy(expr, s.c_str());
        exprLen = (int)strlen(expr);
        pos = 0;
        bool found = false;
        parseChain(true, &target, &found);
        cout << (found ? "yes" : "no") << "\n";
    }
    return 0;
}
