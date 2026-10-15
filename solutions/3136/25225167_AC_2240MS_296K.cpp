// POJ 3136 - The Best Name for Your Baby
// Model: gpt-6.1-sol
// Keep the least terminal string for every nonterminal and length.
// Convolve each rule's symbols, relaxing to a fixed point at each length.
// Empty productions and nonproductive cycles are handled explicitly.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct Rule { int lhs; string rhs; };
int main() {
    int n, limit;
    while (cin >> n >> limit && (n || limit)) {
        vector<Rule> rules(n);
        string line;
        for (int i=0;i<n;++i) {
            cin >> line;
            rules[i].lhs=line[0]-'A'; rules[i].rhs=line.substr(2);
        }
        const string inf="{";
        string best[26][21];
        for(int a=0;a<26;++a) for(int k=0;k<=limit;++k) best[a][k]=inf;
        for(int len=0;len<=limit;++len) {
            bool changed=true;
            while(changed) {
                changed=false;
                for(int r=0;r<n;++r) {
                    vector<string> cur(len+1,inf); cur[0]="";
                    for(size_t p=0;p<rules[r].rhs.size();++p) {
                        char c=rules[r].rhs[p];
                        vector<string> next(len+1,inf);
                        for(int used=0;used<=len;++used) if(cur[used]!=inf) {
                            if(c>='a' && c<='z') {
                                if(used<len) {
                                    string s=cur[used]+c;
                                    if(s<next[used+1]) next[used+1]=s;
                                }
                            } else for(int k=0;used+k<=len;++k) if(best[c-'A'][k]!=inf) {
                                string s=cur[used]+best[c-'A'][k];
                                if(s<next[used+k]) next[used+k]=s;
                            }
                        }
                        cur.swap(next);
                    }
                    if(cur[len]<best[rules[r].lhs][len]) {
                        best[rules[r].lhs][len]=cur[len]; changed=true;
                    }
                }
            }
        }
        cout << (best['S'-'A'][limit]==inf ? "-" : best['S'-'A'][limit]) << '\n';
    }
}
