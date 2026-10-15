// POJ 1957 - Beehives
// Model: gpt-6.1-sol
// Compare occupied hexagonal-grid cell sets under six rotations and translation.
// The traversal order is irrelevant; reflections are explicitly not permitted.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
using namespace std;
typedef pair<int,int> Point;
vector<Point> cells(const string &s, int rotation) {
    const int dx[6] = {1,0,-1,-1,0,1};
    const int dy[6] = {0,1,1,0,-1,-1};
    vector<Point> p;
    int x=0,y=0;
    p.push_back(Point(x,y));
    for (size_t i=0;i<s.size();++i) {
        int d=(s[i]-'a'+rotation)%6;
        x+=dx[d]; y+=dy[d];
        p.push_back(Point(x,y));
    }
    sort(p.begin(),p.end());
    Point origin=p[0];
    for (size_t i=0;i<p.size();++i) {
        p[i].first-=origin.first;
        p[i].second-=origin.second;
    }
    return p;
}
void readLine(string &s) {
    getline(cin,s);
    if (!s.empty() && s[s.size()-1]=='\r') s.erase(s.size()-1);
}
int main() {
    int cases;
    cin>>cases;
    string a,b,separator;
    getline(cin,separator);
    for (int t=0;t<cases;++t) {
        readLine(a); readLine(b); readLine(separator);
        bool same=false;
        if (a.size()==b.size()) {
            vector<Point> target=cells(a,0);
            for (int r=0;r<6;++r)
                if (target==cells(b,r)) same=true;
        }
        cout<<(same ? "true" : "false")<<'\n';
    }
    return 0;
}
