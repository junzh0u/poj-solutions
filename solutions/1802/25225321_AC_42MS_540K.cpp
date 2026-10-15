// POJ 1802 - Ancient Plates
// Model: gpt-6.1-sol
// Rules on different levels commute. Compose one permutation per used size
// at each level; circular offsets make shifts constant time. Empty lines
// remain lines, while exchanges with missing positions are skipped.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
struct Rule { bool shift; string k; vector<pair<int,int> > pairs; };
int indexValue(const string &s) {
    int n=0;
    for(size_t i=0;i<s.size();++i) { n=n*10+s[i]-'0'; if(n>1000) return 1001; }
    return n;
}
int residue(const string &s,int n) {
    int v=0; bool neg=s[0]=='-';
    for(size_t i=(s[0]=='-'||s[0]=='+');i<s.size();++i) v=(v*10+s[i]-'0')%n;
    return neg && v ? n-v : v;
}
vector<int> permutation(int n,const vector<Rule> &rules) {
    vector<int> p(n); for(int i=0;i<n;++i)p[i]=i;
    if(!n)return p;
    int off=0;
    for(size_t r=0;r<rules.size();++r) {
        if(rules[r].shift) off=(off+residue(rules[r].k,n))%n;
        else for(size_t j=0;j<rules[r].pairs.size();++j) {
            int a=rules[r].pairs[j].first,b=rules[r].pairs[j].second;
            if(a>0 && b>0 && a<=n && b<=n) swap(p[(a-1+off)%n],p[(b-1+off)%n]);
        }
    }
    rotate(p.begin(),p.begin()+off,p.end()); return p;
}
string lineRead() { string s; getline(cin,s); if(!s.empty()&&s[s.size()-1]=='\r')s.erase(s.size()-1); return s; }
int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    int cases; cin>>cases; lineRead();
    for(int tc=1;tc<=cases;++tc) {
        int n; cin>>n; lineRead(); vector<vector<string> > text(n);
        for(int i=0;i<n;++i) { string s=lineRead(),w; istringstream in(s); while(in>>w)text[i].push_back(w); }
        int r; cin>>r; lineRead(); vector<Rule> rules[3];
        for(int i=0;i<r;++i) {
            istringstream in(lineRead()); string op,target,t; in>>op>>target;
            Rule rule; rule.shift=op=="SHIFT";
            if(rule.shift)in>>rule.k;
            else while(in>>t) { size_t c=t.find(','); rule.pairs.push_back(make_pair(indexValue(t.substr(0,c)),indexValue(t.substr(c+1)))); }
            int level=target=="L"?0:target=="W"?1:2; rules[level].push_back(rule);
        }
        vector<int> lp=permutation(n,rules[0]); vector<vector<int> > wp(1001),cp(1001);
        vector<bool> wd(1001,false),cd(1001,false);
        cout<<"Scenario #"<<tc<<":\n";
        for(int i=0;i<n;++i) {
            const vector<string> &words=text[lp[i]]; int m=words.size();
            if(!wd[m]) {wp[m]=permutation(m,rules[1]);wd[m]=true;}
            for(int j=0;j<m;++j) {
                const string &w=words[wp[m][j]]; int len=w.size();
                if(!cd[len]) {cp[len]=permutation(len,rules[2]);cd[len]=true;}
                if(j)cout<<' ';
                for(int k=0;k<len;++k)cout<<w[cp[len][k]];
            }
            cout<<'\n';
        }
        cout<<'\n';
    }
}
