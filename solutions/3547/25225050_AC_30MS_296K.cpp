// POJ 3547 - Cube evolvent
// Model: gpt-6.1-sol
// Enumerate all translated hexominoes, then fold their local coordinate frames.
// Exactly six distinct face normals characterize a cube net. Enumerating fixed
// shapes includes both reflections and rotations; each square is one grid cell.
#include <cstdio>
#include <vector>
#include <set>
#include <algorithm>
#include <utility>
using namespace std;
typedef pair<int,int> P;
typedef vector<P> Shape;
int dx[4]={1,-1,0,0},dy[4]={0,0,1,-1};
Shape normalize(Shape s) {
    int x=100,y=100;
    for(int i=0;i<(int)s.size();++i) {x=min(x,s[i].first); y=min(y,s[i].second);}
    for(int i=0;i<(int)s.size();++i) {s[i].first-=x;s[i].second-=y;}
    sort(s.begin(),s.end()); return s;
}
struct V {int x,y,z; V(int a=0,int b=0,int c=0):x(a),y(b),z(c){} };
V neg(V a){return V(-a.x,-a.y,-a.z);}
bool eq(V a,V b){return a.x==b.x && a.y==b.y && a.z==b.z;}
struct Frame {V u,v,n;};
Frame turn(Frame a,int d) {
    Frame b=a;
    if(d==0){b.u=neg(a.n);b.n=a.u;}
    if(d==1){b.u=a.n;b.n=neg(a.u);}
    if(d==2){b.v=neg(a.n);b.n=a.v;}
    if(d==3){b.v=a.n;b.n=neg(a.v);}
    return b;
}
bool net(const Shape &s) {
    Frame f[6]; bool seen[6]={true,false,false,false,false,false};
    f[0].u=V(1,0,0);f[0].v=V(0,1,0);f[0].n=V(0,0,1);
    int q[6],l=0,r=1;q[0]=0;
    while(l<r){int i=q[l++];
        for(int d=0;d<4;++d)for(int j=0;j<6;++j)
            if(s[j]==P(s[i].first+dx[d],s[i].second+dy[d])){
                Frame b=turn(f[i],d);
                if(seen[j]){if(!eq(f[j].u,b.u)||!eq(f[j].v,b.v)||!eq(f[j].n,b.n))return false;}
                else {seen[j]=true;f[j]=b;q[r++]=j;}
            }
    }
    for(int i=0;i<6;++i)for(int j=0;j<i;++j)if(eq(f[i].n,f[j].n))return false;
    return true;
}
int a[100][100];
int main(){
    set<Shape> shapes; Shape one(1,P(0,0));shapes.insert(one);
    for(int k=1;k<6;++k){set<Shape> next;
        for(set<Shape>::const_iterator it=shapes.begin();it!=shapes.end();++it)
            for(int i=0;i<k;++i)for(int d=0;d<4;++d){
                P p((*it)[i].first+dx[d],(*it)[i].second+dy[d]);
                if(find(it->begin(),it->end(),p)==it->end()){
                    Shape t=*it;t.push_back(p);next.insert(normalize(t));
                }
            }
        shapes.swap(next);
    }
    vector<Shape> nets;
    for(set<Shape>::const_iterator it=shapes.begin();it!=shapes.end();++it)if(net(*it))nets.push_back(*it);
    int n,m;if(scanf("%d%d",&n,&m)!=2)return 0;
    for(int i=0;i<n;++i)for(int j=0;j<m;++j)scanf("%d",&a[i][j]);
    int best=0;
    for(int t=0;t<(int)nets.size();++t){const Shape &s=nets[t];int h=0,w=0;
        for(int k=0;k<6;++k){h=max(h,s[k].first+1);w=max(w,s[k].second+1);}
        for(int i=0;i+h<=n;++i)for(int j=0;j+w<=m;++j){int sum=0;
            for(int k=0;k<6;++k)sum+=a[i+s[k].first][j+s[k].second];
            best=max(best,sum);
        }
    }
    printf("%d\n",best);return 0;
}
