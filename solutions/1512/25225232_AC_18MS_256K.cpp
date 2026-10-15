// POJ 1512 - Keeps Going and Going and ...
// Model: gpt-6.1-sol
// Follow one requested position through prefixes and alternating zip branches.
// A cycle containing only prefixes is skipped modulo its total prefix length;
// every positive-position zip step halves the position. Undefined zero-position
// zip cycles are guarded defensively; meaningful requests reach an integer.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
using namespace std;
struct List { bool zip; int a,b; vector<unsigned long> v; List():zip(false),a(0),b(0) {} } d[26];
unsigned long value(int x, unsigned long p) {
    unsigned long seen[26] = {0};
    bool used[26] = {false}, zero[26] = {false};
    for (;;) {
        if (d[x].zip) {
            if (p==0) { if (zero[x]) return 0; zero[x]=true; }
            int y=(p%2==0)?d[x].a:d[x].b;
            p/=2; x=y;
            for (int i=0;i<26;++i) used[i]=false;
        } else {
            if (used[x]) {
                unsigned long period=seen[x]-p;
                if (period) p%=period;
                for (int i=0;i<26;++i) used[i]=false;
            }
            if (p<d[x].v.size()) return d[x].v[(size_t)p];
            used[x]=true; seen[x]=p;
            p-=d[x].v.size(); x=d[x].a;
        }
    }
}
int main() {
    int n,m; if (!(cin>>n>>m)) return 0;
    string line; getline(cin,line);
    for (int i=0;i<n;++i) {
        getline(cin,line); istringstream in(line);
        char name,eq; string t; in>>name>>eq>>t;
        List &a=d[name-'A'];
        if (t=="zip") { char b,c; in>>b>>c; a.zip=true; a.a=b-'A'; a.b=c-'A'; }
        else {
            do {
                if (t.size()==1 && t[0]>='A' && t[0]<='Z') { a.a=t[0]-'A'; break; }
                a.v.push_back(strtoul(t.c_str(),0,10));
            } while (in>>t);
        }
    }
    for (int i=0;i<m;++i) {
        char name; unsigned long s,e; cin>>name>>s>>e;
        unsigned long count=e-s;
        for (unsigned long j=0;j<=count;++j) {
            if (j) cout<<' ';
            cout<<value(name-'A',s+j);
        }
        cout<<'\n';
    }
    return 0;
}
