// POJ 1249 - Oil Pipeline
// Model: gpt-6.1-sol
// Enumerate pipeline rows, charging each occupied column for its shared span.
// Keep the southernmost minimum; render strict outer multiples of five.
// The label margin is the digit count of the northern border, even at y=5.
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;
int main() {
    int x,y,tc=0;
    while(cin>>x>>y && (x || y)) {
        bool well[100][80]={false};
        int lo[100],hi[100];
        fill(lo,lo+100,100); fill(hi,hi+100,-1);
        int xmin=100,xmax=0,ymin=100,ymax=0;
        do {
            well[x][y]=true; lo[x]=min(lo[x],y); hi[x]=max(hi[x],y);
            xmin=min(xmin,x); xmax=max(xmax,x); ymin=min(ymin,y); ymax=max(ymax,y);
            cin>>x>>y;
        } while(x!=-1 || y!=-1);
        int best=100000,p=1;
        for(int r=1;r<=73;++r) {
            int cost=0;
            for(int c=1;c<=94;++c) if(hi[c]>=0) cost+=max(hi[c],r)-min(lo[c],r);
            if(cost<best) {best=cost;p=r;}
        }
        cout<<"OIL FIELD "<<++tc<<'\n';
        int left=(xmin-1)/5*5,right=(xmax/5+1)*5;
        int bottom=(ymin-1)/5*5,top=(ymax/5+1)*5;
        if(right-left-1>69 || top-bottom-1>19) {
            cout<<"Map is too big to draw for pipeline at "<<p<<'\n'; continue;
        }
        int margin=top>=10?2:1;
        for(int r=top;r>=bottom;--r) {
            if(r%5==0) cout<<setw(margin)<<r; else cout<<string(margin,' ');
            for(int c=left;c<=right;++c) {
                char ch='.';
                if(r==top || r==bottom) ch=(c%5==0?'+':'-');
                else if(c==left || c==right) ch=(r%5==0?'+':'|');
                else if(well[c][r]) ch='@';
                else if(r==p || (hi[c]>=0 && r>=min(lo[c],p) && r<=max(hi[c],p))) ch='*';
                cout<<ch;
            }
            cout<<'\n';
        }
        cout<<string(margin,' ');
        ostringstream labels;
        for(int c=left;c<=right;c+=5) { labels<<c; if(c<right) labels<<string(5-(c>=10?2:1),' '); }
        cout<<labels.str()<<'\n';
    }
}
