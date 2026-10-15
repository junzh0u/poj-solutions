// POJ 2093 - Cutting Edge
// Model: gpt-6.1-sol
// Keep the first legal cut of every current pane in a global min-heap.
// Sorted coordinate projections reveal boundaries crossed by no rectangle.
// The ordering applies globally across all panes, not recursively per child.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
struct Rect { int x1,y1,x2,y2; };
Rect r[2005];
struct Order {
    int axis;
    Order(int a):axis(a) {}
    bool operator()(int a,int b) const {
        return axis ? r[a].y1<r[b].y1 : r[a].x1<r[b].x1;
    }
};
struct Pane {
    vector<int> ids;
    int x1,y1,x2,y2,axis,at,cx,cy;
    bool operator<(const Pane &b) const {
        return cx!=b.cx ? cx>b.cx : cy>b.cy;
    }
};
priority_queue<Pane> q;
void add(vector<int> ids) {
    if(ids.size()<2) return;
    Pane p;
    p.ids=ids;
    p.x1=p.y1=10001; p.x2=p.y2=-10001;
    for(unsigned i=0;i<ids.size();++i) {
        Rect a=r[ids[i]];
        p.x1=min(p.x1,a.x1); p.y1=min(p.y1,a.y1);
        p.x2=max(p.x2,a.x2); p.y2=max(p.y2,a.y2);
    }
    // Every horizontal cut starts left of every vertical cut of this pane.
    for(int axis=1;axis>=0;--axis) {
        sort(ids.begin(),ids.end(),Order(axis));
        int end=axis?r[ids[0]].y2:r[ids[0]].x2;
        for(unsigned i=1;i<ids.size();++i) {
            int start=axis?r[ids[i]].y1:r[ids[i]].x1;
            if(end==start) {
                p.axis=axis; p.at=start;
                p.cx=axis?p.x1:start; p.cy=axis?start:p.y1;
                q.push(p); return;
            }
            end=max(end,axis?r[ids[i]].y2:r[ids[i]].x2);
        }
    }
}
int main() {
    int n;
    while(scanf("%d",&n)==1 && n) {
        vector<int> ids;
        for(int i=0;i<n;++i) {
            scanf("%d%d%d%d",&r[i].x1,&r[i].y1,&r[i].x2,&r[i].y2);
            ids.push_back(i);
        }
        add(ids);
        while(!q.empty()) {
            Pane p=q.top(); q.pop();
            if(p.axis) printf("%d %d %d %d\n",p.x1,p.at,p.x2,p.at);
            else printf("%d %d %d %d\n",p.at,p.y1,p.at,p.y2);
            vector<int> a,b;
            for(unsigned i=0;i<p.ids.size();++i) {
                int id=p.ids[i];
                if((p.axis?r[id].y2:r[id].x2)<=p.at) a.push_back(id);
                else b.push_back(id);
            }
            add(a); add(b);
        }
        puts("");
    }
    return 0;
}
