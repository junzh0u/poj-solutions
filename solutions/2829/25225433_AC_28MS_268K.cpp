// POJ 2829 - How Many Free Dimensions?
// Model: gpt-6.1-sol
// Count equivalence classes of components under injective letter assignments.
// Different equality patterns give disjoint matching edges. Equal patterns give
// a permutation of letters, whose order is every assignment orbit's length.
// Repeated-letter multiplicities need not agree between the two sides.
#include <iostream>
#include <string>
using namespace std;
int gcd(int a,int b) { return b ? gcd(b,a%b) : a; }
int main() {
    long long n;
    int k;
    while (cin >> n >> k && (n || k)) {
        string rule; cin >> rule;
        string a=rule.substr(0,k), b=rule.substr(k+1);
        long long total=1;
        for(int i=0;i<k;++i) total*=n;
        int id[26], m=0;
        for(int i=0;i<26;++i) id[i]=-1;
        for(int i=0;i<k;++i) if(id[a[i]-'a']<0) id[a[i]-'a']=m++;
        long long ways=1;
        if(n<m) ways=0;
        else for(int i=0;i<m;++i) ways*=n-i;
        int p[26], rev[26];
        for(int i=0;i<26;++i) p[i]=rev[i]=-1;
        bool same=true;
        for(int i=0;i<k;++i) {
            int x=id[a[i]-'a'], y=id[b[i]-'a'];
            if((p[x]>=0 && p[x]!=y)||(rev[y]>=0 && rev[y]!=x)) same=false;
            p[x]=y; rev[y]=x;
        }
        if(!same) total-=ways;
        else {
            bool seen[26]={false}; int order=1;
            for(int i=0;i<m;++i) if(!seen[i]) {
                int j=i,len=0;
                do { seen[j]=true; ++len; j=p[j]; } while(j!=i);
                order=order/gcd(order,len)*len;
            }
            total-=ways-ways/order;
        }
        cout << total << '\n';
    }
}
