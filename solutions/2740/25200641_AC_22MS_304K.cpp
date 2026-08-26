// POJ 2740 - Book Replacement
// Model: gpt-5.6-terra
// Simulate the round-robin request order and the desks.  Each desk entry
// records its last request time, so the LRU victim on D1 is selected exactly.
// A temporary placement is kept as an ordinary book location; this matters
// when it shares a desk with the displaced D1 book.
#include <iostream>
#include <vector>

using namespace std;

struct Book {
    int id;
    int used;
};

int main() {
    int m, c, n;
    while (cin >> m >> c >> n && (m || c || n)) {
        vector< vector<int> > request(n);
        int total = 0;
        int i, j;
        for (i = 0; i < n; ++i) {
            int k;
            cin >> k;
            request[i].resize(k);
            total += k;
            for (j = 0; j < k; ++j) cin >> request[i][j];
        }

        vector<int> next(n, 0);
        vector< vector<Book> > desk(m);
        long cost = 0;
        int clock = 0;
        int done = 0;
        while (done < total) {
            for (i = 0; i < n; ++i) {
                if (next[i] == (int)request[i].size()) continue;
                int id = request[i][next[i]++];
                ++done;
                ++clock;

                int from = m;
                for (j = 0; j < m; ++j) {
                    int p;
                    for (p = 0; p < (int)desk[j].size(); ++p) {
                        if (desk[j][p].id == id) {
                            from = j;
                            desk[j].erase(desk[j].begin() + p);
                            break;
                        }
                    }
                    if (from != m) break;
                }
                cost += from + 1;
                Book requested;
                requested.id = id;
                requested.used = clock;

                if ((int)desk[0].size() < c) {
                    desk[0].push_back(requested);
                    cost += 1;
                    continue;
                }

                int temporary = m;
                for (j = 0; j < m; ++j) {
                    if ((int)desk[j].size() < c) {
                        temporary = j;
                        desk[j].push_back(requested);
                        break;
                    }
                }
                cost += temporary + 1;

                int victim = 0;
                for (j = 1; j < (int)desk[0].size(); ++j)
                    if (desk[0][j].used < desk[0][victim].used) victim = j;
                Book old = desk[0][victim];
                desk[0].erase(desk[0].begin() + victim);
                cost += 1;

                int target = m;
                for (j = 1; j < m; ++j) {
                    if ((int)desk[j].size() < c) {
                        target = j;
                        desk[j].push_back(old);
                        break;
                    }
                }
                cost += target + 1;

                if (temporary < m) {
                    for (j = 0; j < (int)desk[temporary].size(); ++j)
                        if (desk[temporary][j].id == id) {
                            desk[temporary].erase(desk[temporary].begin() + j);
                            break;
                        }
                }
                cost += temporary + 1;
                desk[0].push_back(requested);
                cost += 1;
            }
        }
        cout << cost << '\n';
    }
    return 0;
}
