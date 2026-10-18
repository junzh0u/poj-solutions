// POJ 3395 - Shift Cipher
// Model: gpt-6-astra
// Sparse dictionary trie and suffix DP for all 26 keys minimize word count.
// The DP remembers whether the preceding word was a singleton. Strictly
// more than two characters per word is checked after minimizing the count.
// Key zero is permitted; dictionary entries must consist only of a-z.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct Node {
    int child, next;
    char letter;
    bool terminal;
    Node(char c=0, int link=-1):child(-1),next(link),letter(c),terminal(false){}
};
vector<Node> trie(1);
int child(int p, char c) {
    for(int q=trie[p].child;q!=-1;q=trie[q].next)
        if(trie[q].letter==c) return q;
    return -1;
}
void insert(const string &s) {
    int p=0;
    for(size_t i=0;i<s.size();++i) {
        int q=child(p,s[i]);
        if(q==-1) {
            q=(int)trie.size();
            trie.push_back(Node(s[i],trie[p].child));
            trie[p].child=q;
        }
        p=q;
    }
    trie[p].terminal=true;
}
int main() {
    ios::sync_with_stdio(false); cin.tie(0);
    string line;
    while(getline(cin,line)) {
        if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
        if(line.empty()) break;
        bool good=line.size()<256;
        for(size_t i=0;i<line.size();++i) if(line[i]<'a'||line[i]>'z') good=false;
        if(good) insert(line);
    }
    while(getline(cin,line)) {
        if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
        if(line=="0") break;
        const int n=(int)line.size(), INF=1000000;
        int best=INF, key=-1;
        string answer;
        for(int k=0;k<26;++k) {
            string s=line;
            for(int i=0;i<n;++i) s[i]=char('a'+(line[i]-'a'-k+26)%26);
            vector< vector<int> > dp(n+1,vector<int>(2,INF));
            vector< vector<int> > take(n+1,vector<int>(2,-1));
            dp[n][0]=dp[n][1]=0;
            for(int i=n-1;i>=0;--i) {
                int p=0;
                for(int j=i;j<n;++j) {
                    p=child(p,s[j]); if(p==-1) break;
                    if(!trie[p].terminal) continue;
                    int single=(i==j);
                    for(int prev=0;prev<2;++prev) {
                        if(prev && single) continue;
                        int cost=1+dp[j+1][single];
                        if(cost<dp[i][prev]) {dp[i][prev]=cost;take[i][prev]=j+1;}
                    }
                }
            }
            if(dp[0][0]<best && 2*dp[0][0]<n) {
                best=dp[0][0]; key=k; answer="";
                int pos=0,prev=0;
                while(pos<n) {
                    int end=take[pos][prev];
                    if(pos) answer+=' ';
                    answer+=s.substr(pos,end-pos);
                    prev=(end-pos==1);pos=end;
                }
            }
        }
        if(key<0) cout<<"NO SOLUTIONS\n";
        else cout<<"k="<<key<<": "<<answer<<'\n';
    }
}
