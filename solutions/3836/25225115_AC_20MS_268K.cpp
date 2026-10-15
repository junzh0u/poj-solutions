// POJ 3836 - P2P File Sharing System
// Model: gpt-6.1-sol
// Simulate integer seconds. All downloads use the servers present at the
// start of the second, then completed clients become servers together.
// Online intervals are half-open; interrupted downloads retain their data.
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
int speed[20][20], dataSize[20], startTime[20], nextSize[20];
bool online[20][1001];
int main() {
    int cases; scanf("%d", &cases);
    while (cases--) {
        int n, T, k, S; scanf("%d%d%d%d", &n, &T, &k, &S);
        memset(dataSize, 0, sizeof(dataSize));
        memset(online, 0, sizeof(online));
        fill(startTime, startTime + n, T + 1);
        for (int i=0, id; i<k; ++i) { scanf("%d", &id); dataSize[id-1]=S; }
        for (int i=0; i<n; ++i) for (int j=0; j<n; ++j) scanf("%d", &speed[i][j]);
        for (int i=0; i<n; ++i) {
            int count; scanf("%d", &count);
            while (count--) {
                int a,b; scanf("%d%d", &a,&b);
                for (int t=a; t<min(b,T); ++t) online[i][t]=true;
            }
        }
        int m; scanf("%d", &m);
        while (m--) { int t,id; scanf("%d%d", &t,&id); --id; startTime[id]=min(startTime[id],t); }
        for (int t=0; t<T; ++t) {
            for (int i=0; i<n; ++i) {
                nextSize[i]=dataSize[i];
                if (dataSize[i]==S || !online[i][t] || startTime[i]>t) continue;
                for (int j=0; j<n; ++j)
                    if (dataSize[j]==S && online[j][t]) nextSize[i]+=speed[i][j];
                nextSize[i]=min(nextSize[i],S);
            }
            copy(nextSize,nextSize+n,dataSize);
        }
        for (int i=0; i<n; ++i) printf("%d%%\n",dataSize[i]*100/S);
    }
    return 0;
}
