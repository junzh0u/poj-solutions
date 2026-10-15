// POJ 2930 - Dialing Dice
// Model: gpt-6.1-sol
// Enumerate unordered triples of unordered opposite-face pairs. For each
// layout, edit-distance DP tracks the last bottom face; insertion closure
// uses the cube face graph's diameter two. Digits absent from the target
// can all be replaced by zero without increasing cost, improving ties.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
struct Pair { int a,b; Pair(int x,int y):a(x),b(y){} };
int score(const string &s, int d[6]) {
    int old[6]={0,0,0,0,0,0}, cur[6], nxt[6];
    for (int i=0;i<(int)s.size();++i) {
        int low=1000;
        for(int f=0;f<6;++f) {
            int v=old[f]+1;
            for(int g=0;g<6;++g) if(f/2!=g/2)
                v=min(v,old[g]+(d[f]!=s[i]-'0'));
            cur[f]=v; low=min(low,v);
        }
        for(int f=0;f<6;++f) {
            int v=min(cur[f],low+2);
            for(int g=0;g<6;++g) if(f/2!=g/2) v=min(v,cur[g]+1);
            nxt[f]=v;
        }
        for(int f=0;f<6;++f) old[f]=nxt[f];
    }
    return *min_element(old,old+6);
}
int main() {
    string s; int tc=0;
    while(cin>>s) {
        bool used[10]={true,false,false,false,false,false,false,false,false,false};
        for(int i=0;i<(int)s.size();++i) used[s[i]-'0']=true;
        vector<Pair> p;
        for(int a=0;a<10;++a) if(used[a])
            for(int b=a;b<10;++b) if(used[b]) p.push_back(Pair(a,b));
        int best=1000; vector<int> ans(6,9);
        for(int a=0;a<(int)p.size();++a)
            for(int b=a;b<(int)p.size();++b)
                for(int c=b;c<(int)p.size();++c) {
                    int d[6]={p[a].a,p[a].b,p[b].a,p[b].b,p[c].a,p[c].b};
                    int cost=score(s,d);
                    if(cost>best) continue;
                    vector<int> v(d,d+6); sort(v.begin(),v.end());
                    if(cost<best || v<ans) {best=cost;ans=v;}
                }
        cout<<"Dice "<<++tc<<": discrepancy is "<<best<<", digits used:";
        for(int i=0;i<6;++i) cout<<' '<<ans[i];
        cout<<'\n';
    }
}
