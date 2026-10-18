// POJ 3355 - Boundary Points
// Model: gpt-6-astra
// Monotone-chain convex hull, removing collinear intermediate points.
// Each input line is one case. Keep original coordinate strings, as the
// contest clarification requires their exact spelling in the output.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;
struct Point { long double x,y; string text; };
bool lessPoint(const Point&a,const Point&b) {
    return a.x<b.x || (a.x==b.x && a.y<b.y);
}
bool equalPoint(const Point&a,const Point&b) {return a.x==b.x && a.y==b.y;}
long double cross(const Point&a,const Point&b,const Point&c) {
    return (b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x);
}
int main() {
    string line;
    while(getline(cin,line)) {
        vector<Point> p;
        string::size_type pos=0,l,r,comma;
        while((l=line.find('(',pos))!=string::npos) {
            r=line.find(')',l); comma=line.find(',',l);
            if(r==string::npos || comma==string::npos || comma>r) break;
            Point a;
            a.x=strtold(line.substr(l+1,comma-l-1).c_str(),0);
            a.y=strtold(line.substr(comma+1,r-comma-1).c_str(),0);
            a.text=line.substr(l,r-l+1);
            p.push_back(a); pos=r+1;
        }
        if(p.empty()) continue;
        sort(p.begin(),p.end(),lessPoint);
        p.erase(unique(p.begin(),p.end(),equalPoint),p.end());
        vector<Point> h(2*p.size()+1);
        int n=(int)p.size(), k=0;
        for(int i=0;i<n;++i) {
            while(k>=2 && cross(h[k-2],h[k-1],p[i])<=0) --k;
            h[k++]=p[i];
        }
        int lower=k;
        for(int i=n-2;i>=0;--i) {
            while(k>lower && cross(h[k-2],h[k-1],p[i])<=0) --k;
            h[k++]=p[i];
        }
        if(n==1) h[k++]=p[0];
        --k; // Closing vertex is emitted after rotating to the lowest point.
        int start=0;
        for(int i=1;i<k;++i)
            if(h[i].y<h[start].y || (h[i].y==h[start].y && h[i].x<h[start].x)) start=i;
        for(int i=0;i<=k;++i) {
            if(i) cout << ' ';
            cout << h[(start+i)%k].text;
        }
        cout << '\n';
    }
    return 0;
}
