// POJ 1756 - Domino Puzzle
// Model: gpt-6-astra
// Dijkstra over connected-component partitions and degree parity masks.
// Each transition adds one domino; a connected graph with zero or two odd
// vertices has an Euler trail. Reconstruct the minimum-cost added pieces.
// Unused digits need not be introduced: remove unused trail endpoints and
// shortcut internal occurrences, strictly reducing the sum of their digits.
#include <cstdio>
#include <vector>
#include <map>
#include <queue>
#include <functional>
#include <utility>
using namespace std;
struct Record { int d, prev, a, b; Record(int D=1000000,int P=-1,int A=0,int B=0):d(D),prev(P),a(A),b(B){} };
int mergepart(int p,int a,int b) {
    int x=(p>>(3*a))&7, y=(p>>(3*b))&7;
    if(x==y)return p;
    int lo=x<y?x:y, hi=x<y?y:x, q=0;
    for(int i=0;i<6;++i){int z=(p>>(3*i))&7;if(z==hi)z=lo;q|=z<<(3*i);}
    return q;
}
int main(){
    int n;if(scanf("%d",&n)!=1)return 0;
    int p=0, odd=0, active=0;
    for(int i=0;i<6;++i)p|=i<<(3*i);
    for(int i=0;i<n;++i){int a,b;scanf("%d%d",&a,&b);--a;--b;active|=(1<<a)|(1<<b);odd^=(1<<a)^(1<<b);p=mergepart(p,a,b);}
    int start=(p<<6)|odd, goal=-1;
    map<int,Record> rec;rec[start]=Record(0);
    priority_queue<pair<int,int>,vector<pair<int,int> >,greater<pair<int,int> > > pq;
    pq.push(make_pair(0,start));
    while(!pq.empty()){
        int d=pq.top().first,s=pq.top().second;pq.pop();
        if(rec[s].d!=d)continue;
        p=s>>6;odd=s&63;int cnt=0,label=-1;bool connected=true;
        for(int i=0;i<6;++i){if(odd&(1<<i))++cnt;if(active&(1<<i)){int z=(p>>(3*i))&7;if(label<0)label=z;else if(z!=label)connected=false;}}
        if(connected&&cnt<=2){goal=s;break;}
        for(int a=0;a<6;++a)if(active&(1<<a))for(int b=a+1;b<6;++b)if(active&(1<<b)){
            int t=(mergepart(p,a,b)<<6)|(odd^(1<<a)^(1<<b));
            int nd=d+a+b+2;
            map<int,Record>::iterator it=rec.find(t);
            if(it==rec.end()||nd<it->second.d){rec[t]=Record(nd,s,a+1,b+1);pq.push(make_pair(nd,t));}
        }
    }
    vector<pair<int,int> > ans;
    for(int s=goal;s!=start;s=rec[s].prev)ans.push_back(make_pair(rec[s].a,rec[s].b));
    printf("%d\n%d\n",rec[goal].d,(int)ans.size());
    for(int i=(int)ans.size()-1;i>=0;--i)printf("%d %d\n",ans[i].first,ans[i].second);
    return 0;
}
