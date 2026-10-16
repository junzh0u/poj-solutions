// POJ 3822 - Cover Up
// Model: gpt-6.1-sol
// Memoize remaining counts in the two equiprobable candidate groups.
// Try every combination of guesses and sum every nonempty success subset;
// an all-wrong turn loses immediately. Zero-probability choices are legal.
#include <cstdio>
#include <cstring>
#include <map>
#include <algorithm>
using namespace std;
int n, cnt[5], aa[5][64], bb[5][64], ids[5][11][11];
double wa[5], wb[5];
map<int,double> memo;
double solve(int key) {
    if (!key) return 1;
    map<int,double>::iterator it=memo.find(key);
    if(it!=memo.end()) return it->second;
    int pos[5], st[5], k=0;
    for(int i=0;i<n;++i) { int s=(key>>(6*i))&63; if(s) {pos[k]=i;st[k++]=s;} }
    double best=0;
    for(int choice=0;choice<(1<<k);++choice) {
        double p[5]; int failed[5]; bool valid=true;
        for(int j=0;j<k;++j) {
            int i=pos[j],a=aa[i][st[j]],b=bb[i][st[j]];
            bool pickA=(choice>>j)&1;
            if((pickA?a:b)==0) {valid=false;break;}
            double total=a*wa[i]+b*wb[i];
            p[j]=(pickA?wa[i]:wb[i])/total;
            failed[j]=ids[i][a-(pickA?1:0)][b-(pickA?0:1)];
        }
        if(!valid) continue;
        double value=0;
        for(int success=1;success<(1<<k);++success) {
            double prob=1;int next=0;
            for(int j=0;j<k;++j) {
                if(success&(1<<j)) prob*=p[j];
                else {prob*=1-p[j];next|=failed[j]<<(6*pos[j]);}
            }
            if(prob>0) value+=prob*solve(next);
        }
        best=max(best,value);
    }
    return memo[key]=best;
}
int main() {
    while(scanf("%d",&n)==1 && n) {
        int start=0;memset(ids,0,sizeof(ids));
        for(int i=0;i<n;++i) {
            int m,l;double p;scanf("%d%d%lf",&m,&l,&p);
            wa[i]=l?p/l:0;wb[i]=(1-p)/(m-l);cnt[i]=0;
            for(int a=0;a<=l;++a) for(int b=0;b<=m-l;++b) if(a||b) {
                int s=++cnt[i];ids[i][a][b]=s;aa[i][s]=a;bb[i][s]=b;
            }
            start|=ids[i][l][m-l]<<(6*i);
        }
        memo.clear();double ans=solve(start);char out[30];
        sprintf(out,"%.3f",ans+1e-12);int len=(int)strlen(out);
        while(len && out[len-1]=='0') out[--len]=0;
        if(len && out[len-1]=='.') out[--len]=0;
        puts(out);
    }
    return 0;
}
