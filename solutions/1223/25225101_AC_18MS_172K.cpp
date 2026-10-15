// POJ 1223 - DEHUFF
// Model: gpt-6.1-sol
// Assign codes at their first occurrence and backtrack through the bit string.
// A complete binary prefix tree with K leaves has K-1 internal nodes, which
// bounds code lengths and prunes partial tries. Zero solutions, like multiple
// solutions, print MULTIPLE TABLES (as clarified on the discussion board).
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
struct Tree {
    int ch[55][2], leaf[55], n, internal;
    Tree(): n(1), internal(0) { for(int i=0;i<55;++i) { ch[i][0]=ch[i][1]=-1; leaf[i]=0; } }
    bool insert(const string &s, int k) {
        int v=0;
        for(int i=0;i<(int)s.size();++i) {
            if(leaf[v]) return false;
            if(ch[v][0]<0 && ch[v][1]<0) { if(++internal>k-1) return false; }
            int b=s[i]-'0';
            if(ch[v][b]<0) { if(n>=55) return false; ch[v][b]=n++; }
            v=ch[v][b];
        }
        if(leaf[v] || ch[v][0]>=0 || ch[v][1]>=0) return false;
        leaf[v]=1; return true;
    }
};
string text,bits,code[128],answer[128];
int freq[128], k, solutions, total;
vector<int> alphabet;
int gcdint(int a,int b) { return b?gcdint(b,a%b):a; }
void search(int p,int q,const Tree &tree,int assigned,int cost) {
    if(solutions>=2) return;
    int left=(int)bits.size()-cost, weight=0,g=0;
    for(int j=0;j<k;++j) { int c=alphabet[j]; if(code[c].empty()) { weight+=freq[c];g=gcdint(g,freq[c]); } }
    if(left<weight || left>weight*(k-1) || (g && left%g)) return;
    while(p<(int)text.size() && !code[(int)text[p]].empty()) {
        const string &s=code[(int)text[p]];
        if(bits.compare(q,s.size(),s)!=0) return;
        q+=(int)s.size(); ++p;
    }
    if(p==(int)text.size()) {
        if(q!=(int)bits.size() || tree.internal!=k-1 || tree.n!=2*k-1) return;
        ++solutions;
        for(int i=0;i<k;++i) answer[alphabet[i]]=code[alphabet[i]];
        return;
    }
    int c=text[p];
    for(int len=1;len<=k-1 && q+len<=(int)bits.size();++len) {
        string s=bits.substr(q,len);
        Tree next=tree;
        if(!next.insert(s,k)) continue;
        code[c]=s;
        search(p+1,q+len,next,assigned+1,cost+len*freq[c]);
        code[c].clear();
        if(solutions>=2) return;
    }
}
int main() {
    int cases; cin>>cases; getline(cin,text);
    for(int tc=1;tc<=cases;++tc) {
        getline(cin,text); getline(cin,bits);
        if(!text.empty() && text[text.size()-1]=='\r') text.erase(text.size()-1);
        if(!bits.empty() && bits[bits.size()-1]=='\r') bits.erase(bits.size()-1);
        alphabet.clear(); for(int i=0;i<128;++i) {freq[i]=0;code[i].clear();}
        for(int i=0;i<(int)text.size();++i) ++freq[(int)text[i]];
        for(int i=0;i<128;++i) if(freq[i]) alphabet.push_back(i);
        k=(int)alphabet.size(); solutions=0;
        Tree tree; search(0,0,tree,0,0);
        cout<<"DATASET #"<<tc<<"\n";
        if(solutions!=1) cout<<"MULTIPLE TABLES\n";
        else for(int i=0;i<k;++i) cout<<(char)alphabet[i]<<" = "<<answer[alphabet[i]]<<"\n";
    }
}
