// POJ 2827 - Auto-Calculation Machine
// Model: claude-sonnet-5
//
// Each record sum(i,j)=v gives, with L=min(i,j), R=max(i,j) and prefix
// sums S[0..], the constraint S[R]-S[L-1]=v. Maintain a weighted
// (potential) union-find over the distinct coordinates L-1 and R
// (compressed via a map, since they range up to 1e9). For a new record,
// if the two endpoints are already in the same set, the recorded v must
// equal the value implied by the stored potentials, otherwise it's a
// bug and the correct value is the implied one; otherwise merge the
// sets consistently with v and accept.
//
// Board pitfall (message 48406/48449): the data is NOT guaranteed
// i<=j, despite what one might assume from the operation name; must
// take L=min(i,j), R=max(i,j) explicitly.

#include <cstdio>
#include <map>

const int MAXN = 20005;
int par[MAXN];
long long diff_[MAXN]; // value[x] - value[root]
std::map<int,int> idOf;
int nodeCnt = 0;

int getId(int coord) {
    std::map<int,int>::iterator it = idOf.find(coord);
    if (it != idOf.end()) return it->second;
    int id = nodeCnt++;
    idOf[coord] = id;
    par[id] = id;
    diff_[id] = 0;
    return id;
}

int find(int x) {
    if (par[x] == x) return x;
    int r = find(par[x]);
    diff_[x] += diff_[par[x]];
    par[x] = r;
    return r;
}

int main() {
    int m;
    while (scanf("%d", &m) == 1) {
        idOf.clear();
        nodeCnt = 0;
        for (int k = 0; k < m; k++) {
            int i, j;
            long long v;
            scanf("%d %d %lld", &i, &j, &v);
            int L = i < j ? i : j;
            int R = i < j ? j : i;
            int a = getId(L - 1);
            int b = getId(R);
            int ra = find(a);
            int rb = find(b);
            if (ra == rb) {
                long long correct = diff_[b] - diff_[a];
                if (correct == v) {
                    printf("Accept\n");
                } else {
                    printf("Bug Detected %lld\n", correct);
                }
            } else {
                diff_[ra] = diff_[b] - diff_[a] - v;
                par[ra] = rb;
                printf("Accept\n");
            }
        }
    }
    return 0;
}
