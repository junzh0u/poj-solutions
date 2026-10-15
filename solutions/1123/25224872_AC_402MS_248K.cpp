// POJ 1123 - For the Porsche
// Model: gpt-6.1-sol
// Enumerate every feature subset, including unused features needed to meet
// the minimum cost. Compare PI rounded to thousandths before all tie-breaks.
// Integer arithmetic rounds positive ratios exactly; 64-bit totals avoid
// overflow when summing the up to twenty input costs and sales values.
#include <iostream>
#include <iomanip>
using namespace std;
typedef long long Int;
int n,m,need[20],winner,bf,bc;
Int lo,hi,cost[20],sale[20],bs,bd,bpi;
void enumerate(int at,int mask,Int c,int f) {
    if(c>hi) return;
    if(at<n) {
        enumerate(at+1,mask,c,f);
        enumerate(at+1,mask|(1<<at),c+cost[at],f+1);
        return;
    }
    if(c<lo) return;
    Int s=0; int customers=0;
    for(int j=0;j<m;++j) if((mask&need[j])==need[j]) {
        s+=sale[j]; ++customers;
    }
    Int pi=(2000*s+c)/(2*c), margin=s-c;
    if(winner<0 || pi>bpi || (pi==bpi &&
       (margin>bs-bd || (margin==bs-bd &&
       (f<bf || (f==bf && customers>bc)))))) {
        winner=mask; bs=s; bd=c; bpi=pi; bf=f; bc=customers;
    }
}
int main() {
    int tests; cin>>tests;
    for(int tc=1;tc<=tests;++tc) {
        cin>>lo>>hi>>n>>m;
        for(int i=0;i<n;++i) cin>>cost[i];
        for(int j=0;j<m;++j) {
            int k; cin>>k; need[j]=0;
            while(k--) { int v; cin>>v; need[j]|=1<<(v-1); }
            cin>>sale[j];
        }
        winner=-1; enumerate(0,0,0,0);
        cout<<"Feature Set "<<tc<<'\n';
        cout<<bpi/1000<<'.'<<setfill('0')<<setw(3)<<bpi%1000
            <<setfill(' ')<<'\n'<<bs<<'\n'<<bd<<'\n';
        bool first=true;
        for(int i=0;i<n;++i) if(winner&(1<<i)) {
            if(!first) cout<<' '; cout<<i+1; first=false;
        }
        cout<<'\n'; first=true;
        for(int j=0;j<m;++j) if((winner&need[j])==need[j]) {
            if(!first) cout<<' '; cout<<j+1; first=false;
        }
        cout<<'\n';
    }
    return 0;
}
