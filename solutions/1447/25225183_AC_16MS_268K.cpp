// POJ 1447 - Ambiguous Dates
// Model: gpt-6.1-sol
// Enumerate component boundaries and all day/month/year assignments.
// A one- or two-digit year denotes every matching century from 1700 to 2299.
// Gregorian day numbers sort and deduplicate all valid interpretations.
#include <iostream>
#include <string>
#include <set>
#include <algorithm>
#include <cstdlib>
using namespace std;
int md[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
bool leap(int y) { return y%4==0 && (y%100!=0 || y%400==0); }
int serial(int y,int m,int d) {
    int z=y-1;
    int ans=365*z+z/4-z/100+z/400+d;
    for(int i=1;i<m;++i) ans+=md[i]+(i==2 && leap(y));
    return ans;
}
void interpret(string a[3],set<int>& out) {
    int p[]={0,1,2};
    do {
        string ds=a[p[0]], ms=a[p[1]], ys=a[p[2]];
        if(ds.size()>2 || ms.size()>2 || (ys.size()!=1 && ys.size()!=2 && ys.size()!=4)) continue;
        int d=atoi(ds.c_str()), m=atoi(ms.c_str()), y=atoi(ys.c_str());
        if(m<1 || m>12 || d<1) continue;
        int first=ys.size()==4?y:1700+y;
        int last=ys.size()==4?y:2200+y;
        for(int v=first;v<=last;v+=100) {
            if(v<1700 || v>2299 || d>md[m]+(m==2 && leap(v))) continue;
            out.insert(serial(v,m,d)-serial(2001,11,4));
        }
    } while(next_permutation(p,p+3));
}
int main() {
    int n; cin>>n;
    for(int tc=1;tc<=n;++tc) {
        string s; cin>>s;
        set<int> out;
        size_t q=s.find_first_not_of("0123456789");
        if(q!=string::npos) {
            size_t r=s.find(s[q],q+1);
            string a[]={s.substr(0,q),s.substr(q+1,r-q-1),s.substr(r+1)};
            interpret(a,out);
        } else {
            for(size_t i=1;i+1<s.size();++i) for(size_t j=i+1;j<s.size();++j) {
                string a[]={s.substr(0,i),s.substr(i,j-i),s.substr(j)};
                interpret(a,out);
            }
        }
        cout<<"Scenario #"<<tc<<":\n";
        if(out.empty()) cout<<"Illegal date\n";
        for(set<int>::iterator i=out.begin();i!=out.end();++i) cout<<*i<<'\n';
        cout<<'\n';
    }
}
