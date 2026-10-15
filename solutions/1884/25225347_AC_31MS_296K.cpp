// POJ 1884 - Train Time
// Model: gpt-6.1-sol
// Build both timetables, checking overlapping station visits first.
// Otherwise bisect the monotone separation using exact motion on each leg.
// Both trains depart at time zero; there is no initial station wait.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
struct Train {
    vector<double> x, arrival, departure;
    double v, a;
    double position(double t) const {
        int n = (int)x.size();
        for (int i=0; i<n-1; ++i) {
            if (t <= departure[i]) return x[i];
            if (t <= arrival[i+1]) {
                double u=t-departure[i], length=fabs(x[i+1]-x[i]);
                double ramp=v/a, duration=arrival[i+1]-departure[i];
                double d;
                if (u<ramp) d=a*u*u/2;
                else if (u>duration-ramp) {
                    double left=duration-u;
                    d=length-a*left*left/2;
                } else d=v*(u-ramp/2);
                return x[i]+(x[i+1]>x[i] ? d : -d);
            }
        }
        return x[n-1];
    }
    void build(double speed, double accel, double wait) {
        v=speed; a=accel;
        arrival.assign(x.size(),0); departure.assign(x.size(),0);
        for (int i=1; i<(int)x.size(); ++i) {
            arrival[i]=departure[i-1]+fabs(x[i]-x[i-1])/v+v/a;
            departure[i]=arrival[i]+wait;
        }
    }
};
int main() {
    double d,v,a,m; int scenario=0;
    while (scanf("%lf",&d)==1 && d>=0) {
        Train left,right; left.x.push_back(0);
        while(d>0) { left.x.push_back(d*5280); scanf("%lf",&d); }
        scanf("%lf%lf%lf",&v,&a,&m);
        right.x.assign(left.x.rbegin(),left.x.rend());
        left.build(v,a,m); right.build(v,a,m);
        int n=(int)left.x.size()-1, station=-1;
        double time=0,place=0;
        for(int i=1;i<n;++i) {
            double start=max(left.arrival[i],right.arrival[n-i]);
            double end=min(left.departure[i],right.departure[n-i]);
            if(start<=end+1e-9) { station=i;time=start;place=left.x[i];break; }
        }
        if(station<0) {
            double lo=0,hi=left.arrival[n];
            for(int it=0;it<100;++it) {
                double mid=(lo+hi)/2;
                if(left.position(mid)<right.position(mid)) lo=mid;
                else hi=mid;
            }
            time=(lo+hi)/2; place=(left.position(time)+right.position(time))/2;
        }
        printf("Scenario #%d\n   Meeting time: %.1f minutes\n",++scenario,time);
        printf("   Meeting distance: %.3f miles from metro center hub",place/5280);
        if(station>=0) printf(", in station %d",station);
        printf("\n\n");
    }
    return 0;
}
