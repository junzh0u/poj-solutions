// POJ 1296 - Noise Effect
// Model: gpt-6.1-sol
// Try all eight square symmetries by swapping axes and reversing each axis.
// Turning the token over allows reflections as well as the four rotations.
// A pixel agrees exactly when its intensity difference is at most 100.
#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;
int a[400][400], b[400][400];
int main() {
    int n;
    while (scanf("%d", &n) == 1 && n) {
        for (int i=0;i<n;++i) for(int j=0;j<n;++j) scanf("%d", &a[i][j]);
        for (int i=0;i<n;++i) for(int j=0;j<n;++j) scanf("%d", &b[i][j]);
        int best=0;
        for(int swapxy=0;swapxy<2;++swapxy)
            for(int flipx=0;flipx<2;++flipx)
                for(int flipy=0;flipy<2;++flipy) {
                    int count=0;
                    for(int i=0;i<n;++i) for(int j=0;j<n;++j) {
                        int x=swapxy?j:i, y=swapxy?i:j;
                        if(flipx) x=n-1-x;
                        if(flipy) y=n-1-y;
                        if(abs(a[i][j]-b[x][y])<=100) ++count;
                    }
                    best=max(best,count);
                }
        printf("%.2f\n",100.0*best/(n*n));
    }
    return 0;
}
