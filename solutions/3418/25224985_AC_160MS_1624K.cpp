// POJ 3418 - Quadratic Functions
// Model: gpt-6.1-sol
// Since b=2a, f(x)=a*|x+1|^2+(c-a). On nonnegative u=|x+1|,
// any two such functions cross at most once, so a Li Chao tree applies.
// Keep the midpoint winner and send the loser only toward an endpoint
// where it is better. Signed coefficients are supported; evaluate in 64 bits.
#include <cstdio>
#include <algorithm>
using namespace std;
typedef long long LL;
struct Node { int a,d,left,right; } tree[1000005];
int used=0,root=0;
char buf[65536]; int pos=0,len=0;
int getch() { if(pos==len) { len=(int)fread(buf,1,sizeof(buf),stdin); pos=0; if(!len)return -1; } return buf[pos++]; }
int number() { int ch=getch(),s=1,v=0; while(ch<=32 && ch!=-1)ch=getch(); if(ch=='-'){s=-1;ch=getch();} while(ch>='0'&&ch<='9'){v=v*10+ch-'0';ch=getch();} return s*v; }
LL value(int a,int d,int x) { return (LL)a*x*x+d; }
void insert(int &p,int l,int r,int a,int d) {
    if(!p){p=++used;tree[p].a=a;tree[p].d=d;return;}
    int mid=(l+r)/2;
    if(value(a,d,mid)<value(tree[p].a,tree[p].d,mid)){
        swap(a,tree[p].a);swap(d,tree[p].d);
    }
    if(l==r)return;
    if(value(a,d,l)<value(tree[p].a,tree[p].d,l))insert(tree[p].left,l,mid,a,d);
    else if(value(a,d,r)<value(tree[p].a,tree[p].d,r))insert(tree[p].right,mid+1,r,a,d);
}
LL query(int x) {
    int p=root,l=0,r=1000001; LL best=value(tree[p].a,tree[p].d,x);
    while(p){
        LL v=value(tree[p].a,tree[p].d,x);if(v<best)best=v;
        int mid=(l+r)/2;
        if(x<=mid){p=tree[p].left;r=mid;}else{p=tree[p].right;l=mid+1;}
    }
    return best;
}
int main(){
    int m=number();
    while(m--){int op=getch();while(op<=32&&op!=-1)op=getch();
        if(op=='I'){int a=number();number();int c=number();insert(root,0,1000001,a,c-a);}
        else {int x=number()+1;if(x<0)x=-x;printf("%d\n",(int)query(x));}
    }
    return 0;
}
