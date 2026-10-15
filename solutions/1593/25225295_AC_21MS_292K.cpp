// POJ 1593 - Radar Scopes
// Model: gpt-6.1-sol
// Match numeric squawk identifiers and compare Cartesian displacement over five
// seconds with the mean reported speed. The 10% equipment tolerance is relative
// to the radar-derived speed; missing planes use the shortest radial boundary
// distance. Preserve squawk spelling (including sample-leading zeroes).
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <cmath>
#include <cstdlib>
using namespace std;
struct Plane { string label; double x,y,d,s; };
typedef map<int,Plane> Sweep;
void readSweep(Sweep &p,int n) {
    for(int i=0;i<n;++i) {
        Plane q; double a;
        cin>>q.label>>a>>q.d>>q.s;
        a*=acos(-1.0)/180.0;
        q.x=q.d*sin(a); q.y=q.d*cos(a);
        p[atoi(q.label.c_str())]=q;
    }
}
int main() {
    int n,sc=0;
    while(cin>>n) {
        Sweep a,b,all; readSweep(a,n); cin>>n; readSweep(b,n);
        all=a; all.insert(b.begin(),b.end());
        cout<<"Scenario # "<<++sc<<'\n';
        for(Sweep::iterator it=all.begin();it!=all.end();++it) {
            int id=it->first; string warning;
            if(a.count(id) && b.count(id)) {
                const Plane &p=a[id], &q=b[id];
                double dx=p.x-q.x,dy=p.y-q.y;
                double radar=sqrt(dx*dx+dy*dy)*720.0;
                double mean=(p.s+q.s)/2.0;
                if(fabs(mean-radar)>radar*0.1+1e-8)
                    warning="equipment warning";
            } else {
                const Plane &p=it->second;
                bool reaches=(p.s*1.1+1e-8 >= (10.0-p.d)*720.0);
                if(a.count(id)) warning=reaches?"domain exited":"domain loss";
                else warning=reaches?"new intrusion":"new aloft";
            }
            if(!warning.empty()) cout<<setw(5)<<it->second.label<<" -- "<<warning<<'\n';
        }
        cout<<'\n';
    }
    return 0;
}
