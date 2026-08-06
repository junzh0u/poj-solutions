// POJ 3692 - Kindergarten
//
// Every pair of girls knows each other and every pair of boys knows each
// other, so a group in which everyone knows everyone is exactly a set of
// girls S plus a set of boys T such that every girl in S knows every boy
// in T -- the only constraint is on the cross pairs.
//
// Build the COMPLEMENT bipartite graph: an edge (g, b) whenever girl g and
// boy b do NOT know each other. Then S u T is a valid group iff it spans no
// complement edge, i.e. iff it is an independent set of that graph, and the
// answer is the maximum independent set. The complement graph is bipartite,
// so by Koenig's theorem
//     max independent set = |V| - max matching = (G + B) - max matching,
// and the matching is found with Kuhn's augmenting-path algorithm.
//
// Notes on the statement:
//  - It never says the group must contain both a girl and a boy, and the
//    all-girls / all-boys groups fall out of the independent-set formulation
//    for free (with M = 0 the answer is max(G, B)).
//  - Input is multiple test cases terminated by "0 0 0"; output is one line
//    per case, "Case <k>: <answer>", numbered from 1.
//
// G, B <= 200, so the complement has at most 40000 edges and Kuhn runs in
// O(G * G * B) per case -- 100 worst cases finish in well under a second.

#include <cstdio>
#include <cstring>

const int MAXN = 205;

int G, B, M;
bool know[MAXN][MAXN];   /* know[g][b] */
bool comp[MAXN][MAXN];   /* complement: g and b do NOT know each other */
int matchB[MAXN];        /* boy -> girl */
bool used[MAXN];

bool tryKuhn(int g) {
    for (int b = 1; b <= B; ++b) {
        if (!comp[g][b] || used[b]) continue;
        used[b] = true;
        if (matchB[b] == 0 || tryKuhn(matchB[b])) {
            matchB[b] = g;
            return true;
        }
    }
    return false;
}

int main() {
    int cas = 0;
    while (scanf("%d %d %d", &G, &B, &M) == 3) {
        if (G == 0 && B == 0 && M == 0) break;
        for (int i = 1; i <= G; ++i)
            for (int j = 1; j <= B; ++j)
                know[i][j] = false;
        for (int k = 0; k < M; ++k) {
            int x, y;
            scanf("%d %d", &x, &y);
            if (x >= 1 && x <= G && y >= 1 && y <= B) know[x][y] = true;
        }
        for (int i = 1; i <= G; ++i)
            for (int j = 1; j <= B; ++j)
                comp[i][j] = !know[i][j];

        for (int j = 0; j <= B; ++j) matchB[j] = 0;
        int matching = 0;
        for (int i = 1; i <= G; ++i) {
            memset(used, 0, sizeof(bool) * (B + 1));
            if (tryKuhn(i)) ++matching;
        }
        printf("Case %d: %d\n", ++cas, G + B - matching);
    }
    return 0;
}
