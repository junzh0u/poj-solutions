// POJ 2974 - Convex area
// Model: claude-sonnet-5
// Approach: incremental 3D convex hull (double precision). Points are added
// one at a time; for each new point the set of faces it can "see" (strictly
// positive side) is flood-filled and removed, and replaced by new faces
// joining the point to the horizon edges. The initial non-degenerate
// tetrahedron is found by scanning for a point not coincident with P0, a
// third point not collinear with the first two, and a fourth point not
// coplanar with the first three -- this handles input sets that include
// extra collinear or duplicate points (the discuss board for this problem
// notes the judge data includes three collinear points), since those points
// are simply skipped during the search and later treated as interior points
// (lying exactly on an existing face, contributing zero visibility) during
// the incremental step. Ambiguity check: the problem guarantees at most 3
// points per hull face, so every maximal face is already a triangle and no
// coplanar-triangle merging is required; summing all final triangle areas
// gives the exact surface area.
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

const double EPS = 1e-8;
int sgn(double x){ if(fabs(x) < EPS) return 0; return x < 0 ? -1 : 1; }

struct Pt{
    double x,y,z;
    Pt(){}
    Pt(double x_,double y_,double z_):x(x_),y(y_),z(z_){}
    Pt operator-(const Pt&b) const{ return Pt(x-b.x,y-b.y,z-b.z); }
    Pt operator*(const Pt&b) const{
        return Pt(y*b.z-z*b.y, z*b.x-x*b.z, x*b.y-y*b.x);
    }
    double operator^(const Pt&b) const{ return x*b.x+y*b.y+z*b.z; }
};

double vlen(const Pt&a){ return sqrt(a^a); }

const int MAXN = 30;
const int MAXF = 2000; // generous bound on total faces ever created
struct Face{ int a,b,c; bool ok; };

int n;
Pt P[MAXN];
int trianglecnt;
int adj[MAXN][MAXN];
Face F[MAXF];

double ptof(const Pt&p, const Face&f){
    Pt m = P[f.b]-P[f.a];
    Pt m2 = P[f.c]-P[f.a];
    Pt t = p-P[f.a];
    return (m*m2)^t;
}

void deal(int p,int a,int b);

void dfs(int p,int now){
    F[now].ok = false;
    deal(p,F[now].b,F[now].a);
    deal(p,F[now].c,F[now].b);
    deal(p,F[now].a,F[now].c);
}

void deal(int p,int a,int b){
    int f = adj[a][b];
    if(F[f].ok){
        if(sgn(ptof(P[p],F[f])) > 0){
            dfs(p,f);
        } else {
            Face add;
            add.a=b; add.b=a; add.c=p; add.ok=true;
            adj[b][a]=adj[a][p]=adj[p][b]=trianglecnt;
            F[trianglecnt++]=add;
        }
    }
}

double triArea(const Pt&a,const Pt&b,const Pt&c){
    return vlen((b-a)*(c-a));
}

double construct(){
    trianglecnt = 0;
    if(n < 4) return 0.0;

    // find index 1: a point distinct from P[0]
    bool degenerate = true;
    for(int i=1;i<n;i++){
        if(vlen(P[0]-P[i]) > EPS){
            swap(P[1],P[i]);
            degenerate = false;
            break;
        }
    }
    if(degenerate) return 0.0;

    // find index 2: a point not collinear with P[0],P[1]
    degenerate = true;
    for(int i=2;i<n;i++){
        if(vlen((P[0]-P[1])*(P[1]-P[i])) > EPS){
            swap(P[2],P[i]);
            degenerate = false;
            break;
        }
    }
    if(degenerate) return 0.0;

    // find index 3: a point not coplanar with P[0],P[1],P[2]
    degenerate = true;
    for(int i=3;i<n;i++){
        if(fabs(((P[0]-P[1])*(P[1]-P[2])) ^ (P[0]-P[i])) > EPS){
            swap(P[3],P[i]);
            degenerate = false;
            break;
        }
    }
    if(degenerate) return 0.0;

    for(int i=0;i<4;i++){
        Face add;
        add.a=(i+1)%4; add.b=(i+2)%4; add.c=(i+3)%4; add.ok=true;
        if(sgn(ptof(P[i],add)) > 0) swap(add.b,add.c);
        adj[add.a][add.b]=adj[add.b][add.c]=adj[add.c][add.a]=trianglecnt;
        F[trianglecnt++]=add;
    }

    for(int i=4;i<n;i++){
        for(int j=0;j<trianglecnt;j++){
            if(F[j].ok && sgn(ptof(P[i],F[j])) > 0){
                dfs(i,j);
                break;
            }
        }
    }

    double area = 0.0;
    for(int i=0;i<trianglecnt;i++){
        if(F[i].ok){
            area += triArea(P[F[i].a],P[F[i].b],P[F[i].c]);
        }
    }
    return area/2.0;
}

int main(){
    while(scanf("%d",&n)==1 && n!=0){
        for(int i=0;i<n;i++){
            double x,y,z;
            scanf("%lf%lf%lf",&x,&y,&z);
            P[i]=Pt(x,y,z);
        }
        double area = construct();
        int r = (int)floor(area + 0.5);
        printf("%d\n", r);
    }
    return 0;
}
