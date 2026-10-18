// POJ 2779 - Atom Bombs!? God!
// Model: gpt-6-astra
// Sweep all pairwise sight-line intersections with the guard line.
// At each event, reverse every collinear block and update its mismatches.
// Even equal-letter crossings split open intervals: occlusion is forbidden.
// Read coordinates as doubles, as recommended by the discussion board.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cmath>
#include <utility>
using namespace std;
struct Point { double x,y; char c; } p[505];
struct Event {
    double x; int a,b;
    bool operator<(const Event& e) const { return x<e.x; }
};
struct Initial {
    bool operator()(int a,int b) const {
        if(p[a].y!=p[b].y) return p[a].y>p[b].y;
        return p[a].x<p[b].x;
    }
};
int par[505],ord[505],pos[505];
int root(int a) { return par[a]==a?a:par[a]=root(par[a]); }
int main() {
    int n; char word[505];
    if(scanf("%d%s",&n,word)!=2) return 0;
    for(int i=0;i<n;++i) scanf(" %c%lf%lf",&p[i].c,&p[i].x,&p[i].y);
    vector<Event> ev;
    for(int i=0;i<n;++i) for(int j=i+1;j<n;++j) {
        if(p[i].y==p[j].y) {
            if(p[i].x==p[j].x) { printf("0\n\n"); return 0; }
        } else {
            Event e;
            e.x=(p[i].x*p[j].y-p[j].x*p[i].y)/(p[j].y-p[i].y);
            e.a=i; e.b=j; ev.push_back(e);
        }
    }
    sort(ev.begin(),ev.end());
    for(int i=0;i<n;++i) ord[i]=i;
    sort(ord,ord+n,Initial());
    int bad=0;
    for(int i=0;i<n;++i) { pos[ord[i]]=i; bad+=p[ord[i]].c!=word[i]; }
    vector<double> cuts;
    vector<pair<int,int> > ans;
    int prev=-1;
    for(size_t i=0;i<ev.size();) {
        size_t j=i+1;
        while(j<ev.size() && fabs(ev[j].x-ev[i].x)<1e-9) ++j;
        int here=(int)cuts.size(); cuts.push_back(ev[i].x);
        if(bad==0) ans.push_back(make_pair(prev,here));
        vector<int> ids;
        for(size_t k=i;k<j;++k) {
            ids.push_back(ev[k].a); ids.push_back(ev[k].b);
        }
        sort(ids.begin(),ids.end());
        ids.erase(unique(ids.begin(),ids.end()),ids.end());
        for(size_t k=0;k<ids.size();++k) par[ids[k]]=ids[k];
        for(size_t k=i;k<j;++k) par[root(ev[k].a)]=root(ev[k].b);
        vector<pair<int,int> > blocks;
        for(size_t k=0;k<ids.size();++k)
            blocks.push_back(make_pair(root(ids[k]),pos[ids[k]]));
        sort(blocks.begin(),blocks.end());
        for(size_t k=0;k<blocks.size();) {
            size_t end=k+1;
            while(end<blocks.size() && blocks[end].first==blocks[k].first) ++end;
            for(size_t t=k;t<end;++t) {
                int at=blocks[t].second; bad-=p[ord[at]].c!=word[at];
            }
            for(size_t l=k,r=end-1;l<r;++l,--r)
                swap(ord[blocks[l].second],ord[blocks[r].second]);
            for(size_t t=k;t<end;++t) {
                int at=blocks[t].second; pos[ord[at]]=at;
                bad+=p[ord[at]].c!=word[at];
            }
            k=end;
        }
        prev=here; i=j;
    }
    if(bad==0) ans.push_back(make_pair(prev,-1));
    printf("%d\n",(int)ans.size());
    for(size_t i=0;i<ans.size();++i) {
        if(i) printf(" ");
        if(ans[i].first<0) printf("*"); else printf("%.7f",cuts[ans[i].first]+0.0);
        printf(" ");
        if(ans[i].second<0) printf("*"); else printf("%.7f",cuts[ans[i].second]+0.0);
    }
    printf("\n");
    return 0;
}
