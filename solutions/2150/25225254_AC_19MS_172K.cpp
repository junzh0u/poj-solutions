// POJ 2150 - Crossing Prisms
// Model: gpt-6.1-sol
// Integrate the lateral faces over slabs between consecutive vertex heights.
// A horizontal section is S x S, even when S has several intervals.
// Horizontal faces are the symmetric difference of the limiting sections;
// this counts coincident faces of the two prisms only once.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
struct Point { double x,z; };
typedef vector<double> Slice;
Point p[4]; int n;
Slice section(double probe, double z, double &factor) {
    Slice s; factor=0;
    for(int i=0;i<n;++i) {
        Point a=p[i], b=p[(i+1)%n];
        if(probe>min(a.z,b.z) && probe<max(a.z,b.z)) {
            double slope=(b.x-a.x)/(b.z-a.z);
            s.push_back(a.x+(z-a.z)*slope);
            factor+=sqrt(1+slope*slope);
        }
    }
    sort(s.begin(),s.end()); return s;
}
double width(const Slice &s) {
    double w=0; for(unsigned i=0;i+1<s.size();i+=2) w+=s[i+1]-s[i];
    return w;
}
double common(const Slice &a,const Slice &b) {
    double w=0;
    for(unsigned i=0;i+1<a.size();i+=2)
        for(unsigned j=0;j+1<b.size();j+=2)
            w+=max(0.0,min(a[i+1],b[j+1])-max(a[i],b[j]));
    return w;
}
int main() {
    while(scanf("%d",&n)==1 && n) {
        vector<double> levels;
        for(int i=0;i<n;++i) { scanf("%lf%lf",&p[i].x,&p[i].z); levels.push_back(p[i].z); }
        sort(levels.begin(),levels.end());
        levels.erase(unique(levels.begin(),levels.end()),levels.end());
        double area=0,f;
        for(unsigned i=0;i+1<levels.size();++i) {
            double lo=levels[i],hi=levels[i+1],mid=(lo+hi)/2;
            double w=width(section(mid,mid,f));
            area+=2*w*f*(hi-lo);
        }
        for(unsigned i=0;i<levels.size();++i) {
            double z=levels[i]; Slice a,b;
            if(i) a=section((levels[i-1]+z)/2,z,f);
            if(i+1<levels.size()) b=section((z+levels[i+1])/2,z,f);
            double u=width(a),v=width(b),c=common(a,b);
            area+=u*u+v*v-2*c*c;
        }
        printf("%.4f\n",area);
    }
    return 0;
}
