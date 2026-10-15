// POJ 2223 - Get Out 'Da Way!
// Model: gpt-6.1-sol
// Intersect each bullet with the target plane in relative coordinates.
// Use the sample's miss-message capitalization; discussion specifies padding
// every hit-target row to the maximum input width.
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>
#include <cctype>
using namespace std;
struct Velocity { double n,e,u; };
Velocity readVelocity() {
    string s; getline(cin,s); if (!s.empty() && s[s.size()-1]=='\r') s.erase(s.size()-1);
    Velocity v; sscanf(s.c_str(),"%lf,%lf,%lf",&v.n,&v.e,&v.u); return v;
}
int main() {
    string s; bool first=true;
    while (getline(cin,s)) {
        if (!s.empty() && s[s.size()-1]=='\r') s.erase(s.size()-1);
        int n; if (sscanf(s.c_str(),"START %d",&n)!=1) continue;
        Velocity target=readVelocity(); vector<Velocity> bullets(n);
        for (int i=0;i<n;++i) bullets[i]=readVelocity();
        vector<string> grid; int w=0;
        while (getline(cin,s)) {
            if (!s.empty() && s[s.size()-1]=='\r') s.erase(s.size()-1);
            if (s=="END") break;
            grid.push_back(s); if ((int)s.size()>w) w=s.size();
        }
        int h=grid.size();
        for (int r=0;r<h;++r) grid[r].resize(w,' ');
        bool hit=false;
        for (int i=0;i<n;++i) {
            double dn=bullets[i].n-target.n;
            if (dn<=0) continue;
            double t=10.0/dn;
            double x=w/2.0+10.0*(bullets[i].e-target.e)*t;
            double y=h/2.0-10.0*(bullets[i].u-target.u)*t;
            if (x<0 || x>=w || y<0 || y>=h) continue;
            int c=(int)floor(x),r=(int)floor(y);
            if (!isspace((unsigned char)grid[r][c])) { grid[r][c]='*'; hit=true; }
        }
        if (!first) cout << '\n'; first=false;
        if (!hit) cout << "Got Out Da Way!\n";
        else for (int r=0;r<h;++r) cout << grid[r] << '\n';
    }
    return 0;
}
