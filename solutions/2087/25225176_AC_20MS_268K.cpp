// POJ 2087 - Petanque
// Model: gpt-6.1-sol
// Simulate each throw by transferring its remaining travel to the first ball
// on its ray. Ball identities and owners stay fixed when positions change.
// Recompute the closest boule after each throw, including coche displacement.
// Geometric comparisons use the statement's absolute tolerance of 1e-5.
#include <cstdio>
#include <cmath>
const double EPS=1e-5;
struct Ball { double x,y; int owner; } b[7];
double distance(int i) {
    double x=b[i].x-b[0].x, y=b[i].y-b[0].y;
    return sqrt(x*x+y*y);
}
int closest(int n) {
    int k=1;
    for(int i=2;i<n;i++) if(distance(i)<distance(k)-EPS) k=i;
    return k;
}
int main() {
    int cases; scanf("%d",&cases);
    while(cases--) {
        char name[2][21]; scanf("%20s%20s",name[0],name[1]);
        int used[2]={0,0};
        for(int n=0;n<7;n++) {
            int x,y,angle,d; scanf("%d%d%d%d",&x,&y,&angle,&d);
            int owner=-1;
            if(n==1) owner=0;
            else if(n==2) owner=1;
            else if(n>2) {
                if(used[0]==3) owner=1;
                else if(used[1]==3) owner=0;
                else owner=1-b[closest(n)].owner;
            }
            if(owner>=0) used[owner]++;
            b[n].x=x; b[n].y=y; b[n].owner=owner;
            double rad=angle*acos(-1.0)/180.0;
            double dx=cos(rad),dy=sin(rad),left=d;
            int moving=n;
            while(true) {
                int hit=-1; double first=left;
                for(int j=0;j<=n;j++) if(j!=moving) {
                    double vx=b[j].x-b[moving].x,vy=b[j].y-b[moving].y;
                    double along=vx*dx+vy*dy;
                    if(along>EPS && along<first-EPS && fabs(vx*dy-vy*dx)<EPS) {
                        first=along; hit=j;
                    }
                }
                if(hit<0) {
                    b[moving].x+=left*dx; b[moving].y+=left*dy;
                    break;
                }
                b[moving].x=b[hit].x; b[moving].y=b[hit].y;
                left-=first; moving=hit;
            }
        }
        int winner=b[closest(7)].owner,score=0;
        double opponent=1e100;
        for(int i=1;i<7;i++) if(b[i].owner!=winner && distance(i)<opponent) opponent=distance(i);
        for(int i=1;i<7;i++) if(b[i].owner==winner && distance(i)<opponent-EPS) score++;
        printf("%s %d\n",name[winner],score);
    }
    return 0;
}
