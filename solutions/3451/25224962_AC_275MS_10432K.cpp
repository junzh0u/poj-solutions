// POJ 3451 - Money Money Money, Must Be Funny
// Model: gpt-6.1-sol
// Compute bounded minimum-coin tables for both purses, then match payment
// against change. Residue-class monotone queues make each type linear.
// Parse decimal amounts exactly as cents; zero-value tenders are useless.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
const int INF=1000000;
struct Coin { int v,n; Coin(int a,int b):v(a),n(b){} };
int cents(string s) {
    size_t p=s.find('.');
    if(p==string::npos) { int x=0; for(size_t i=0;i<s.size();++i) x=x*10+s[i]-'0'; return x*100; }
    int x=0; for(size_t i=0;i<p;++i) x=x*10+s[i]-'0';
    int f=s[p+1]-'0'; f*=10; if(p+2<s.size()) f+=s[p+2]-'0';
    return x*100+f;
}
vector<Coin> readpurse() {
    vector<Coin> a; string s; int n; char x;
    while(cin>>s && s!="-1") { cin>>n>>x; int v=cents(s); if(v && n) a.push_back(Coin(v,n)); }
    return a;
}
vector<int> table(const vector<Coin>& a) {
    int sum=0; for(size_t i=0;i<a.size();++i) sum+=a[i].v*a[i].n;
    vector<int> d(sum+1,INF), qi(sum+1), qv(sum+1); d[0]=0;
    int reach=0;
    for(size_t i=0;i<a.size();++i) {
        int v=a[i].v,n=a[i].n,newreach=reach+v*n;
        for(int r=0;r<v && r<=newreach;++r) {
            int h=0,t=0,k=0;
            for(int s=r;s<=newreach;s+=v,++k) {
                int old=(s<=reach ? d[s] : INF);
                while(h<t && qi[h]<k-n) ++h;
                if(old<INF) {
                    int val=old-k;
                    while(h<t && qv[t-1]>=val) --t;
                    qi[t]=k; qv[t++]=val;
                }
                d[s]=(h<t ? qv[h]+k : INF);
            }
        }
        reach=newreach;
    }
    return d;
}
int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    string s;
    while(cin>>s && s!="-1") {
        int price=cents(s); vector<Coin> a=readpurse(),b=readpurse();
        vector<int> p=table(a),q=table(b); int ans=INF;
        for(int paid=price;paid<(int)p.size() && paid-price<(int)q.size();++paid)
            ans=min(ans,p[paid]+q[paid-price]);
        if(ans>=INF) cout<<"The payment is impossible.\n";
        else cout<<ans<<" tenders must be exchanged.\n";
    }
}
