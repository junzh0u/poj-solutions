// POJ 2147 - Dice Puzzle
// Model: gpt-6.1-sol
// Opposite faces sum to seven, so each directional value is constant
// along its axis. Enumerate the six boundary values of each x-slice;
// top and front determine its nine right faces. Intersect those views.
// Figure 7 puts the first top row at the front of the cube.
#include <cstdio>
#include <set>
#include <algorithm>
#include <iterator>
using namespace std;
int rightFace[7][7], topView[3][3], frontView[3][3];
void orientations(int t,int f,int r) {
    if(rightFace[t][f]) return;
    rightFace[t][f]=r;
    orientations(t,r,7-f);
    orientations(f,7-t,r);
}
int main() {
    orientations(1,2,3);
    int cases; if(scanf("%d",&cases)!=1) return 0;
    while(cases--) {
        for(int y=0;y<3;++y) for(int x=0;x<3;++x) scanf("%d",&topView[y][x]);
        for(int z=0;z<3;++z) for(int x=0;x<3;++x) scanf("%d",&frontView[z][x]);
        set<int> possible;
        for(int x=0;x<3;++x) {
            set<int> current;
            for(int code=0;code<46656;++code) {
                int v=code,t[3],f[3]; bool ok=true;
                for(int i=0;i<3;++i) { t[i]=v%6+1;v/=6; if(topView[i][x] && topView[i][x]!=t[i]) ok=false; }
                for(int i=0;i<3;++i) { f[i]=v%6+1;v/=6; if(frontView[i][x] && frontView[i][x]!=f[i]) ok=false; }
                if(!ok) continue;
                int key=0;
                for(int z=0;z<3;++z) for(int y=0;y<3;++y) {
                    int r=rightFace[t[y]][f[z]];
                    if(!r) ok=false;
                    key=key*6+(r ? r-1 : 0);
                }
                if(ok) current.insert(key);
            }
            if(x==0) possible.swap(current);
            else {
                set<int> intersection;
                set_intersection(possible.begin(),possible.end(),current.begin(),current.end(),inserter(intersection,intersection.begin()));
                possible.swap(intersection);
            }
        }
        bool sums[55]={false};
        for(set<int>::iterator it=possible.begin();it!=possible.end();++it) {
            int key=*it,sum=0;
            for(int i=0;i<9;++i) { sum+=key%6+1;key/=6; }
            sums[sum]=true;
        }
        bool first=true;
        for(int sum=9;sum<=54;++sum) if(sums[sum]) {
            if(!first) printf(" ");
            printf("%d",sum); first=false;
        }
        if(first) printf("0");
        printf("\n");
    }
    return 0;
}
