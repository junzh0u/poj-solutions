// POJ 2331 - Water pipe
// Model: gpt-5.6-terra
// Iterative deepening searches the horizontal coordinate first and then the
// vertical one.  The two coordinate moves commute, so this loses no path while
// letting both phases share the bounded inventory of pipe segments.  Reverse
// BFS distances with unlimited inventory give an admissible pruning bound.
#include <algorithm>
#include <cstring>
#include <iostream>
#include <queue>
using namespace std;

int x_start, y_start, x_target, y_target, k, limit;
int length_[4], left_[4], lower_[2][1001];

bool search(int axis, int pos, int used) {
    if (used + lower_[axis][pos] > limit) return false;
    if (axis == 0 && pos == x_target) return search(1, y_start, used);
    if (axis == 1 && pos == y_target) return true;

    int target = axis == 0 ? x_target : y_target;
    for (int i = 0; i < k; ++i) {
        if (left_[i] == 0) continue;
        int first = pos + (target >= pos ? length_[i] : -length_[i]);
        int second = pos + (target >= pos ? -length_[i] : length_[i]);
        if (first >= 1 && first <= 1000) {
            --left_[i];
            if (search(axis, first, used + 1)) return true;
            ++left_[i];
        }
        if (second >= 1 && second <= 1000) {
            --left_[i];
            if (search(axis, second, used + 1)) return true;
            ++left_[i];
        }
    }
    return false;
}

void build_lower(int axis, int target) {
    for (int i = 1; i <= 1000; ++i) lower_[axis][i] = -1;
    queue<int> q;
    lower_[axis][target] = 0;
    q.push(target);
    while (!q.empty()) {
        int pos = q.front();
        q.pop();
        for (int i = 0; i < k; ++i) {
            int next = pos - length_[i];
            if (next >= 1 && lower_[axis][next] < 0) {
                lower_[axis][next] = lower_[axis][pos] + 1;
                q.push(next);
            }
            next = pos + length_[i];
            if (next <= 1000 && lower_[axis][next] < 0) {
                lower_[axis][next] = lower_[axis][pos] + 1;
                q.push(next);
            }
        }
    }
}

int main() {
    if (!(cin >> x_start >> y_start >> x_target >> y_target >> k)) return 0;
    for (int i = 0; i < k; ++i) cin >> length_[i];
    int total = 0;
    for (int i = 0; i < k; ++i) {
        cin >> left_[i];
        total += left_[i];
    }
    build_lower(0, x_target);
    build_lower(1, y_target);
    for (limit = 0; limit <= total; ++limit)
        if (search(0, x_start, 0)) {
            cout << limit << '\n';
            return 0;
        }
    cout << -1 << '\n';
    return 0;
}
