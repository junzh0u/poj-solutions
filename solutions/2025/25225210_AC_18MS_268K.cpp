// POJ 2025 - Cubic Rube
// Model: gpt-6.1-sol
// Enumerate all proper cube rotations and every translation inside the target.
// Match one solid's occupied cells to the other's complement. Reflections are
// forbidden; leading empty rows and columns do not constrain translations.
// A posted No case is actually Yes: (x,y,z) -> (z+3,y+1,5-x) fits it.
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
using namespace std;
struct Point { int c[3]; };
set<string> placements(const vector<Point>& v) {
    set<string> result;
    if (v.empty()) { result.insert(string(125,'0')); return result; }
    int p[3]={0,1,2};
    do {
        int inv=0;
        for(int i=0;i<3;i++) for(int j=i+1;j<3;j++) if(p[i]>p[j]) inv++;
        for(int sx=-1;sx<=1;sx+=2) for(int sy=-1;sy<=1;sy+=2) for(int sz=-1;sz<=1;sz+=2) {
            int s[3]={sx,sy,sz};
            if(sx*sy*sz != (inv%2?-1:1)) continue;
            vector<Point> w(v.size());
            int lo[3]={100,100,100},hi[3]={-100,-100,-100};
            for(size_t i=0;i<v.size();i++) for(int k=0;k<3;k++) {
                int x=w[i].c[k]=s[k]*v[i].c[p[k]];
                lo[k]=min(lo[k],x); hi[k]=max(hi[k],x);
            }
            int d[3]; for(int k=0;k<3;k++) d[k]=hi[k]-lo[k]+1;
            for(int x=0;x<=5-d[0];x++) for(int y=0;y<=5-d[1];y++) for(int z=0;z<=5-d[2];z++) {
                string mask(125,'0');
                for(size_t i=0;i<w.size();i++) {
                    int a=w[i].c[0]-lo[0]+x,b=w[i].c[1]-lo[1]+y,c=w[i].c[2]-lo[2]+z;
                    mask[a*25+b*5+c]='1';
                }
                result.insert(mask);
            }
        }
    } while(next_permutation(p,p+3));
    return result;
}
int main() {
    int n; cin>>n;
    while(n--) {
        vector<Point> a[2];
        for(int x=0;x<5;x++) for(int t=0;t<2;t++) for(int y=0;y<5;y++) {
            char h; cin>>h;
            for(int z=0;z<h-'0';z++) { Point p={{x,y,z}}; a[t].push_back(p); }
        }
        bool yes=false;
        if(a[0].size()+a[1].size()==125) {
            set<string> first=placements(a[0]),second=placements(a[1]);
            for(set<string>::iterator it=second.begin();it!=second.end();++it) {
                string complement=*it;
                for(int i=0;i<125;i++) complement[i]=(complement[i]=='0'?'1':'0');
                if(first.count(complement)) { yes=true; break; }
            }
        }
        cout<<(yes?"Yes":"No")<<'\n';
    }
}
