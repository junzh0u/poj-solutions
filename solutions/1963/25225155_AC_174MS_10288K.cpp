// POJ 1963 - Cave Exploration
// Model: gpt-6.1-sol
// Split corridors at intersections, endpoints and the entry point, then
// follow the left/straight/right/back rule on the resulting plane graph.
// Merely touching an intersection visits both corridors; stop on the first
// return to the entry, even when it is reached with the opposite heading.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;
struct Segment { char t; int c,a,b; vector<pair<int,int> > p; };
struct Node { int next[4],h,v; Node(int H,int V):h(H),v(V) { for(int i=0;i<4;++i) next[i]=-1; } };
vector<Node> nodes;
int add(int h,int v) { nodes.push_back(Node(h,v)); return (int)nodes.size()-1; }
int point(Segment &s,int pos,int id) {
    for(unsigned j=0;j<s.p.size();++j) if(s.p[j].first==pos) return s.p[j].second;
    int u=add(s.t=='H'?id:-1,s.t=='V'?id:-1);
    s.p.push_back(make_pair(pos,u)); return u;
}
int main() {
    int T; scanf("%d",&T);
    while(T--) {
        int n; scanf("%d",&n); vector<Segment> s(n); nodes.clear(); nodes.reserve(252001);
        for(int i=0;i<n;++i) { scanf(" %c%d%d%d",&s[i].t,&s[i].c,&s[i].a,&s[i].b); if(s[i].a>s[i].b) swap(s[i].a,s[i].b); }
        for(int i=0;i<n;++i) if(s[i].t=='H') for(int j=0;j<n;++j) if(s[j].t=='V') {
            if(s[i].a<=s[j].c && s[j].c<=s[i].b && s[j].a<=s[i].c && s[i].c<=s[j].b) {
                int u=add(i,j); s[i].p.push_back(make_pair(s[j].c,u)); s[j].p.push_back(make_pair(s[i].c,u));
            }
        }
        for(int i=0;i<n;++i) { point(s[i],s[i].a,i); point(s[i],s[i].b,i); }
        int x,y; char ch; scanf("%d%d %c",&x,&y,&ch); int start=-1;
        for(int i=0;i<n;++i) { int pos=s[i].t=='H'?x:y, fixed=s[i].t=='H'?y:x;
            if(fixed==s[i].c && s[i].a<=pos && pos<=s[i].b) start=point(s[i],pos,i);
        }
        for(int i=0;i<n;++i) { sort(s[i].p.begin(),s[i].p.end()); int d=s[i].t=='H'?0:1;
            for(unsigned j=1;j<s[i].p.size();++j) { int a=s[i].p[j-1].second,b=s[i].p[j].second; nodes[a].next[d]=b; nodes[b].next[d+2]=a; }
        }
        vector<int> seen(n,0); int u=start,d=ch=='E'?0:ch=='N'?1:ch=='W'?2:3;
        do {
            if(nodes[u].h!=-1) seen[nodes[u].h]=1;
            if(nodes[u].v!=-1) seen[nodes[u].v]=1;
            u=nodes[u].next[d]; if(u==start) break;
            int choices[4]={(d+1)%4,d,(d+3)%4,(d+2)%4};
            for(int k=0;k<4;++k) if(nodes[u].next[choices[k]]!=-1) { d=choices[k]; break; }
        } while(true);
        int ans=0; for(int i=0;i<n;++i) ans+=!seen[i]; printf("%d\n",ans);
    }
    return 0;
}
