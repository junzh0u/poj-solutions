// POJ 1397 - The Bulk
// Model: gpt-6.1-sol
// Sweep horizontal faces in increasing height. Unit-square columns alternate
// between entering and leaving the solid, independently of polygon winding.
// Scanline pairs cover each orthogonal polygon, including concave outlines.
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;
struct Point { int x,y,z; };
struct Face { int z; vector<Point> p; bool operator<(const Face& b) const { return z<b.z; } };
unsigned char inside[1000][1000];
int main() {
    int t; scanf("%d", &t);
    while(t--) {
        int n; scanf("%d", &n);
        vector<Face> faces;
        for(int i=0;i<n;++i) {
            int k; scanf("%d", &k); Face f; f.p.resize(k);
            bool horizontal=true;
            for(int j=0;j<k;++j) {
                scanf("%d%d%d", &f.p[j].x,&f.p[j].y,&f.p[j].z);
                if(j && f.p[j].z!=f.p[0].z) horizontal=false;
            }
            f.z=f.p[0].z;
            if(horizontal) faces.push_back(f);
        }
        sort(faces.begin(),faces.end());
        memset(inside,0,sizeof(inside));
        int volume=0;
        for(size_t i=0;i<faces.size();++i) {
            const Face& f=faces[i]; int lo=1000,hi=0;
            for(size_t j=0;j<f.p.size();++j) { lo=min(lo,f.p[j].y); hi=max(hi,f.p[j].y); }
            for(int y=lo;y<hi;++y) {
                vector<int> xs;
                for(size_t j=0;j<f.p.size();++j) {
                    const Point& a=f.p[j]; const Point& b=f.p[(j+1)%f.p.size()];
                    if(a.x==b.x && min(a.y,b.y)<=y && y<max(a.y,b.y)) xs.push_back(a.x);
                }
                sort(xs.begin(),xs.end());
                for(size_t j=0;j<xs.size();j+=2)
                    for(int x=xs[j];x<xs[j+1];++x) {
                        volume += inside[y][x] ? f.z : -f.z;
                        inside[y][x]^=1;
                    }
            }
        }
        printf("The bulk is composed of %d units.\n",volume);
    }
    return 0;
}
