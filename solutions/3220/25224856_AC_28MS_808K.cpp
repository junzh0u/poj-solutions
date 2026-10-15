// POJ 3220 - Context-Free Languages
// Model: gpt-6.1-sol
// Memoize whether each variable derives a given substring. Binary rules
// try every nonempty split; precomputed feasible lengths prune impossible
// states. Alternatives may mix terminal and binary rules on the same line.
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;
vector< pair<int,int> > rules[26];
bool terminal[26][26];
vector< vector<unsigned char> > lengths;
vector<unsigned char> memo;
string word;
int n;
bool parse(int a,int l,int r) {
    if (!lengths[a][r-l]) return false;
    int index=(l*n+r-1)*26+a;
    if(memo[index]) return memo[index]==2;
    bool ok=false;
    if(r-l==1) ok=terminal[a][word[l]-'a'];
    else for(unsigned int t=0;t<rules[a].size() && !ok;++t) {
        int b=rules[a][t].first,c=rules[a][t].second;
        for(int k=l+1;k<r;++k)
            if(lengths[b][k-l] && lengths[c][r-k] && parse(b,l,k) && parse(c,k,r)) {ok=true;break;}
    }
    memo[index]=ok?2:1;
    return ok;
}
int main() {
    string line;
    while(cin>>line && line!="#") {
        int a=line[0]-'A';
        for(string::size_type p=3;p<line.size();) {
            string::size_type q=line.find('|',p);
            if(q==string::npos) q=line.size();
            if(q-p==1) terminal[a][line[p]-'a']=true;
            else rules[a].push_back(make_pair(line[p]-'A',line[p+1]-'A'));
            p=q+1;
        }
    }
    vector<string> words;
    int maximum=0;
    while(cin>>line) {words.push_back(line);maximum=max(maximum,(int)line.size());}
    lengths.assign(26,vector<unsigned char>(maximum+1,0));
    for(int a=0;a<26;++a) for(int c=0;c<26;++c) if(terminal[a][c] && maximum) lengths[a][1]=1;
    for(int len=2;len<=maximum;++len) for(int a=0;a<26;++a)
        for(unsigned int t=0;t<rules[a].size() && !lengths[a][len];++t)
            for(int k=1;k<len;++k) if(lengths[rules[a][t].first][k] && lengths[rules[a][t].second][len-k]) {lengths[a][len]=1;break;}
    for(unsigned int i=0;i<words.size();++i) {
        word=words[i];n=word.size();memo.assign(n*n*26,0);
        cout<<(parse('S'-'A',0,n)?"YES":"NO")<<'\n';
    }
    return 0;
}
