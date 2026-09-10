// POJ 2834 - Where Are You?
// Model: claude-sonnet-5
// Approach: BFS from home (node 0) to get shortest-path edge distances (each
// edge = 5 days). Jon visits his F destinations in the given order; for each
// he walks the shortest path there and immediately back (round trip 10*dist
// days). Only the exact-day arrival at a destination is a candidate meeting
// (intermediate returns to home are instantaneous pass-throughs, confirmed
// unambiguously by the problem's own discuss board: "No, he only stops at
// Home after finishing all destinations"). After his last return home he
// waits there forever, so any Cely appearance at home on or after that final
// return day also counts. If some destination is unreachable, Jon never
// completes that trip (or anything after it): only events strictly before
// that point can produce a meeting. Cely's appearances are given sorted by
// day, so scanning them in order and returning the first that matches either
// a recorded (day,place) arrival or the final-wait condition gives the
// earliest possible meeting day directly, with no need to sort Jon's events.
// Uses 64-bit arithmetic throughout because the statement gives no bound on
// Cely's appearance day d, only on M/R/F/T; POJ language 4 (old MSVC) has no
// long long, so this is submitted under G++ (language 0) instead, with
// scanf/printf %lld which G++ supports.
#include <cstdio>
#include <vector>
#include <queue>
#include <map>
#include <utility>
using namespace std;

const int MAXN = 10005;
vector<int> adj[MAXN];
long long dist_[MAXN];

int main(){
    int M;
    if(scanf("%d", &M)!=1) return 0;
    int R;
    scanf("%d", &R);
    for(int i=0;i<R;i++){
        int u,v;
        scanf("%d %d", &u, &v);
        if(u>=0 && u<M && v>=0 && v<M){
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
    }
    int F;
    scanf("%d", &F);
    vector<int> dests(F);
    for(int i=0;i<F;i++) scanf("%d", &dests[i]);
    int T;
    scanf("%d", &T);
    vector<long long> cp(T), cd(T);
    for(int i=0;i<T;i++){
        long long p,d;
        scanf("%lld %lld", &p, &d);
        cp[i]=p; cd[i]=d;
    }

    for(int i=0;i<M;i++) dist_[i] = -1;
    queue<int> q;
    dist_[0]=0;
    q.push(0);
    while(!q.empty()){
        int u=q.front(); q.pop();
        for(size_t k=0;k<adj[u].size();k++){
            int v=adj[u][k];
            if(dist_[v]==-1){
                dist_[v]=dist_[u]+1;
                q.push(v);
            }
        }
    }

    long long curTime = 0;
    bool completedAll = true;
    map<pair<long long,long long>, bool> arrivalSet;
    for(int i=0;i<F;i++){
        int dest = dests[i];
        if(dest<0 || dest>=M || dist_[dest]==-1){
            completedAll = false;
            break;
        }
        long long oneWay = 5LL * dist_[dest];
        long long arrive = curTime + oneWay;
        arrivalSet[make_pair(arrive, (long long)dest)] = true;
        curTime = arrive + oneWay;
    }
    long long finalReturn = curTime;

    long long answer = -1;
    for(int i=0;i<T;i++){
        long long p = cp[i], d = cd[i];
        bool matched = false;
        if(arrivalSet.find(make_pair(d,p)) != arrivalSet.end()) matched = true;
        if(!matched && completedAll && p==0 && d>=finalReturn) matched = true;
        if(matched){ answer = d; break; }
    }

    if(answer>=0) printf("%lld\n", answer);
    else printf("Being expected..\n");
    return 0;
}
