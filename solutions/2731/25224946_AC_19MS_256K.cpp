// POJ 2731 - ACM(ACronymMaker)
// Model: gpt-6.1-sol
// Count ordered subsequences word by word, requiring a contribution from each
// significant word. Six base-10^9 limbs avoid overflow, including intermediate
// states: a line has at most 150 characters, hence fewer than 2^150 subsets.
// The discussion reports answers exceeding the statement's 32-bit bound.
#include <iostream>
#include <sstream>
#include <string>
#include <set>
#include <vector>
#include <iomanip>
using namespace std;
struct Count {
    int a[6];
    Count(int v=0) { for(int i=0;i<6;++i) a[i]=0; a[0]=v; }
    void add(const Count &b) {
        int carry=0;
        for(int i=0;i<6;++i) {
            int t=a[i]+b.a[i]+carry;
            carry=t>=1000000000; a[i]=t-carry*1000000000;
        }
    }
    bool zero() const { for(int i=0;i<6;++i) if(a[i]) return false; return true; }
};
ostream& operator<<(ostream& out,const Count& v) {
    int k=5; while(k>0 && !v.a[k]) --k;
    out<<v.a[k]; while(k--) out<<setw(9)<<setfill('0')<<v.a[k];
    return out;
}
int main() {
    int n; string line,w;
    while(cin>>n && n) {
        set<string> ignore;
        for(int i=0;i<n;++i) { cin>>w; ignore.insert(w); }
        getline(cin,line);
        while(getline(cin,line)) {
            if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
            if(line=="LAST CASE") break;
            istringstream in(line); string abbr; in>>abbr;
            if(abbr.empty()) continue;
            int m=abbr.size(); vector<Count> prev(m+1); prev[0]=Count(1);
            while(in>>w) if(!ignore.count(w)) {
                vector<Count> used(m+1);
                for(int c=0;c<(int)w.size();++c)
                    for(int j=m-1;j>=0;--j) if(abbr[j]-'A'==w[c]-'a') {
                        used[j+1].add(used[j]); used[j+1].add(prev[j]);
                    }
                prev=used;
            }
            cout<<abbr;
            if(prev[m].zero()) cout<<" is not a valid abbreviation\n";
            else cout<<" can be formed in "<<prev[m]<<" ways\n";
        }
    }
    return 0;
}
