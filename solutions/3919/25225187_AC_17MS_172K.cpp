// POJ 3919 - Cell Towers
// Model: gpt-6.1-sol
// Walk the integer mile markers using cumulative segment lengths. If the
// remaining road is at least half a mile, evaluate its endpoint at the next
// marker. Compare rounded signal strengths; keep the first tower on ties.
#include <cstdio>
#include <cmath>
#include <algorithm>
struct Point { double x,y; };
int main() {
    int t,r;
    while (scanf("%d", &t)==1 && t) {
        scanf("%d", &r);
        Point tower[10], road[11]; double power[10], sum[11];
        for(int i=0;i<t;++i) scanf("%lf%lf%lf",&tower[i].x,&tower[i].y,&power[i]);
        for(int i=0;i<=r;++i) scanf("%lf%lf",&road[i].x,&road[i].y);
        sum[0]=0;
        for(int i=1;i<=r;++i) {
            double dx=road[i].x-road[i-1].x,dy=road[i].y-road[i-1].y;
            sum[i]=sum[i-1]+sqrt(dx*dx+dy*dy);
        }
        int last=(int)floor(sum[r]+0.5),seg=1,previous=-1;
        for(int mile=0;mile<=last;++mile) {
            double at=std::min((double)mile,sum[r]);
            while(seg<r && sum[seg]<at) ++seg;
            double f=(at-sum[seg-1])/(sum[seg]-sum[seg-1]);
            double x=road[seg-1].x+f*(road[seg].x-road[seg-1].x);
            double y=road[seg-1].y+f*(road[seg].y-road[seg-1].y);
            int best=0; double strength=-1;
            for(int i=0;i<t;++i) {
                double dx=x-tower[i].x,dy=y-tower[i].y;
                double s=floor(power[i]/(dx*dx+dy*dy)+0.5);
                if(s>strength) {strength=s;best=i;}
            }
            if(best!=previous) {
                if(previous!=-1) printf(" ");
                printf("(%d,%c)",mile,'A'+best); previous=best;
            }
        }
        printf("\n");
    }
    return 0;
}
