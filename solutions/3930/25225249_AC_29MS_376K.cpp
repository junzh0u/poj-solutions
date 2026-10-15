// POJ 3930 - Elevator
// Model: gpt-6.1-sol
// Simulate each second, admitting requests before choosing the next action.
// Keep a direction while passengers or hall calls require travel ahead;
// reverse before boarding at the end of a sweep. Door actions take one second.
#include <cstdio>
#include <vector>
using namespace std;
struct Request { int t,s,d,state; };
vector<Request> a;
int now, floorNow, dir;
void stamp() { printf("%02d:%02d ",now/60,now%60); }
int requestDir(const Request &r) { return r.d>=r.s?1:-1; }
bool waiting(int k) { return a[k].state==0 && a[k].t<=now; }
bool ahead(int direction) {
    for(int k=0;k<(int)a.size();++k) {
        if(a[k].state==1 && a[k].d!=floorNow) return true;
        if(waiting(k) && ((a[k].s-floorNow)*direction>0 ||
            (a[k].s==floorNow && requestDir(a[k])==direction))) return true;
    }
    return false;
}
void choose() {
    if(dir && ahead(dir)) return;
    if(dir && ahead(-dir)) { dir=-dir; return; }
    int best=-1, priority=-1;
    for(int k=0;k<(int)a.size();++k) if(waiting(k)) {
        int p=a[k].s==floorNow ? 2+(requestDir(a[k])==1) : (a[k].s>floorNow?1:0);
        if(best<0 || a[k].t<a[best].t || (a[k].t==a[best].t && p>priority)) {
            best=k; priority=p;
        }
    }
    if(best<0) dir=0;
    else dir=a[best].s==floorNow?requestDir(a[best]):(a[best].s>floorNow?1:-1);
}
int exiting() { int c=0; for(int k=0;k<(int)a.size();++k) if(a[k].state==1 && a[k].d==floorNow) ++c; return c; }
int entering() { int c=0; for(int k=0;k<(int)a.size();++k) if(waiting(k)&&a[k].s==floorNow&&requestDir(a[k])==dir) ++c; return c; }
int main() {
    int T; scanf("%d",&T);
    for(int tc=1;tc<=T;++tc) {
        int n; scanf("%d%d",&floorNow,&n); a.resize(n);
        for(int k=0;k<n;++k) { scanf("%d%d%d",&a[k].t,&a[k].s,&a[k].d); a[k].state=0; }
        now=0; dir=0; bool moving=false; int done=0;
        printf("Case %d:\n",tc);
        while(done<n) {
            choose();
            if(exiting() || entering()) {
                if(moving) { stamp(); printf("The elevator stops at floor %d.\n",floorNow); moving=false; }
                stamp(); puts("The elevator door is opening."); ++now;
                while(true) {
                    int c=exiting();
                    if(c) {
                        stamp(); printf("%d people leave the elevator.\n",c);
                        for(int k=0;k<n;++k) if(a[k].state==1&&a[k].d==floorNow) { a[k].state=2; ++done; }
                        ++now; continue;
                    }
                    choose(); c=entering();
                    if(!c) break;
                    stamp(); printf("%d people enter the elevator.\n",c);
                    for(int k=0;k<n;++k) if(waiting(k)&&a[k].s==floorNow&&requestDir(a[k])==dir) a[k].state=1;
                    ++now;
                }
                stamp(); puts("The elevator door is closing."); ++now;
            } else if(dir) {
                if(!moving) { stamp(); printf("The elevator starts to move %s from floor %d.\n",dir==1?"up":"down",floorNow); moving=true; }
                floorNow+=dir; ++now;
            } else { moving=false; ++now; }
        }
        puts("");
    }
    return 0;
}
