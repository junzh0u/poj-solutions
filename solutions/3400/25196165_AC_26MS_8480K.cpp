// POJ 3400 - Dropping the stones
// Model: gpt-5.6-terra
// Dynamic programming over the stones already dropped into each bunker. The
// current receiver changes only after the just-dropped stone makes it more
// than D heavier; equality does not switch the gutter.

#include <iostream>
#include <cstring>
using namespace std;

int n;
int d;
int weight[10], value[10];
int sumWeight[1 << 10];
int sumValue[1 << 10];
int memo[1 << 10][1 << 10][2];

int solve(int inA, int inB, int receiver) {
    int &ans = memo[inA][inB][receiver];
    if (ans != -1) return ans;
    int used = inA | inB;
    if (used == (1 << n) - 1) return ans = sumValue[inB];

    ans = 0;
    int i;
    for (i = 0; i < n; ++i) {
        int bit = 1 << i;
        if (used & bit) continue;
        if (receiver == 0) {
            int nextA = inA | bit;
            int nextReceiver = 0;
            if (sumWeight[nextA] - sumWeight[inB] > d) nextReceiver = 1;
            int candidate = solve(nextA, inB, nextReceiver);
            if (candidate > ans) ans = candidate;
        } else {
            int nextB = inB | bit;
            int nextReceiver = 1;
            if (sumWeight[nextB] - sumWeight[inA] > d) nextReceiver = 0;
            int candidate = solve(inA, nextB, nextReceiver);
            if (candidate > ans) ans = candidate;
        }
    }
    return ans;
}

int main() {
    if (!(cin >> n >> d)) return 0;
    int i, mask;
    for (i = 0; i < n; ++i) cin >> weight[i] >> value[i];
    for (mask = 1; mask < (1 << n); ++mask) {
        int bit = mask & -mask;
        int previous = mask - bit;
        int index = 0;
        while ((1 << index) != bit) ++index;
        sumWeight[mask] = sumWeight[previous] + weight[index];
        sumValue[mask] = sumValue[previous] + value[index];
    }
    memset(memo, -1, sizeof(memo));
    cout << solve(0, 0, 0) << '\n';
    return 0;
}
