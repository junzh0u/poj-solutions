// POJ 2791 - Area 51
// Model: gpt-6-astra
// Partition the fence at every pairwise line-of-sight coincidence. The
// angular order is constant inside each open interval; test a rational
// interior point using exact integer cross products. Never merge adjacent
// matching intervals: their shared endpoint is an inadmissible viewpoint.
// The prose and official input validator require y > 0; the printed
// input bound y >= 0 is a typo. O(m^3 log m) time, O(m^2) space.
#include <algorithm>
#include <cstdio>
#include <vector>
using namespace std;
struct Fraction {
    int n, d;
    Fraction(int a=0, int b=1): n(a), d(b) {
        if (d<0) { n=-n; d=-d; }
    }
    bool operator<(const Fraction& b) const { return n*b.d < b.n*d; }
    bool operator==(const Fraction& b) const { return n*b.d == b.n*d; }
};
struct Chimney { char c; int x,y; } p[100];
struct BySight {
    Fraction v;
    BySight(Fraction f): v(f) {}
    bool operator()(int a, int b) const {
        return (p[a].x*v.d-v.n)*p[b].y < (p[b].x*v.d-v.n)*p[a].y;
    }
};
int main() {
    int m;
    char wanted[101];
    if (scanf("%d%s", &m, wanted)!=2) return 0;
    for (int i=0;i<m;++i) scanf(" %c%d%d", &p[i].c,&p[i].x,&p[i].y);
    vector<Fraction> events;
    for (int i=0;i<m;++i) for (int j=0;j<i;++j)
        if (p[i].y!=p[j].y)
            events.push_back(Fraction(p[i].x*p[j].y-p[j].x*p[i].y,p[j].y-p[i].y));
    sort(events.begin(),events.end());
    events.erase(unique(events.begin(),events.end()),events.end());
    int k=events.size();
    vector<int> answer;
    for (int i=0;i<=k;++i) {
        Fraction v;
        if (k==0) v=Fraction(0);
        else if (i==0) v=Fraction(events[0].n-events[0].d,events[0].d);
        else if (i==k) v=Fraction(events[k-1].n+events[k-1].d,events[k-1].d);
        else v=Fraction(events[i-1].n+events[i].n,events[i-1].d+events[i].d);
        int order[100];
        for (int j=0;j<m;++j) order[j]=j;
        sort(order,order+m,BySight(v));
        bool good=true;
        for (int j=0;j<m;++j) if (p[order[j]].c!=wanted[j]) good=false;
        if (good) answer.push_back(i);
    }
    printf("%d\n",int(answer.size()));
    for (unsigned j=0;j<answer.size();++j) {
        int i=answer[j];
        if (j) printf(" ");
        if (i==0) printf("*");
        else printf("%.12g",double(events[i-1].n)/events[i-1].d);
        printf(" ");
        if (i==k) printf("*");
        else printf("%.12g",double(events[i].n)/events[i].d);
    }
    printf("\n");
    return 0;
}
