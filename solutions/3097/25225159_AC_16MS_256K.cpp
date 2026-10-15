// POJ 3097 - Falling Ice
// Model: gpt-6.1-sol
// Maintain the exposed boundary from the left wall to the right wall.
// Enumerate supporting pairs from outside inward, rejecting overlaps and
// reversed boundary arcs; a valid pair blocks all nested pairs below it.
// Choose the lowest reachable center, breaking floor ties to the left.
// Check all old disks and box bounds as well as boundary orientation.
// Reachability matters: empty pockets sealed by a larger disk are excluded.
#include <cstdio>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
const double EPS=1e-9;
struct Circle {
    double x,y,r;
    int wall; // 0: disk, 1: left, 2: floor, 3: right
    Circle(double X=0,double Y=0,double R=0,int W=0):x(X),y(Y),r(R),wall(W){}
};
double width;
double clearance(const Circle &a,const Circle &b) {
    if(a.wall==1) {
        if(b.wall==3) return width;
        if(b.wall==2) return 0;
        return b.x-b.r;
    }
    if(a.wall==2) return 1e100;
    if(b.wall==3) return width-a.x-a.r;
    if(b.wall==2) return a.y-a.r;
    return sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y))-a.r-b.r;
}
Circle resting(Circle a,Circle b,double r) {
    if(a.wall==1) {
        if(b.wall==2) return Circle(r,r,r);
        double dx=b.x-r, R=b.r+r;
        return Circle(r,b.y+sqrt(max(0.0,R*R-dx*dx)),r);
    }
    if(b.wall==3) {
        a.x=width-a.x;
        Circle c=resting(Circle(0,0,0,1),a,r);
        c.x=width-c.x;
        return c;
    }
    if(b.wall==2) {
        swap(a.x,a.y);
        Circle c=resting(Circle(0,0,0,1),a,r);
        swap(c.x,c.y);
        return c;
    }
    double dx=b.x-a.x,dy=b.y-a.y,D=sqrt(dx*dx+dy*dy);
    double R=a.r+r,S=b.r+r,E=(R*R-S*S+D*D)/(2*D);
    double F=sqrt(max(0.0,R*R-E*E));
    return Circle(a.x+(E*dx-F*dy)/D,a.y+(E*dy+F*dx)/D,r);
}
void vectorTo(const Circle &c,const Circle &b,double &x,double &y) {
    if(b.wall) { x=b.x; y=b.y; }
    else { x=b.x-c.x; y=b.y-c.y; }
}
bool reversed(const Circle &c,const Circle &left,const Circle &mid) {
    double ax,ay,bx,by;
    vectorTo(c,left,ax,ay); vectorTo(c,mid,bx,by);
    return ax*by-ay*bx < -EPS;
}
double badness(const Circle &a) {
    if(a.y>a.r) return a.y+width;
    return 2*a.r+a.x;
}
bool better(const Circle &a,const Circle &b) { return badness(a)<badness(b); }
int main() {
    int n;
    while(scanf("%lf",&width)==1 && width>0) {
        scanf("%d",&n);
        vector<Circle> boundary;
        boundary.push_back(Circle(-1,0,0,1));
        boundary.push_back(Circle(0,-1,0,2));
        boundary.push_back(Circle(0,1,0,3));
        double height=0;
        vector<Circle> placed;
        for(int z=0;z<n;++z) {
            double d; scanf("%lf",&d); double r=d/2;
            int m=boundary.size(),lo=0,hi=0;
            bool blocked[13][13]={{false}};
            Circle best(0,1e100,r);
            for(int gap=m-2;gap>=1;--gap) for(int l=0;l+gap<m;++l) {
                int h=l+gap;
                if(blocked[l][h] || clearance(boundary[l],boundary[h])>2*r+EPS) continue;
                Circle c=resting(boundary[l],boundary[h],r);
                bool valid=c.x>=r-EPS && c.x<=width-r+EPS && c.y>=r-EPS;
                for(unsigned j=0;j<placed.size();++j) if(clearance(c,placed[j]) < -EPS) valid=false;
                for(int j=l+1;j<h;++j) {
                    if(clearance(c,boundary[j]) < -EPS || reversed(c,boundary[l],boundary[j])) {
                        valid=false; break;
                    }
                }
                if(!valid) continue;
                if(better(c,best)) { best=c; lo=l; hi=h; }
                for(int j=l;j<h;++j) for(int k=j+1;k<=h;++k) blocked[j][k]=true;
            }
            boundary.erase(boundary.begin()+lo+1,boundary.begin()+hi);
            boundary.insert(boundary.begin()+lo+1,best);
            height=max(height,best.y+r);
            placed.push_back(best);
        }
        printf("%.2f\n",height);
    }
    return 0;
}
