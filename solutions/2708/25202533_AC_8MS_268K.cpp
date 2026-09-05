// POJ 2708 - Time to Graduate
// Model: claude-sonnet-5
// Approach: BFS over states (bitmask of completed courses, semester parity).
// Semester type alternates deterministically starting with Fall (semester 1).
// From a state, compute the set of courses available this semester (not yet
// taken, offered this semester or 'B', all prerequisites already completed).
// Taking more courses can only help (monotone: completing more courses only
// enlarges future availability), so it is always weakly optimal to take
// exactly min(m, |available|) courses each semester; when |available| > m we
// must branch over every size-m subset (enumerated via Gosper's hack) since
// which courses are chosen affects what becomes available later. BFS over
// (mask, parity) states (parity determines whether the *next* semester is
// Fall or Spring) finds the minimum number of semesters to reach the full
// mask; first time full mask is reached, that level is the answer.
// No statement ambiguity found; board confirms this exact approach
// (enumerate all m-subsets when availability exceeds the cap).
#include <cstdio>
#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <queue>
using namespace std;

int n, m;
int prereqMask[12];
char semType[12]; // 'F', 'S', or 'B'

int popcount(unsigned x) {
    int c = 0;
    while (x) { c += x & 1; x >>= 1; }
    return c;
}

int main() {
    while (true) {
        if (scanf("%d %d", &n, &m) != 2) break;
        if (n == -1 && m == -1) break;

        map<string, int> idIndex;
        vector<string> ids(n);
        for (int i = 0; i < n; ++i) {
            char buf[16];
            scanf("%s", buf);
            ids[i] = buf;
            idIndex[ids[i]] = i;
        }

        for (int i = 0; i < n; ++i) {
            char idbuf[16], sembuf[8];
            int p;
            scanf("%s %s %d", idbuf, sembuf, &p);
            int idx = idIndex[idbuf];
            semType[idx] = sembuf[0];
            int mask = 0;
            for (int j = 0; j < p; ++j) {
                char prebuf[16];
                scanf("%s", prebuf);
                mask |= (1 << idIndex[prebuf]);
            }
            prereqMask[idx] = mask;
        }

        int fullMask = (1 << n) - 1;

        // visited[mask][parity] = true if reached
        static bool visited[4096][2];
        memset(visited, 0, sizeof(visited));

        // queue holds (mask, parity, level)
        queue<int> qmask, qpar, qlev;
        visited[0][0] = true;
        qmask.push(0); qpar.push(0); qlev.push(0);

        int answer = -1;

        if (fullMask == 0) {
            answer = 0;
        }

        while (!qmask.empty() && answer == -1) {
            int mask = qmask.front(); qmask.pop();
            int par = qpar.front(); qpar.pop();
            int lev = qlev.front(); qlev.pop();

            // parity 0 means the *next* semester (lev+1) is Fall, parity 1 means Spring.
            char want = (par == 0) ? 'F' : 'S';

            // Compute available courses this semester.
            vector<int> avail;
            for (int i = 0; i < n; ++i) {
                if (mask & (1 << i)) continue;
                if (semType[i] != 'B' && semType[i] != want) continue;
                if ((prereqMask[i] & mask) != prereqMask[i]) continue;
                avail.push_back(i);
            }

            int s = (int)avail.size();
            int k = s < m ? s : m;
            int newPar = 1 - par;
            int newLev = lev + 1;

            if (k == 0) {
                int newMask = mask;
                if (newMask == fullMask) { answer = newLev; break; }
                if (!visited[newMask][newPar]) {
                    visited[newMask][newPar] = true;
                    qmask.push(newMask); qpar.push(newPar); qlev.push(newLev);
                }
            } else {
                // enumerate all k-subsets of avail (0..s-1) via Gosper's hack
                unsigned x = (1u << k) - 1;
                unsigned limit = (1u << s);
                while (x < limit) {
                    int bits = 0;
                    for (int j = 0; j < s; ++j) {
                        if (x & (1u << j)) bits |= (1 << avail[j]);
                    }
                    int newMask = mask | bits;
                    if (newMask == fullMask) { answer = newLev; break; }
                    if (!visited[newMask][newPar]) {
                        visited[newMask][newPar] = true;
                        qmask.push(newMask); qpar.push(newPar); qlev.push(newLev);
                    }
                    // Gosper's hack: next combination
                    unsigned c = x & (-(int)x);
                    unsigned r = x + c;
                    x = (((x ^ r) >> 2) / c) | r;
                }
            }
        }

        printf("The minimum number of semesters required to graduate is %d.\n", answer);
    }
    return 0;
}
