// POJ 2848 - Kindergarten Graduation
// Model: gpt-6-astra
// Search only forward slides and hops over the opposite sex, memoizing dead
// positions. Each mixed pair crosses once, so every successful path has
// g*b hops and g+b slides. Cache answers for repeated class compositions.
// Same-sex hops are legal too; use them directly for a one-sex class.
#include <iostream>
#include <string>
#include <set>
#include <algorithm>
using namespace std;
string goal, moves;
set<string> dead;
string cache[25][25];
bool ready[25][25];
bool solve(string &s, int empty) {
    if (s == goal) return true;
    if (dead.count(s)) return false;
    const int delta[4] = {2, -2, 1, -1};
    const char child[4] = {'B', 'G', 'B', 'G'};
    const char code[4] = {'h', 'H', 's', 'S'};
    for (int i = 0; i < 4; ++i) {
        int p = empty + delta[i];
        if (p < 0 || p >= (int)s.size() || s[p] != child[i]) continue;
        if (i < 2 && s[empty + delta[i]/2] == child[i]) continue;
        swap(s[empty], s[p]);
        moves += code[i];
        if (solve(s, p)) return true;
        moves.erase(moves.size()-1);
        swap(s[empty], s[p]);
    }
    dead.insert(s);
    return false;
}
int main() {
    int tests;
    cin >> tests;
    while (tests--) {
        int id, g, b;
        cin >> id >> g >> b;
        if (!ready[g][b]) {
            moves.clear();
            if (b == 0) moves = string(g/2, 'H') + string(g%2, 'S');
            else if (g == 0) moves = string(b/2, 'h') + string(b%2, 's');
            else {
                string s = string(g, 'G') + "_" + string(b, 'B');
                goal = string(b, 'B') + "_" + string(g, 'G');
                dead.clear();
                solve(s, g);
            }
            cache[g][b] = moves;
            ready[g][b] = true;
        }
        const string &answer = cache[g][b];
        cout << id << ' ' << answer.size() << '\n';
        for (unsigned int i = 0; i < answer.size(); i += 50)
            cout << answer.substr(i, 50) << '\n';
        cout << '\n';
    }
    return 0;
}
