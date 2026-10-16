// POJ 3496 - Word Hyphenation
// Model: gpt-6.1-sol
// A trie matches patterns at every position of a dot-padded word.
// At each gap the largest matched digit wins; odd values allow breaks.
// Exceptions replace all patterns, preserving the input letter cases.
// Breaks are restricted to internal gaps with two letters on either side.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
using namespace std;
struct Node {
    map<char,int> next;
    vector<int> weight;
};
int main() {
    ios::sync_with_stdio(false);
    vector<Node> trie(1);
    map<string, vector<int> > exceptions;
    string s;
    while (cin >> s && s != "#") {
        if (s[0] == '@') {
            string key;
            vector<int> gaps;
            for (size_t i=1;i<s.size();++i) {
                if(s[i]=='-') gaps.push_back((int)key.size());
                else key += s[i];
            }
            exceptions[key]=gaps;
        } else {
            string key;
            vector<int> weights(1,0);
            for(size_t i=0;i<s.size();++i) {
                if(isdigit((unsigned char)s[i])) weights.back()=s[i]-'0';
                else { key+=s[i]; weights.push_back(0); }
            }
            int v=0;
            for(size_t i=0;i<key.size();++i) {
                map<char,int>::iterator it=trie[v].next.find(key[i]);
                if(it==trie[v].next.end()) {
                    int u=(int)trie.size();
                    trie[v].next[key[i]]=u;
                    trie.push_back(Node()); v=u;
                } else v=it->second;
            }
            if(trie[v].weight.empty()) trie[v].weight=weights;
            else for(size_t i=0;i<weights.size();++i)
                trie[v].weight[i]=max(trie[v].weight[i],weights[i]);
        }
    }
    while(cin >> s) {
        string key=s;
        for(size_t i=0;i<key.size();++i) key[i]=(char)tolower((unsigned char)key[i]);
        vector<int> cut(s.size()+1,0);
        map<string,vector<int> >::iterator ex=exceptions.find(key);
        if(ex!=exceptions.end()) {
            for(size_t i=0;i<ex->second.size();++i) cut[ex->second[i]]=1;
        } else if(s.size()>=5) {
            string padded="."+key+".";
            vector<int> value(padded.size()+1,0);
            for(size_t i=0;i<padded.size();++i) {
                int v=0;
                for(size_t j=i;j<padded.size();++j) {
                    map<char,int>::iterator it=trie[v].next.find(padded[j]);
                    if(it==trie[v].next.end()) break;
                    v=it->second;
                    const vector<int>& w=trie[v].weight;
                    for(size_t k=0;k<w.size();++k) value[i+k]=max(value[i+k],w[k]);
                }
            }
            for(size_t i=2;i+2<=s.size();++i) cut[i]=value[i+1]%2;
        }
        for(size_t i=0;i<s.size();++i) {
            if(i && cut[i]) cout << '-';
            cout << s[i];
        }
        cout << '\n';
    }
}
