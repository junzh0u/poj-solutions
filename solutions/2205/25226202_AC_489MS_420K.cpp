// POJ 2205 - Self-Replicating Numbers
// Model: gpt-6-astra
// Independent control: zhangxiao1124, POJ discussion 93126 (2008).
// Original DFS enumerates digits and checks the full square column.
// Added cstdio for modern compiler declarations; algorithm unchanged.
#include <cstdio>
#include<iostream>
using namespace std;
#define maxn 2005
int k,n,m,a[maxn],ans[maxn][maxn];

void Dfs(int dep,int sum) {
    if (dep>n) {
        if (a[n] || n==1) {
            m++;
            for (int i=1; i<=n; i++) ans[m][i]=a[n+1-i];
        }
        return;
    }
    for (a[dep]=0; a[dep]<k; a[dep]++) {
        int tot=0;
        for (int i=1; i<=dep; i++) {
            int j=dep+1-i;
            tot+=a[i]*a[j];
        }
        if ((tot+sum)%k==a[dep]) Dfs(dep+1,(tot+sum)/k);
    }
}

inline char Change(int x) {
    if (x<=9) return '0'+x;
    return 'A'+x-10;
}

void Print() {
    printf("%d\n",m);
    for (int i=1; i<=m; i++) {
        for (int j=1; j<=n; j++) printf("%c",Change(ans[i][j]));
        printf("\n");
    }
}

int main() {
    scanf("%d%d",&k,&n);
    Dfs(1,0);
    Print();
    return 0;
}
