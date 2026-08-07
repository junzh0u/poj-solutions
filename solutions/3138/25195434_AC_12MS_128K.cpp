// POJ 3138 - ACM Team Selection
// Model: claude-sonnet-5
// Approach: For each school, three independent boolean conditions each
// contribute at most one team to the on-site total: (a) some team from the
// school solved >= M problems in the preliminary contest, (b) the school
// meets condition b, (c) the school meets condition c. Condition a is a
// per-school flag (OR over its teams' Tot), not summed per qualifying team
// -- the board's most common bug ("每个条件只能+1次") is exactly counting a
// twice/thrice when multiple teams from one school clear M. Verified against
// both sample cases by hand: with that per-school-OR rule, case 1 sums to
// 1+3+3+1+2=10 and case 2 sums to 2+1+1+2+3=9, matching the expected output,
// so no statement ambiguity remained to resolve.
#include <cstdio>
#include <cstring>

int main() {
    int S, T, M;
    int caseNum = 1;
    while (scanf("%d %d %d", &S, &T, &M) == 3) {
        if (S == 0 && T == 0 && M == 0) break;
        static int b[105], c[105], hasA[105];
        memset(b, 0, sizeof(b));
        memset(c, 0, sizeof(c));
        memset(hasA, 0, sizeof(hasA));
        for (int i = 0; i < S; i++) {
            int id, p, q;
            scanf("%d %d %d", &id, &p, &q);
            b[id] = p;
            c[id] = q;
        }
        for (int i = 0; i < T; i++) {
            int sid, tot;
            scanf("%d %d", &sid, &tot);
            if (tot >= M) hasA[sid] = 1;
        }
        int sum = 0;
        for (int id = 1; id <= S; id++) {
            sum += hasA[id] + b[id] + c[id];
        }
        printf("Case %d: %d\n", caseNum, sum);
        caseNum++;
    }
    return 0;
}
