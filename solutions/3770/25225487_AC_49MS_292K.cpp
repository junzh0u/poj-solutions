// POJ 3770 - Baseball on the Mars
// Model: gpt-6.1-sol
// Enumerate all orders of throws to occupied destination bases. All runners
// arrive after five seconds, so only throws totaling strictly less than 25
// distance units can eliminate them. Score the surviving bases after every
// bat, including failed bats; returning home runners then leave the field.
#include <iostream>
#include <cmath>
#include <limits>
using namespace std;
const double bx[4]={10,10,0,0}, by[4]={0,10,10,0};
long long k[4], best;
int survivors;
void search(int mask, double x, double y, double length) {
    long long score=0;
    for(int i=0;i<4;++i) if(mask&(1<<i)) score+=k[i];
    if(score<best) { best=score; survivors=mask; }
    for(int i=0;i<4;++i) if(mask&(1<<i)) {
        double dx=x-bx[i], dy=y-by[i];
        double next=length+sqrt(dx*dx+dy*dy);
        if(next<25.0-1e-9) search(mask^(1<<i),bx[i],by[i],next);
    }
}
int main() {
    int n;
    while(cin>>n && n) {
        for(int i=0;i<4;++i) cin>>k[i];
        int state=0;
        for(int z=0;z<n;++z) {
            double x,y; cin>>x>>y;
            if(x>=0 && y>=0) {
                best=numeric_limits<long long>::max();
                search((state<<1)|1,x,y,0);
                state=survivors;
            } else {
                best=0;
                for(int i=0;i<3;++i) if(state&(1<<i)) best+=k[i];
            }
            cout<<best<<'\n';
            state &= 7;
        }
        cout<<'\n';
    }
    return 0;
}
