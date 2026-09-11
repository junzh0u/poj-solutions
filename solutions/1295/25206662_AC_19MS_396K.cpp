// POJ 1295 - I hate SPAM, but some people love it
// Model: claude-sonnet-5
//
// A SPAM message floods a directed "friends" graph starting from its
// originator: whoever receives a given message forwards it exactly once to
// everyone in their own friend list (a person never forwards the same
// message twice). So the set of people who ever handle a message is the
// set reachable from the originator by following "i knows email of j"
// edges (BFS/DFS on a directed graph, <=20 nodes, <=100 messages).
//
// The number of messages a person T forwarded for that SPAM is the size of
// their own friend list if they were reached, and 0 if they never received
// it at all (they had nothing to forward). T is then classified against
// T1/T2 into A1/A2/A3.
//
// Ambiguity settled from the discuss board (message 29404 on this problem):
// the official sample's second test case has person 6 (Anne) unreachable
// from person 1, the originator of the first two SPAMs (no one's friend
// list contains 6), yet the expected output still gives Anne an attribute
// for those two messages -- A1 in both cases. That only matches a model
// where *every* person gets an attribute for *every* message, using T=0
// (forwarded nothing) when they were never reached, rather than a model
// that skips unreached people. Verified this reproduces both sample cases
// exactly, including that specific row.

#include <cstdio>
#include <string>
#include <vector>
#include <queue>
using namespace std;

struct Spam { int P, T1, T2; string A1, A2, A3; };

int main() {
    int N;
    while (scanf("%d", &N) == 1 && N != 0) {
        vector<vector<int> > friends_(N + 1);
        for (int i = 1; i <= N; ++i) {
            int f;
            while (scanf("%d", &f) == 1 && f != 0) {
                friends_[i].push_back(f);
            }
        }

        vector<Spam> spams;
        int P;
        while (scanf("%d", &P) == 1 && P != 0) {
            Spam s;
            s.P = P;
            char a1[25], a2[25], a3[25];
            scanf("%d %d %s %s %s", &s.T1, &s.T2, a1, a2, a3);
            s.A1 = a1; s.A2 = a2; s.A3 = a3;
            spams.push_back(s);
        }

        vector<string> names(N + 1);
        char buf[25];
        for (int i = 1; i <= N; ++i) {
            scanf("%s", buf);
            names[i] = buf;
        }

        vector<vector<string> > result(N + 1);

        for (size_t m = 0; m < spams.size(); ++m) {
            const Spam &s = spams[m];
            vector<bool> reached(N + 1, false);
            queue<int> q;
            reached[s.P] = true;
            q.push(s.P);
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (size_t k = 0; k < friends_[u].size(); ++k) {
                    int v = friends_[u][k];
                    if (!reached[v]) {
                        reached[v] = true;
                        q.push(v);
                    }
                }
            }
            for (int v = 1; v <= N; ++v) {
                int T = reached[v] ? (int)friends_[v].size() : 0;
                string attr;
                if (T < s.T1) attr = s.A1;
                else if (T < s.T2) attr = s.A2;
                else attr = s.A3;
                result[v].push_back(attr);
            }
        }

        for (int i = 1; i <= N; ++i) {
            printf("%s:", names[i].c_str());
            for (size_t k = 0; k < result[i].size(); ++k) {
                printf(" %s", result[i][k].c_str());
            }
            printf("\n");
        }
    }
    return 0;
}
