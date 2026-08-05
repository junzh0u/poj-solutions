// POJ 1324 - Holedox Moving
// BFS over states (head position, body shape). The body shape is encoded as
// L-1 relative directions (2 bits each: from segment i to segment i+1),
// which keeps the state space to n*m*4^(L-1) <= 20*20*4^7 ~= 6.55M states
// instead of blowing up with absolute coordinates for every segment.
//
// Ambiguity: the statement says a move target must be "neither a stone nor
// occupied by its body" -- read literally at the pre-move instant, this
// forbids moving the head into the square the tail currently sits on, even
// though the tail vacates that square in the same step. Sample case 1 (a
// body that forms a closed 2x2 loop) only matches the official answer (9)
// under this strict reading; allowing the tail-vacate shortcut finds a
// shorter (invalid) path of 3.
#include <cstdio>
#include <cstring>
#include <vector>
using namespace std;

static const int DR[4] = {-1, 1, 0, 0};
static const int DC[4] = {0, 0, -1, 1};
// direction codes: 0=Up 1=Down 2=Left 3=Right; opposite = code ^ 1

int n, m, L;
bool stoneMap[20][20];
int br[8], bc[8];

int main() {
    int caseNo = 0;
    while (scanf("%d %d %d", &n, &m, &L) == 3) {
        if (n == 0 && m == 0 && L == 0) break;
        ++caseNo;
        for (int i = 0; i < L; ++i) {
            int r, c;
            scanf("%d %d", &r, &c);
            br[i] = r - 1;
            bc[i] = c - 1;
        }
        memset(stoneMap, 0, sizeof(stoneMap));
        int K;
        scanf("%d", &K);
        for (int i = 0; i < K; ++i) {
            int r, c;
            scanf("%d %d", &r, &c);
            stoneMap[r - 1][c - 1] = true;
        }

        int shapeBits = 2 * (L - 1);
        int shapeCount = 1 << shapeBits;
        int mask = shapeCount - 1;
        int total = n * m * shapeCount;

        // initial shape code: dir[i] = direction from B_i to B_{i+1}
        int shapeCode = 0;
        for (int i = 0; i < L - 1; ++i) {
            int dr = br[i + 1] - br[i];
            int dc = bc[i + 1] - bc[i];
            int code = -1;
            for (int d = 0; d < 4; ++d) {
                if (DR[d] == dr && DC[d] == dc) { code = d; break; }
            }
            shapeCode |= (code << (2 * i));
        }

        int startIdx = (br[0] * m + bc[0]) * shapeCount + shapeCode;

        vector<char> visited(total, 0);
        vector<int> q;
        q.reserve(total);
        q.push_back(startIdx);
        visited[startIdx] = 1;

        int answer = -1;
        int dist = 0;
        size_t qi = 0;
        int pos_r[8], pos_c[8];

        while (qi < q.size()) {
            size_t levelEnd = q.size();
            bool found = false;
            for (; qi < levelEnd; ++qi) {
                int idx = q[qi];
                int sc = idx % shapeCount;
                int rc = idx / shapeCount;
                int c = rc % m;
                int r = rc / m;
                if (r == 0 && c == 0) { answer = dist; found = true; break; }

                pos_r[0] = r; pos_c[0] = c;
                for (int i = 0; i < L - 1; ++i) {
                    int code = (sc >> (2 * i)) & 3;
                    pos_r[i + 1] = pos_r[i] + DR[code];
                    pos_c[i + 1] = pos_c[i] + DC[code];
                }

                for (int d = 0; d < 4; ++d) {
                    int nr = r + DR[d];
                    int nc = c + DC[d];
                    if (nr < 0 || nr >= n || nc < 0 || nc >= m) continue;
                    if (stoneMap[nr][nc]) continue;
                    // Includes the tail (i up to L, not L-1): see ambiguity note above.
                    bool collide = false;
                    for (int i = 0; i < L; ++i) {
                        if (pos_r[i] == nr && pos_c[i] == nc) { collide = true; break; }
                    }
                    if (collide) continue;
                    int opp = d ^ 1;
                    int newShape = ((sc << 2) | opp) & mask;
                    int newIdx = (nr * m + nc) * shapeCount + newShape;
                    if (!visited[newIdx]) {
                        visited[newIdx] = 1;
                        q.push_back(newIdx);
                    }
                }
            }
            if (found) break;
            ++dist;
        }

        printf("Case %d: %d\n", caseNo, answer);
    }
    return 0;
}
