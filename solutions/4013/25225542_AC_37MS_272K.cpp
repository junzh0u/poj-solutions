// POJ 4013 - Cube Coloring
// Model: gpt-6.1-sol
// Enumerate permutations of the six main-face colors, compare unordered
// face visions, and deduplicate lexicographically minimal proper rotations.
// Match the original Java 6 jury traversal order: POJ uses exact output.
// The missing POJ statement is recovered from NEERC Northern 2011 C.
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
using namespace std;
const int n[6][6]={{0,1,2,3,4,5},{1,0,4,5,2,3},{2,0,1,5,3,4},{3,0,2,5,4,1},{4,0,3,5,1,2},{5,4,3,2,1,0}};
string canonical(const string &s) {
    string best=s;
    for(int f=0;f<6;++f) for(int r=0;r<4;++r) {
        string t=s;
        t[0]=s[n[f][0]]; t[5]=s[n[f][5]];
        for(int k=0;k<4;++k) t[k+1]=s[n[f][1+(k+r)%4]];
        if(t<best) best=t;
    }
    return best;
}
vector<string> vision(const string &s) {
    vector<string> v;
    for(int f=0;f<6;++f) {
        string t;
        for(int j=0;j<5;++j) t+=s[n[f][j]];
        sort(t.begin()+1,t.end()); v.push_back(t);
    }
    sort(v.begin(),v.end()); return v;
}
int bucket(char c) { int h=c; h^=(h>>20)^(h>>12); return (h^(h>>7)^(h>>4))&15; }
struct HashOrder { bool operator()(char a,char b) const { return bucket(a)!=bucket(b)?bucket(a)<bucket(b):a<b; } };
int main() {
    vector<string> v(6); string s;
    for(int i=0;i<6;++i) {
        if(!(cin>>v[i])) return 0;
        s+=v[i][0]; sort(v[i].begin()+1,v[i].end());
    }
    sort(v.begin(),v.end()); sort(s.begin(),s.end(),HashOrder());
    int found=0; set<string> seen;
    do {
        string t=canonical(s);
        if(seen.insert(t).second && vision(t)==v) {
            cout<<t<<'\n'; if(++found==2) break;
        }
    } while(next_permutation(s.begin(),s.end(),HashOrder()));
    if(!found) cout<<"Impossible\n";
    return 0;
}
