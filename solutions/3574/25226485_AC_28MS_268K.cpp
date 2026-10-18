// POJ 3574 - Japanese Writing
// Model: gpt-6-astra
// Compress x and y coordinates independently, preserving equalities and order.
// The eight relative directions are exactly the signs of coordinate differences.
// Sorted directed strokes in these canonical coordinates determine the shape.
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
vector<vector<int> > readShape() {
    int m; cin >> m;
    vector<vector<int> > s(m, vector<int>(4));
    vector<int> x,y;
    for(int i=0;i<m;++i) {
        for(int j=0;j<4;++j) cin >> s[i][j];
        x.push_back(s[i][0]); x.push_back(s[i][2]);
        y.push_back(s[i][1]); y.push_back(s[i][3]);
    }
    sort(x.begin(),x.end()); x.erase(unique(x.begin(),x.end()),x.end());
    sort(y.begin(),y.end()); y.erase(unique(y.begin(),y.end()),y.end());
    for(int i=0;i<m;++i) for(int j=0;j<4;++j) {
        const vector<int>& a = j%2 ? y : x;
        s[i][j]=lower_bound(a.begin(),a.end(),s[i][j])-a.begin();
    }
    sort(s.begin(),s.end());
    return s;
}
int main() {
    int n; if(!(cin >> n)) return 0;
    vector<vector<int> > base=readShape();
    for(int i=1;i<n;++i) cout << (readShape()==base ? "CORRECT" : "INCORRECT") << '\n';
    return 0;
}
