// POJ 1449 - Enigma
// Model: gpt-6.1-sol
// Complete missing permutation entries and initial displacements by enumeration.
// Decode with conjugated rotor permutations and check the known plaintext.
// Carries occur after full revolutions measured from the initial positions,
// not whenever the displayed displacement wraps from z to a.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
string key[6], plain, cipher, answer;
vector<pair<int,int> > holes;
int forwardMap[5][26], backwardMap[5][26];
bool check() {
    for (int r=0;r<5;++r) {
        bool used[26]={false};
        for(int i=0;i<26;++i) {
            int v=key[r][i]-'a';
            if(used[v]) return false;
            used[v]=true;
            forwardMap[r][i]=v;
            backwardMap[r][v]=i;
        }
    }
    for(int i=0;i<26;++i)
        if(forwardMap[3][forwardMap[3][i]]!=i) return false;
    string result;
    for(int t=0;t<(int)cipher.size();++t) {
        int k[4], n=t;
        for(int r=0;r<4;++r) { k[r]=(key[5][r]-'a'+n)%26; n/=26; }
        int x=forwardMap[4][cipher[t]-'a'];
        for(int r=0;r<4;++r) x=(forwardMap[r][(x+k[r])%26]-k[r]+26)%26;
        for(int r=2;r>=0;--r) x=(backwardMap[r][(x+k[r])%26]-k[r]+26)%26;
        x=backwardMap[4][x];
        if(plain[t]!='?' && plain[t]-'a'!=x) return false;
        result+=char('a'+x);
    }
    answer=result;
    return true;
}
bool search(int h) {
    if(h==(int)holes.size()) return check();
    int r=holes[h].first, p=holes[h].second;
    bool used[26]={false};
    if(r<5) for(int i=0;i<26;++i) if(key[r][i]!='?') used[key[r][i]-'a']=true;
    for(int v=0;v<26;++v) if(!used[v]) {
        key[r][p]=char('a'+v);
        if(search(h+1)) return true;
    }
    key[r][p]='?';
    return false;
}
int main() {
    int cases; cin>>cases;
    for(int c=1;c<=cases;++c) {
        holes.clear();
        for(int r=0;r<6;++r) {
            cin>>key[r];
            for(int p=0;p<(int)key[r].size();++p)
                if(key[r][p]=='?') holes.push_back(make_pair(r,p));
        }
        cin>>plain>>cipher;
        search(0);
        cout<<"Scenario #"<<c<<":\n"<<answer<<"\n\n";
    }
}
