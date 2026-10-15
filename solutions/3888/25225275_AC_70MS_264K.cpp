// POJ 3888 - Dimensional Warp Drive
// Model: gpt-6.1-sol
// Sort trajectories by discovery year and maintain a row-echelon basis over
// the field modulo 11. Repeated use permits every field coefficient, so the
// destination is reachable exactly when its displacement lies in this span.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
struct Warp { int v[11], year; };
bool earlier(const Warp &a, const Warp &b) { return a.year < b.year; }
int basis[11][11], inv[11];
void insertVector(int *v) {
    for (int i=0;i<11;i++) if (v[i]) {
        if (!basis[i][i]) {
            int m=inv[v[i]];
            for (int j=i;j<11;j++) basis[i][j]=v[j]*m%11;
            return;
        }
        int m=v[i];
        for (int j=i;j<11;j++) v[j]=(v[j]-m*basis[i][j]%11+11)%11;
    }
}
bool reachable(const int *target) {
    int v[11]; memcpy(v,target,sizeof(v));
    for (int i=0;i<11;i++) if (v[i]) {
        if (!basis[i][i]) return false;
        int m=v[i];
        for (int j=i;j<11;j++) v[j]=(v[j]-m*basis[i][j]%11+11)%11;
    }
    return true;
}
int main() {
    for (int a=1;a<11;a++) for (int b=1;b<11;b++) if (a*b%11==1) inv[a]=b;
    int tests; scanf("%d",&tests);
    while (tests--) {
        int n,start[11],target[11]; Warp w[1000];
        scanf("%d",&n);
        for (int i=0;i<11;i++) scanf("%d",&start[i]);
        for (int i=0;i<11;i++) { scanf("%d",&target[i]); target[i]=(target[i]-start[i]+11)%11; }
        for (int i=0;i<n;i++) { for (int j=0;j<11;j++) scanf("%d",&w[i].v[j]); scanf("%d",&w[i].year); }
        sort(w,w+n,earlier); memset(basis,0,sizeof(basis));
        int answer=-1;
        for (int i=0;i<n;i++) {
            insertVector(w[i].v);
            if (reachable(target)) { answer=w[i].year; break; }
        }
        if (answer<0) puts("unreachable"); else printf("%d\n",answer);
    }
    return 0;
}
