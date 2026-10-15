// POJ 1439 - Rhombs
// Model: gpt-6.1-sol
// Integrate along the boundary in the basis of directions 2 and 3.
// The diagonal-edge integrals give B and C separately; shoelace gives
// their total together with A. Internal edges cancel in every tiling.
// The diagram fixes the output order; input contains polygons until EOF.
#include <iostream>
#include <cstdlib>
using namespace std;
int main() {
    const int dx[7]={0,1,1,0,-1,-1,0};
    const int dy[7]={0,-1,0,1,1,0,-1};
    int n;
    while (cin >> n) {
        long long x=0,y=0,area=0,b=0,c=0;
        for (int i=0;i<n;++i) {
            int d,k; cin >> d >> k;
            long long nx=x+dx[d]*k,ny=y+dy[d]*k;
            area+=x*ny-y*nx;
            if (dx[d] && dy[d]) {
                b+=(x+nx)*(ny-y);
                c-=(y+ny)*(nx-x);
            }
            x=nx; y=ny;
        }
        cout << (area-b-c)/2 << ' ' << b/2 << ' ' << c/2 << '\n';
    }
    return 0;
}
