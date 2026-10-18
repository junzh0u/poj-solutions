// POJ 3382 - Deciphering
// Model: gpt-6-astra
// A dictionary trie finds words; suffix DP follows each sentence rule.
// Distinct rule/part-of-speech interpretations count separately, even when
// they print the same sentence. Counts saturate strictly above 10^18.
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstring>
using namespace std;
struct Count {
    int hi, lo;
    Count(int x=0):hi(0),lo(x){}
    bool positive() const { return hi || lo; }
    void add(const Count &b) {
        if (hi==1000000001) return;
        if (b.hi==1000000001) {hi=1000000001;lo=0;return;}
        lo+=b.lo; hi+=b.hi;
        if(lo>=1000000000){lo-=1000000000;++hi;}
        if(hi>1000000000 || (hi==1000000000 && lo)) {hi=1000000001;lo=0;}
    }
};
struct Node { int ch[26], mask; Node():mask(0){memset(ch,0,sizeof(ch));} };
struct Match {int end,mask; Match(int e,int m):end(e),mask(m){}};
Count dp[1001][101];
int need[101], nxt[101], takeEnd[1001][101], takeState[1001][101];
int main(){
    int n,m,k; if(!(cin>>n>>m>>k))return 0;
    vector<Node> tr(1);
    for(int i=0;i<n;++i){
        string w; int c; cin>>w>>c; int p=0;
        for(int j=0;j<(int)w.size();++j){int x=w[j]-'a'; if(!tr[p].ch[x]){int q=tr.size();tr[p].ch[x]=q;tr.push_back(Node());} p=tr[p].ch[x];}
        for(int j=0;j<c;++j){int a;cin>>a;tr[p].mask|=1<<(a-1);}
    }
    int states=1; vector<int> starts;
    for(int i=0;i<m;++i){int len;cin>>len;starts.push_back(states);for(int j=0;j<len;++j){int a;cin>>a;need[states]=1<<(a-1);nxt[states]=(j+1==len?0:states+1);++states;}}
    string text;cin>>text;int L=text.size();dp[L][0]=Count(1);
    for(int i=L-1;i>=0;--i){
        vector<Match> matches;int p=0;
        for(int j=i;j<L;++j){p=tr[p].ch[text[j]-'a'];if(!p)break;if(tr[p].mask)matches.push_back(Match(j+1,tr[p].mask));}
        for(int s=1;s<states;++s)for(int t=0;t<(int)matches.size();++t){
            int e=matches[t].end;
            if((matches[t].mask&need[s]) && dp[e][nxt[s]].positive()){
                dp[i][s].add(dp[e][nxt[s]]);takeEnd[i][s]=e;takeState[i][s]=nxt[s];
            }
        }
        for(int r=0;r<m;++r){int s=starts[r];dp[i][0].add(dp[i][s]);if(dp[i][s].positive()){takeEnd[i][0]=takeEnd[i][s];takeState[i][0]=takeState[i][s];}}
    }
    Count ans=dp[0][0];
    if(ans.hi>1000000000)cout<<"TOO MANY\n";
    else if(ans.hi)cout<<ans.hi<<setw(9)<<setfill('0')<<ans.lo<<'\n';
    else cout<<ans.lo<<'\n';
    if(ans.positive()){
        int p=0,s=0;
        while(p<L){int e=takeEnd[p][s],q=takeState[p][s];cout<<text.substr(p,e-p);if(q==0)cout<<'.';if(e<L)cout<<' ';p=e;s=q;}cout<<'\n';
    }
    return 0;
}
