// POJ 2670 - The Sorcerer's Stone
// Model: claude-sonnet-5
//
// Each chest's contained stone (R) is produced by exactly that chest and no
// other (the statement guarantees stones in different chests are distinct),
// so the "recipe" for any stone is unique.  To open the target chest (the
// one containing "Sorcerer's Stone") we need every one of its required
// stones; each required stone is either already owned, or must come from
// its unique producing chest, whose own requirements recurse the same way.
// Because there is no choice of producer, the minimal set of chests that
// must be opened is exactly the set reachable by this dependency closure
// (no smarter selection could do better, and none is worse since the
// producer is forced) -- so a DFS over the dependency DAG with cycle
// detection (a chest currently being resolved that is required again means
// deadlock -> impossible) gives the answer directly, no search needed.
//
// Ambiguity handled per the discuss board: trailing spaces can appear at
// the end of input lines, so every line is right-trimmed (also strips a
// possible trailing '\r') before parsing/comparison.

#include <cstdio>
#include <string>
#include <vector>
#include <set>
#include <map>
using namespace std;

static string rtrim(const string &s) {
    size_t end = s.size();
    while (end > 0) {
        char c = s[end - 1];
        if (c == '\r' || c == '\n' || c == ' ' || c == '\t') --end;
        else break;
    }
    return s.substr(0, end);
}

static bool readLine(string &out) {
    // read a line via getchar to be robust; returns false on EOF with nothing read
    out.clear();
    int c = getchar();
    if (c == EOF) return false;
    while (c != EOF && c != '\n') {
        out += (char)c;
        c = getchar();
    }
    out = rtrim(out);
    return true;
}

int main() {
    int N, M;
    while (scanf("%d %d", &N, &M) == 2) {
        if (N == 0 && M == 0) break;
        // consume rest of the line after the two integers
        {
            string dummy;
            readLine(dummy);
        }

        set<string> have;
        for (int i = 0; i < N; ++i) {
            string s;
            readLine(s);
            have.insert(s);
        }

        vector<vector<string> > inputs(M);
        vector<string> output(M);
        map<string, int> producer;
        int targetIdx = -1;

        for (int i = 0; i < M; ++i) {
            string line;
            readLine(line);
            size_t colonPos = line.rfind(": ");
            string left = line.substr(0, colonPos);
            string R = line.substr(colonPos + 2);
            output[i] = R;
            producer[R] = i;
            if (R == "Sorcerer's Stone") targetIdx = i;

            vector<string> &parts = inputs[i];
            size_t pos = 0;
            while (true) {
                size_t comma = left.find(", ", pos);
                if (comma == string::npos) {
                    parts.push_back(left.substr(pos));
                    break;
                }
                parts.push_back(left.substr(pos, comma - pos));
                pos = comma + 2;
            }
        }

        // state: 0 = unvisited, 1 = in progress (on recursion stack), 2 = opened
        vector<int> state(M, 0);
        int countOpened = 0;
        bool possible = true;

        // iterative DFS to avoid recursion-depth issues on adversarial cycles
        // implemented via explicit stack since M <= 100 recursion would be fine,
        // but keep it simple and safe with an explicit function using recursion
        // (M <= 100 so recursion depth is bounded by M, safe).
        struct Resolver {
            vector<vector<string> > &inputs;
            vector<int> &state;
            map<string, int> &producer;
            set<string> &have;
            int &countOpened;
            bool ok;

            Resolver(vector<vector<string> > &in, vector<int> &st, map<string,int> &pr, set<string> &hv, int &co)
                : inputs(in), state(st), producer(pr), have(hv), countOpened(co), ok(true) {}

            bool resolve(int idx) {
                if (state[idx] == 2) return true;
                if (state[idx] == 1) return false; // cycle -> deadlock
                state[idx] = 1;
                for (size_t k = 0; k < inputs[idx].size(); ++k) {
                    const string &s = inputs[idx][k];
                    if (have.count(s)) continue;
                    map<string,int>::iterator it = producer.find(s);
                    if (it == producer.end()) {
                        return false; // no way to obtain this stone
                    }
                    if (!resolve(it->second)) return false;
                }
                state[idx] = 2;
                ++countOpened;
                return true;
            }
        };

        if (targetIdx == -1) {
            // guaranteed by statement not to happen, but guard anyway
            possible = false;
        } else {
            Resolver r(inputs, state, producer, have, countOpened);
            possible = r.resolve(targetIdx);
        }

        if (possible) printf("%d\n", countOpened);
        else printf("-1\n");
    }
    return 0;
}
