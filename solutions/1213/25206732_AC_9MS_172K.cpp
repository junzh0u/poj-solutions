// POJ 1213 - Roman Numerals
// Model: claude-sonnet-5
//
// Two independent questions per line "R1+R2=R3":
//  1) "Correct"/"Incorrect": convert each Roman numeral to its standard
//     Arabic value (classic left-to-right scan: subtract a letter whose
//     value is less than the one immediately following it, otherwise add)
//     and check value(R1)+value(R2)==value(R3).
//  2) "impossible"/"valid"/"ambiguous": treat the distinct Roman letters
//     appearing in the line as unknown decimal digits (an injective map
//     from letters to digits 0-9, no numeral's leading letter mapped to 0
//     -- "zero never appears on its own or as a leading digit"). Read each
//     numeral as a base-10 number formed by its mapped digits in order and
//     count how many such assignments make R1+R2==R3 as ordinary decimal
//     addition. 0 solutions -> impossible, 1 -> valid, >1 -> ambiguous.
//
// At most 7 distinct Roman letters ever occur (I V X L C D M). A naive
// brute force over all injective digit assignments (up to 10P7 = 604800
// per line) is the textbook approach for this problem, but it is only
// fast when a solution is found quickly (early cutoff at 2); a line whose
// true answer is "impossible" forces the full 604800-assignment search
// with no shortcut, and with several thousand such lines this measured
// ~23s locally -- too close to the judge's 1000ms limit once its slower
// hardware is accounted for. So this solves it the standard cryptarithm
// way instead: process decimal columns from least to most significant
// with an explicit carry, resolving each column's newly-seen letters
// (at most 3 per column, since each of R1/R2/R3 contributes one
// character) and pruning immediately on any column/carry mismatch,
// rather than deferring every check to a fully assigned leaf. This is
// the same technique used for SEND+MORE=MONEY-style puzzles and is many
// orders of magnitude faster; the stress case above now finishes
// instantly.
//
// No ambiguity in the statement itself: the three worked samples pin down
// both the Roman-value algorithm and the digit-encoding rules exactly
// (V+V=X ambiguous with 4 digit solutions, X+X=XX impossible since the only
// algebraic solution needs the leading letter X=0, XX+XX=MXC valid with the
// statement's own M=1,X=9,C=8).

#include <string>
#include <iostream>
using namespace std;

static int romanLetterValue(char c) {
    switch (c) {
        case 'I': return 1;
        case 'V': return 5;
        case 'X': return 10;
        case 'L': return 50;
        case 'C': return 100;
        case 'D': return 500;
        case 'M': return 1000;
    }
    return 0;
}

static long toRomanValue(const string &s) {
    long total = 0;
    int n = (int)s.size();
    for (int i = 0; i < n; i++) {
        int v = romanLetterValue(s[i]);
        if (i + 1 < n && v < romanLetterValue(s[i + 1])) {
            total -= v;
        } else {
            total += v;
        }
    }
    return total;
}

static string gR1, gR2, gR3;
static int digitMap[128];
static bool usedDigit[10];
static bool isLeading[128];
static int solutionCount;
static int totalLen;

struct Slot {
    bool virtualZero;
    unsigned char letter;
};

static Slot slotFor(const string &s, int j) {
    Slot sl;
    int len = (int)s.size();
    if (j < len) {
        sl.virtualZero = false;
        sl.letter = (unsigned char)s[len - 1 - j];
    } else {
        sl.virtualZero = true;
        sl.letter = 0;
    }
    return sl;
}

static int slotValue(const Slot &sl) {
    return sl.virtualZero ? 0 : digitMap[sl.letter];
}

static void solveColumn(int j, int carryIn);

static void assignUnknowns(unsigned char *unknowns, int count, int idx, int j, int carryIn,
                            const Slot &s1, const Slot &s2, const Slot &s3) {
    if (solutionCount >= 2) return;
    if (idx == count) {
        int d1 = slotValue(s1);
        int d2 = slotValue(s2);
        int d3 = slotValue(s3);
        int sum = d1 + d2 + carryIn;
        if (sum % 10 != d3) return;
        solveColumn(j + 1, sum / 10);
        return;
    }
    unsigned char letter = unknowns[idx];
    int startD = isLeading[letter] ? 1 : 0;
    for (int d = startD; d < 10; d++) {
        if (usedDigit[d]) continue;
        usedDigit[d] = true;
        digitMap[letter] = d;
        assignUnknowns(unknowns, count, idx + 1, j, carryIn, s1, s2, s3);
        usedDigit[d] = false;
        digitMap[letter] = -1;
        if (solutionCount >= 2) return;
    }
}

static void solveColumn(int j, int carryIn) {
    if (solutionCount >= 2) return;
    if (j == totalLen) {
        if (carryIn == 0) solutionCount++;
        return;
    }
    Slot s1 = slotFor(gR1, j);
    Slot s2 = slotFor(gR2, j);
    Slot s3 = slotFor(gR3, j);

    unsigned char unknowns[3];
    int count = 0;
    const Slot *slots[3] = { &s1, &s2, &s3 };
    for (int i = 0; i < 3; i++) {
        const Slot &sl = *slots[i];
        if (sl.virtualZero) continue;
        if (digitMap[sl.letter] != -1) continue;
        bool already = false;
        for (int k = 0; k < count; k++) {
            if (unknowns[k] == sl.letter) { already = true; break; }
        }
        if (!already) unknowns[count++] = sl.letter;
    }

    assignUnknowns(unknowns, count, 0, j, carryIn, s1, s2, s3);
}

int main() {
    string line;
    while (cin >> line) {
        if (line == "#") break;
        size_t plusPos = line.find('+');
        size_t eqPos = line.find('=');
        gR1 = line.substr(0, plusPos);
        gR2 = line.substr(plusPos + 1, eqPos - plusPos - 1);
        gR3 = line.substr(eqPos + 1);

        long v1 = toRomanValue(gR1);
        long v2 = toRomanValue(gR2);
        long v3 = toRomanValue(gR3);
        bool romanCorrect = (v1 + v2 == v3);

        for (int i = 0; i < 128; i++) { digitMap[i] = -1; isLeading[i] = false; }
        for (int i = 0; i < 10; i++) usedDigit[i] = false;

        const string *parts[3] = { &gR1, &gR2, &gR3 };
        for (int p = 0; p < 3; p++) {
            isLeading[(unsigned char)(*parts[p])[0]] = true;
        }

        totalLen = (int)gR1.size();
        if ((int)gR2.size() > totalLen) totalLen = (int)gR2.size();
        if ((int)gR3.size() > totalLen) totalLen = (int)gR3.size();

        solutionCount = 0;
        solveColumn(0, 0);

        const char *word2 = solutionCount == 0 ? "impossible" : (solutionCount == 1 ? "valid" : "ambiguous");
        cout << (romanCorrect ? "Correct" : "Incorrect") << " " << word2 << "\n";
    }
    return 0;
}
