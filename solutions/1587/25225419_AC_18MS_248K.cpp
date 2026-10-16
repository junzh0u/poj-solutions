// POJ 1587 - High and Dry
// Model: gpt-6.1-sol
// Layered dynamic programming minimizes days, then maximizes successive docks.
// Tide intervals repeat across midnight; waiting offshore before docking is allowed.
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
using namespace std;
const double EPS=1e-7, INF=1e100;
double clockval() { string s; cin>>s; int h,m,t; char c; istringstream in(s); in>>h>>c>>m>>c>>t; return h*3600.0+m*60+t; }
double accessible(double t,double low,double period,double h) {
    if(h==0) return t;
    if(2*h>period+EPS) return INF;
    double r=t-low-floor((t-low)/period)*period;
    if(r<h-EPS) return t+h-r;
    if(r>period-h+EPS) return t+period+h-r;
    return t;
}
int main() {
    int days;
    while(cin>>days && days) {
        double speed; cin>>speed; speed/=3600;
        double rise=clockval(),ri=clockval();
        double set=clockval(),si=clockval();
        double low=clockval(),period=clockval();
        int n; cin>>n;
        vector<double> x(n+1),h(n+1);
        for(int i=0;i<=n;++i) { cin>>x[i]>>h[i]; h[i]*=3600; }
        vector< vector<int> > best(n+1);
        vector<int> reached(n+1,0); reached[0]=1;
        bool done=false;
        for(int d=0;d<days && !done;++d) {
            vector< vector<int> > next(n+1);
            vector<int> have(n+1,0);
            for(int i=0;i<=n;++i) if(reached[i]) {
                double depart=accessible(rise+d*ri,low,period,h[i]);
                for(int j=i;j<=n;++j) {
                    bool ok=(j==i);
                    if(j>i && depart<=set+d*si+EPS) {
                        double arrive=accessible(depart+(x[j]-x[i])/speed,low,period,h[j]);
                        ok=arrive<=set+d*si+EPS;
                    }
                    if(ok) {
                        vector<int> path=best[i]; path.push_back(j);
                        if(!have[j] || next[j]<path) { have[j]=1; next[j]=path; }
                    }
                }
            }
            reached.swap(have); best.swap(next);
            if(reached[n]) {
                for(unsigned i=0;i<best[n].size();++i) { if(i) cout<<' '; cout<<best[n][i]; }
                cout<<'\n'; done=true;
            }
        }
        if(!done) cout<<"NO ITINERARY POSSIBLE\n";
    }
}
