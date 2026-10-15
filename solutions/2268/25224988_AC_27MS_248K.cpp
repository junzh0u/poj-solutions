// POJ 2268 - Euro Cup 2000
// Model: gpt-6.1-sol
// Enumerate the three point outcomes of each remaining game.
// For best/worst ranks, count equal-point teams behind/ahead respectively.
// This is attainable: changing all of a team's remaining games to wins/losses
// cannot worsen/improve its point rank, and arbitrarily large goal margins
// give it the best/worst goal difference among teams still tied on points.
// The guarantee that every team has a remaining game is essential here.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
using namespace std;
int n;
vector<int> points, bestRank, worstRank;
vector<pair<int,int> > games;
void search(int k) {
    if (k == (int)games.size()) {
        for (int i=0;i<n;++i) {
            int better=1, tied=0;
            for (int j=0;j<n;++j) if (j!=i) {
                if (points[j]>points[i]) ++better;
                else if (points[j]==points[i]) ++tied;
            }
            bestRank[i]=min(bestRank[i],better);
            worstRank[i]=max(worstRank[i],better+tied);
        }
        return;
    }
    int a=games[k].first,b=games[k].second;
    points[a]+=3; search(k+1); points[a]-=3;
    ++points[a]; ++points[b]; search(k+1); --points[a]; --points[b];
    points[b]+=3; search(k+1); points[b]-=3;
}
int main() {
    int tc=0;
    while (cin>>n && n) {
        vector<string> names(n);
        map<string,int> id;
        for (int i=0;i<n;++i) { cin>>names[i]; id[names[i]]=i; }
        vector<vector<int> > remaining(n,vector<int>(n,2));
        points.assign(n,0); bestRank.assign(n,n); worstRank.assign(n,1);
        int g; cin>>g;
        for (int i=0;i<g;++i) {
            string s,t; int x,y; cin>>s>>t>>x>>y;
            int a=id[s],b=id[t]; --remaining[a][b]; --remaining[b][a];
            if (x>y) points[a]+=3;
            else if (x<y) points[b]+=3;
            else { ++points[a]; ++points[b]; }
        }
        games.clear();
        for (int i=0;i<n;++i) for (int j=i+1;j<n;++j)
            for (int k=0;k<remaining[i][j];++k) games.push_back(make_pair(i,j));
        search(0);
        cout<<"Group #"<<++tc<<'\n';
        for (int i=0;i<n;++i) cout<<names[i]<<' '<<bestRank[i]<<'-'<<worstRank[i]<<'\n';
        cout<<'\n';
    }
    return 0;
}
