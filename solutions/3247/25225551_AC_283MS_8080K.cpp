// POJ 3247 - Robots
// Model: gpt-6.1-sol
// Store rod displacement vectors in a segment tree. A rotation at joint idx
// rotates exactly rods idx+1 through N; joint positions are prefix sums.
// Lazy rotation matrices compose in chronological order (new times old).
// Axes are world-coordinate vectors, with the right-hand direction stated
// by the examples. Double precision follows the discussion's warning.
#include <cstdio>
#include <cmath>
struct Vec { double x[3]; };
struct Mat { double a[3][3]; };
struct Node { Vec v; Mat m; bool lazy; } tr[200020];
Vec rods[50005];
Vec add(Vec a,Vec b){for(int i=0;i<3;++i)a.x[i]+=b.x[i];return a;}
Vec mul(const Mat &a,Vec b){Vec c={{0,0,0}};for(int i=0;i<3;++i)for(int j=0;j<3;++j)c.x[i]+=a.a[i][j]*b.x[j];return c;}
Mat mul(const Mat &a,const Mat &b){Mat c;for(int i=0;i<3;++i)for(int j=0;j<3;++j){c.a[i][j]=0;for(int k=0;k<3;++k)c.a[i][j]+=a.a[i][k]*b.a[k][j];}return c;}
void apply(int p,const Mat &a){tr[p].v=mul(a,tr[p].v);if(tr[p].lazy)tr[p].m=mul(a,tr[p].m);else tr[p].m=a;tr[p].lazy=true;}
void push(int p){if(tr[p].lazy){apply(p*2,tr[p].m);apply(p*2+1,tr[p].m);tr[p].lazy=false;}}
void build(int p,int l,int r){tr[p].lazy=false;if(l==r){tr[p].v=rods[l];return;}int m=(l+r)/2;build(p*2,l,m);build(p*2+1,m+1,r);tr[p].v=add(tr[p*2].v,tr[p*2+1].v);}
void rotate(int p,int l,int r,int q,const Mat &a){if(q<=l){apply(p,a);return;}push(p);int m=(l+r)/2;if(q<=m)rotate(p*2,l,m,q,a);rotate(p*2+1,m+1,r,q,a);tr[p].v=add(tr[p*2].v,tr[p*2+1].v);}
Vec query(int p,int l,int r,int q){if(r<=q)return tr[p].v;push(p);int m=(l+r)/2;Vec a=query(p*2,l,m,q);if(q>m)a=add(a,query(p*2+1,m+1,r,q));return a;}
int main(){int n,m;while(scanf("%d%d",&n,&m)==2 && n){Vec prev,cur;scanf("%lf%lf%lf",&prev.x[0],&prev.x[1],&prev.x[2]);for(int i=1;i<=n;++i){scanf("%lf%lf%lf",&cur.x[0],&cur.x[1],&cur.x[2]);for(int j=0;j<3;++j)rods[i].x[j]=cur.x[j]-prev.x[j];prev=cur;}build(1,1,n);while(m--){int cmd,t;scanf("%d%d",&cmd,&t);if(cmd==2){Vec a={{0,0,0}};if(t)a=query(1,1,n,t);for(int i=0;i<3;++i)if(fabs(a.x[i])<0.00005)a.x[i]=0;printf("%.4f %.4f %.4f\n",a.x[0],a.x[1],a.x[2]);}else{double angle,u[3];scanf("%lf%lf%lf%lf",&angle,&u[0],&u[1],&u[2]);double len=sqrt(u[0]*u[0]+u[1]*u[1]+u[2]*u[2]);for(int i=0;i<3;++i)u[i]/=len;double c=cos(angle),s=sin(angle);Mat a;for(int i=0;i<3;++i)for(int j=0;j<3;++j)a.a[i][j]=(i==j?c:0)+(1-c)*u[i]*u[j];a.a[0][1]-=s*u[2];a.a[1][0]+=s*u[2];a.a[0][2]+=s*u[1];a.a[2][0]-=s*u[1];a.a[1][2]-=s*u[0];a.a[2][1]+=s*u[0];rotate(1,1,n,t+1,a);}}puts("");}return 0;}
