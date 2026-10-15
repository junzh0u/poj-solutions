// POJ 2842 - N-dimension Matching
// Model: gpt-6.1-sol
// Collapse each dimension with two unsigned rolling hashes. Inspect candidate
// origins in row-major (lexical) order and verify every element exactly, so
// hash collisions cannot affect the answer. Input contains one matrix pair.
#include <cstdio>
#include <vector>
using namespace std;
struct Hash {
    unsigned int x, y;
    Hash(unsigned int a=0, unsigned int b=0): x(a), y(b) {}
};
vector<Hash> collapse(const vector<Hash>& v, int len, int stride, int width, int dim) {
    const unsigned int bx=911382323u + 2u*dim, by=972663749u + 4u*dim;
    unsigned int px=1, py=1;
    for(int i=0;i<width;i++) { px*=bx; py*=by; }
    int outlen=len-width+1, block=len*stride;
    vector<Hash> out(v.size()/len*outlen);
    for(int outer=0;outer<(int)v.size()/block;outer++) {
        for(int inner=0;inner<stride;inner++) {
            Hash h;
            int start=outer*block+inner;
            for(int j=0;j<len;j++) {
                h.x=h.x*bx+v[start+j*stride].x;
                h.y=h.y*by+v[start+j*stride].y;
                if(j>=width) {
                    h.x-=v[start+(j-width)*stride].x*px;
                    h.y-=v[start+(j-width)*stride].y*py;
                }
                if(j>=width-1) out[(outer*outlen+j-width+1)*stride+inner]=h;
            }
        }
    }
    return out;
}
int main() {
    int n; if(scanf("%d",&n)!=1) return 0;
    int a[10],b[10],sa[10],dims[10],pos[10];
    int ns=1,nt=1;
    for(int i=0;i<n;i++) { scanf("%d",a+i); ns*=a[i]; }
    for(int i=0;i<n;i++) { scanf("%d",b+i); nt*=b[i]; }
    vector<int> s(ns),t(nt),offset(nt);
    vector<Hash> hs(ns),ht(nt);
    for(int i=0;i<ns;i++) { scanf("%d",&s[i]); hs[i]=Hash(s[i],s[i]); }
    for(int i=0;i<nt;i++) { scanf("%d",&t[i]); ht[i]=Hash(t[i],t[i]); }
    int stride=1;
    for(int d=n-1;d>=0;d--) { sa[d]=stride; stride*=a[d]; dims[d]=a[d]-b[d]+1; }
    for(int i=0;i<nt;i++) {
        int z=i;
        for(int d=n-1;d>=0;d--) { offset[i]+=(z%b[d])*sa[d]; z/=b[d]; }
    }
    stride=1;
    for(int d=n-1;d>=0;d--) {
        hs=collapse(hs,a[d],stride,b[d],d);
        ht=collapse(ht,b[d],1,b[d],d);
        stride*=dims[d];
    }
    for(int i=0;i<(int)hs.size();i++) {
        if(hs[i].x!=ht[0].x || hs[i].y!=ht[0].y) continue;
        int z=i,origin=0;
        for(int d=n-1;d>=0;d--) { pos[d]=z%dims[d]; z/=dims[d]; origin+=pos[d]*sa[d]; }
        int j=0;
        while(j<nt && s[origin+offset[j]]==t[j]) j++;
        if(j==nt) {
            for(int d=0;d<n;d++) printf("%s%d",d?" ":"",pos[d]+1);
            printf("\n"); return 0;
        }
    }
    return 0;
}
