// POJ 1081 - You Who?
// Model: gpt-5.6-terra
// Enumerate the two balanced classes, maintaining each assigned student's
// number of strangers in that class.  Prune a partial assignment as soon as
// it cannot improve the best maximum loneliness found so far.
// The input is one whitespace-separated database ending at EOF; student ids
// need not appear in record order.
#include <iostream>
#include <cstring>
using namespace std;

int n;
bool knows[30][30];
int side[30], lonely[2][30];
int best;

void search(int v, int cnt0, int cnt1, int current) {
    int cap = (n + 1) / 2;
    if (current >= best) return;
    if (v == n) {
        best = current;
        return;
    }
    for (int s = 0; s < 2; ++s) {
        int cnt = s == 0 ? cnt0 : cnt1;
        if (cnt == cap) continue;
        int changed[30], changes = 0;
        int value = 0;
        for (int i = 0; i < v; ++i) {
            if (side[i] == s && !knows[v][i]) {
                changed[changes++] = i;
                ++lonely[s][i];
                ++value;
            }
        }
        lonely[s][v] = value;
        int next = current;
        if (value > next) next = value;
        for (int i = 0; i < changes; ++i)
            if (lonely[s][changed[i]] > next) next = lonely[s][changed[i]];
        if (s == 0) {
            side[v] = 0;
            search(v + 1, cnt0 + 1, cnt1, next);
        } else {
            side[v] = 1;
            search(v + 1, cnt0, cnt1 + 1, next);
        }
        for (int i = 0; i < changes; ++i) --lonely[s][changed[i]];
        lonely[s][v] = 0;
    }
}

int main() {
    memset(knows, 0, sizeof(knows));
    int id, count, other;
    n = 0;
    while (cin >> id >> count) {
        if (id > n) n = id;
        --id;
        for (int i = 0; i < count; ++i) {
            cin >> other;
            if (other > n) n = other;
            knows[id][other - 1] = true;
        }
    }
    for (int i = 0; i < n; ++i) knows[i][i] = true;
    best = n;
    memset(lonely, 0, sizeof(lonely));
    side[0] = 0;
    search(1, 1, 0, 0);
    cout << best << '\n';
    return 0;
}
