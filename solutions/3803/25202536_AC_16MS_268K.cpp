// POJ 3803 - Repeated Substitution with Sed
// Model: claude-sonnet-5
//
// Each substitution (alpha,beta) is applied by scanning the string left to
// right and greedily replacing every non-overlapping occurrence of alpha
// with beta (leftmost match wins whenever choices overlap). Since
// |alpha| < |beta| <= 10 for every rule, one application either leaves the
// string unchanged (alpha does not occur) or strictly increases its length.
// Because |gamma| < |delta| <= 10, every string that can ever matter has
// length between |gamma| and |delta| <= 10, so the whole reachable state
// space is small. This makes a plain BFS over strings (states) correct and
// fast: states only grow in length, so BFS visiting order is also
// nondecreasing in length/step count, and a string is skipped once its
// length exceeds |delta| (it can never shrink back) or once it has already
// been reached with fewer steps.
#include <cstdio>
#include <string>
#include <vector>
#include <queue>
#include <map>
using namespace std;

string applyRule(const string &s, const string &a, const string &b) {
    string res;
    res.reserve(s.size() + b.size());
    int n = (int)s.size(), m = (int)a.size();
    int i = 0;
    while (i < n) {
        if (i + m <= n && s.compare(i, m, a) == 0) {
            res += b;
            i += m;
        } else {
            res += s[i];
            i++;
        }
    }
    return res;
}

int main() {
    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        vector<string> alpha(n), beta(n);
        char buf1[32], buf2[32];
        for (int i = 0; i < n; i++) {
            scanf("%s %s", buf1, buf2);
            alpha[i] = buf1;
            beta[i] = buf2;
        }
        char g[32], d[32];
        scanf("%s %s", g, d);
        string gamma = g, delta = d;

        int targetLen = (int)delta.size();

        map<string, int> dist;
        queue<string> q;
        dist[gamma] = 0;
        q.push(gamma);

        int answer = -1;
        if (gamma == delta) {
            answer = 0;
        } else {
            while (!q.empty()) {
                string cur = q.front();
                q.pop();
                int curDist = dist[cur];
                for (int i = 0; i < n; i++) {
                    string nxt = applyRule(cur, alpha[i], beta[i]);
                    if (nxt == cur) continue; // no occurrence, self-loop
                    if ((int)nxt.size() > targetLen) continue; // can never shrink back
                    map<string, int>::iterator it = dist.find(nxt);
                    if (it != dist.end()) continue; // already reached with <= steps
                    dist[nxt] = curDist + 1;
                    if (nxt == delta) {
                        answer = curDist + 1;
                        goto done;
                    }
                    q.push(nxt);
                }
            }
        done:;
        }

        printf("%d\n", answer);
    }
    return 0;
}
