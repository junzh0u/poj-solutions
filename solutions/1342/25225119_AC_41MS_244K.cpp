// POJ 1342 - Tag Trees
// Model: gpt-6.1-sol
// Each node costs one bit plus its value minus its parent's value
// (zero for the root's parent). Thus the answer is the leaf sum minus
// three times the internal-node sum, plus the number of all nodes.
// Reduce adjacent 2x2 blocks online, retaining one half-width row per
// level. Coding order is irrelevant because parents precede children.
// The board warns that the printed k bound is overstated; streaming
// avoids relying on the smaller bounds reported there.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;
vector<unsigned int> pending[21];
unsigned int firstValue[21];
int positionInRow[21], rowParity[21], widthAt[21], levels;
long long answer;
unsigned int readNumber() {
    int c; do { c = getchar(); } while(c <= ' ' && c != EOF);
    unsigned int x = 0;
    while(c >= '0' && c <= '9') { x = x * 10 + c - '0'; c = getchar(); }
    return x;
}
void feed(int level, unsigned int value) {
    if(level == levels) return;
    int col = positionInRow[level];
    if(!(col & 1)) firstValue[level] = value;
    else {
        unsigned int m = min(firstValue[level], value);
        if(!rowParity[level]) pending[level][col / 2] = m;
        else {
            m = min(m, pending[level][col / 2]);
            answer -= 3LL * m;
            feed(level + 1, m);
        }
    }
    if(++positionInRow[level] == widthAt[level]) {
        positionInRow[level] = 0;
        rowParity[level] ^= 1;
    }
}
int main() {
    int tests = readNumber();
    while(tests--) {
        levels = readNumber();
        int n = 1 << levels;
        long long count = 0, nodes = 1;
        for(int l = levels; l >= 0; --l) { count += nodes; nodes *= 4; }
        answer = count;
        for(int l = 0; l < levels; ++l) {
            widthAt[l] = n >> l;
            pending[l].assign(widthAt[l] / 2, 0);
            positionInRow[l] = rowParity[l] = 0;
        }
        for(int r = 0; r < n; ++r) for(int c = 0; c < n; ++c) {
            unsigned int x = readNumber();
            answer += x;
            feed(0, x);
        }
        cout << answer << '\n';
    }
    return 0;
}
