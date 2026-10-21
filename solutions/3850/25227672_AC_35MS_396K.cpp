// POJ 3850 - Simple Polygon
// Model: gpt-6-astra
// Sort by polar angle about the bottom-left point, nearer points first.
// Reverse the final equal-angle group so the closing edge does not overlap
// another edge. Collinear consecutive vertices are legal, overlapping edges
// are not. Distinct angular wedges separate all remaining edges.
// O(n log n) time and O(n) space; coordinate products fit signed 32-bit int.
#include <algorithm>
#include <cstdio>
#include <vector>
using namespace std;
struct Point { int x, y, id; };
Point pivot;
int cross(const Point &a, const Point &b) {
    return (a.x-pivot.x)*(b.y-pivot.y)-(a.y-pivot.y)*(b.x-pivot.x);
}
int distance2(const Point &a) {
    int x=a.x-pivot.x, y=a.y-pivot.y;
    return x*x+y*y;
}
bool angle(const Point &a, const Point &b) {
    int c=cross(a,b);
    return c ? c>0 : distance2(a)<distance2(b);
}
int main() {
    int cases; if (scanf("%d", &cases)!=1) return 0;
    while (cases--) {
        int n; scanf("%d", &n);
        vector<Point> p(n);
        int k=0;
        for (int i=0;i<n;++i) {
            scanf("%d%d", &p[i].x, &p[i].y); p[i].id=i;
            if (p[i].y<p[k].y || (p[i].y==p[k].y && p[i].x<p[k].x)) k=i;
        }
        swap(p[0],p[k]); pivot=p[0];
        sort(p.begin()+1,p.end(),angle);
        k=n-1;
        while (k>1 && cross(p[k-1],p[n-1])==0) --k;
        reverse(p.begin()+k,p.end());
        for (int i=0;i<n;++i) printf("%s%d",i?" ":"",p[i].id);
        printf("\n");
    }
    return 0;
}
