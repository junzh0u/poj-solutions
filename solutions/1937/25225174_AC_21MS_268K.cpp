// POJ 1937 - Balanced Food
// Model: gpt-6.1-sol
// Average equal-area sector centroids and test the convex table sector.
// Subset DP finds whether each remainder can be eaten completely;
// ascending reconstruction gives the first ordering. The statement
// guarantees connectivity regardless of which slices have been removed.
// The table is a circular sector, not the triangle joining its corners.
#include <cstdio>
#include <cmath>
int n, choice[1024];
bool good[1024], possible[1024];
double cx[10],cy[10];
int main() {
    const double pi=acos(-1.0), eps=1e-9;
    while(scanf("%d",&n)==1 && n) {
        double px,py,r,tx,ty,ux,uy,vx,vy;
        scanf(" (%lf ,%lf ) %lf (%lf ,%lf ) (%lf ,%lf ) (%lf ,%lf )",&px,&py,&r,&tx,&ty,&ux,&uy,&vx,&vy);
        ux-=tx; uy-=ty; vx-=tx; vy-=ty;
        double rad=sqrt(ux*ux+uy*uy);
        double a=2*pi/n;
        for(int i=0;i<n;++i) {
            cx[i]=px+2*r/(3*a)*(sin((i+1)*a)-sin(i*a));
            cy[i]=py+2*r/(3*a)*(cos(i*a)-cos((i+1)*a));
        }
        int full=(1<<n)-1;
        good[0]=possible[0]=true;
        for(int m=1;m<=full;++m) {
            int count=0; double x=0,y=0;
            for(int i=0;i<n;++i) if(m&(1<<i)) {
                ++count; x+=cx[i]; y+=cy[i];
            }
            x=x/count-tx; y=y/count-ty;
            good[m]=(x*x+y*y<=(rad+eps)*(rad+eps)
                     && ux*y-uy*x>=-eps*rad && x*vy-y*vx>=-eps*rad);
            possible[m]=false; choice[m]=-1;
            if(good[m]) for(int i=0;i<n;++i) if((m&(1<<i)) && possible[m^(1<<i)]) {
                possible[m]=true; choice[m]=i; break;
            }
        }
        if(!possible[full]) puts("impossible");
        else { int m=full; bool first=true;
            while(m) { int i=choice[m]; printf("%s%d",first?"":" ",i+1); first=false; m^=1<<i; }
            puts("");
        }
    }
    return 0;
}
