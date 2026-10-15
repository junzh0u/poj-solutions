// POJ 1803 - Box Art
// Model: gpt-6.1-sol
// Clip every box to the initial box, then sweep distinct x boundaries.
// In each slab, sweep y events and maintain covered z length in a segment tree.
// Partly intersecting boxes contribute their intersection, not their full volume.
// Coordinates and the total volume (at most 1000^3) fit in signed int.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;
struct Box { int a,b,c,d,e,f; };
struct Event {
    int y,z1,z2,delta,id;
    bool operator<(const Event& o) const { return y<o.y; }
};
int cover[4096],len[4096];
void update(int p,int l,int r,int a,int b,int v) {
    if(a<=l && r<=b) cover[p]+=v;
    else {
        int m=(l+r)/2;
        if(a<m) update(p*2,l,m,a,b,v);
        if(b>m) update(p*2+1,m,r,a,b,v);
    }
    if(cover[p]) len[p]=r-l;
    else if(r-l==1) len[p]=0;
    else len[p]=len[p*2]+len[p*2+1];
}
int main() {
    int t; if(scanf("%d",&t)!=1) return 0;
    for(int cs=1;cs<=t;++cs) {
        Box base; scanf("%d%d%d%d%d%d",&base.a,&base.b,&base.c,&base.d,&base.e,&base.f);
        int n; scanf("%d",&n);
        vector<Box> boxes; vector<int> xs; vector<Event> events;
        for(int i=0;i<n;++i) {
            Box q; scanf("%d%d%d%d%d%d",&q.a,&q.b,&q.c,&q.d,&q.e,&q.f);
            q.a=max(q.a,base.a); q.b=max(q.b,base.b); q.c=max(q.c,base.c);
            q.d=min(q.d,base.d); q.e=min(q.e,base.e); q.f=min(q.f,base.f);
            if(q.a>=q.d || q.b>=q.e || q.c>=q.f) continue;
            int id=boxes.size(); boxes.push_back(q); xs.push_back(q.a); xs.push_back(q.d);
            Event ev; ev.z1=q.c; ev.z2=q.f; ev.id=id;
            ev.y=q.b; ev.delta=1; events.push_back(ev);
            ev.y=q.e; ev.delta=-1; events.push_back(ev);
        }
        sort(xs.begin(),xs.end()); xs.erase(unique(xs.begin(),xs.end()),xs.end());
        sort(events.begin(),events.end());
        int volume=0;
        for(int k=0;k+1<(int)xs.size();++k) {
            memset(cover,0,sizeof(cover)); memset(len,0,sizeof(len));
            int area=0,prev=0;
            for(int j=0;j<(int)events.size();++j) {
                const Event& e=events[j]; const Box& b=boxes[e.id];
                if(b.a>xs[k] || b.d<=xs[k]) continue;
                area+=len[1]*(e.y-prev); prev=e.y;
                update(1,0,1000,e.z1,e.z2,e.delta);
            }
            volume+=area*(xs[k+1]-xs[k]);
        }
        printf("Scenario #%d:\n%d\n\n",cs,volume);
    }
    return 0;
}
