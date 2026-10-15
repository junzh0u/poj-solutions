// POJ 1532 - Mixed Numbers
// Model: gpt-6.1-sol
// Parse the clockwise-rotated drawing recursively. A leading digit fixes
// the baseline; each following vertical bar bounds numerator on its right
// and denominator on its left. Sum terms along the baseline.
// Preserve spaces inside figures; tolerate empty separator lines and EOF.
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
using namespace std;
vector<string> g;
double eval(int a, int b, int l, int r) {
    int x=a, y=-1;
    for (; x<=b; ++x) {
        for (int j=l;j<=r;++j) if (g[x][j]!=' ') { y=j; break; }
        if (y!=-1) break;
    }
    if (y==-1) return 0;
    double sum=0;
    while (x<=b) {
        if (g[x][y]>='1' && g[x][y]<='9') {
            double term=g[x][y]-'0';
            ++x;
            if (x<=b && g[x][y]=='|') {
                int first=x;
                while (x<=b && g[x][y]=='|') ++x;
                term+=eval(first,x-1,y+1,r)/eval(first,x-1,l,y-1);
            }
            sum+=term;
        } else ++x;
    }
    return sum;
}
int main() {
    int n,m;
    while (cin>>n>>m && (n||m)) {
        cin.ignore(numeric_limits<streamsize>::max(),'\n');
        g.clear(); string s;
        while ((int)g.size()<m && getline(cin,s)) {
            if (!s.empty() && s[s.size()-1]=='\r') s.erase(s.size()-1);
            if (s.empty()) continue;
            s.resize(n,' '); g.push_back(s);
        }
        if ((int)g.size()!=m) break;
        cout<<fixed<<setprecision(2)<<eval(0,m-1,0,n-1)<<'\n';
    }
}
