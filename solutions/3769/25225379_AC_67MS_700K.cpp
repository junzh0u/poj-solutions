// POJ 3769 - Baseball on the Mercury
// Model: gpt-6.1-sol
// Backward expectation DP on the eight occupied-base masks. Enumerate
// legal destinations after each observed batting level, then choose the
// best continuation. Only the batter must advance; existing runners may
// stay. A sacrifice always costs one out, including when it succeeds.
#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;
struct Move { int mask, score; Move(int m,int s):mask(m),score(s){} };
vector<Move> moves[8][5];
int pos[4], cnt, limit;
void generate(int at,int mask,int score,vector<Move>& out) {
    if(at==cnt) { out.push_back(Move(mask,score)); return; }
    int p=pos[at], low=(p==0?1:0);
    for(int d=low;d<=limit;++d) {
        int q=p+d;
        if(q>=4) generate(at+1,mask,score+1,out);
        else if(!(mask&(1<<(q-1)))) generate(at+1,mask|(1<<(q-1)),score,out);
    }
}
int main() {
    for(int m=0;m<8;++m) for(int k=1;k<=4;++k) {
        cnt=0; if(!(m&4)) pos[cnt++]=0;
        for(int b=1;b<=3;++b) if(m&(1<<(b-1))) pos[cnt++]=b;
        limit=(m&4)?2:k;
        generate(0,0,0,moves[m][k]);
    }
    int n;
    while(scanf("%d",&n)==1 && n) {
        vector<double> p(n*5);
        for(int i=0;i<n*5;++i) scanf("%lf",&p[i]);
        double next[8]={0},cur[8];
        for(int i=n-1;i>=0;--i) {
            for(int m=0;m<8;++m) {
                if(m&4) {
                    double best=-1e100;
                    const vector<Move>& v=moves[m][1];
                    for(size_t j=0;j<v.size();++j) best=max(best,v[j].score+next[v[j].mask]);
                    double s=p[i*5+4];
                    cur[m]=-1+(1-s)*next[m]+s*best;
                } else {
                    double fail=1,ans=0;
                    for(int k=1;k<=4;++k) {
                        double best=-1e100;
                        const vector<Move>& v=moves[m][k];
                        for(size_t j=0;j<v.size();++j) best=max(best,v[j].score+next[v[j].mask]);
                        double s=p[i*5+k-1]; fail-=s; ans+=s*best;
                    }
                    cur[m]=ans+fail*(next[m]-1);
                }
            }
            for(int m=0;m<8;++m) next[m]=cur[m];
        }
        printf("%.3f\n",next[0]+1e-9);
    }
    return 0;
}
