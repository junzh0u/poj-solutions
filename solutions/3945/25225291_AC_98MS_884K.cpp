// POJ 3945 - Traveling Cube
// Model: gpt-6.1-sol
// Breadth-first search over square, one of 24 orientations, and visit count.
// A colored square is enterable only if it is the next requested color and
// matches the new top face; already visited colors cannot be entered again.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <cstring>
using namespace std;
string roll(string s,int d) {
    string t=s;
    if(d==0) {t[0]=s[3];t[1]=s[2];t[2]=s[0];t[3]=s[1];}
    if(d==1) {t[0]=s[2];t[1]=s[3];t[2]=s[1];t[3]=s[0];}
    if(d==2) {t[0]=s[5];t[1]=s[4];t[4]=s[0];t[5]=s[1];}
    if(d==3) {t[0]=s[4];t[1]=s[5];t[4]=s[1];t[5]=s[0];}
    return t;
}
int dista[7][900][24];
int main() {
    vector<string> ori; map<string,int> ids;
    ori.push_back("rcgmby"); ids[ori[0]]=0;
    for(unsigned i=0;i<ori.size();++i) for(int d=0;d<4;++d) {
        string t=roll(ori[i],d);
        if(ids.find(t)==ids.end()) {ids[t]=(int)ori.size();ori.push_back(t);}
    }
    int tr[24][4];
    for(int i=0;i<24;++i) for(int d=0;d<4;++d) tr[i][d]=ids[roll(ori[i],d)];
    int w,h; const int dx[4]={0,0,1,-1},dy[4]={-1,1,0,0};
    while(cin>>w>>h && (w||h)) {
        vector<string> a(h); int start=0;
        for(int y=0;y<h;++y) {cin>>a[y]; for(int x=0;x<w;++x) if(a[y][x]=='#') start=y*w+x;}
        string order;cin>>order;
        memset(dista,-1,sizeof(dista)); queue<int> q;
        dista[0][start][0]=0; q.push(start*24); int answer=-1;
        while(!q.empty()) {
            int st=q.front();q.pop();int o=st%24;st/=24;int p=st%(w*h),k=st/(w*h);
            int len=dista[k][p][o]; if(k==6) {answer=len;break;}
            for(int d=0;d<4;++d) {
                int x=p%w+dx[d],y=p/w+dy[d];
                if(x<0||x>=w||y<0||y>=h) continue;
                char c=a[y][x];if(c=='k')continue;
                int no=tr[o][d],nk=k,np=y*w+x;
                if(c!='w'&&c!='#') {if(c!=order[k]||c!=ori[no][0])continue;++nk;}
                if(dista[nk][np][no]>=0)continue;
                dista[nk][np][no]=len+1;q.push((nk*w*h+np)*24+no);
            }
        }
        if(answer<0)cout<<"unreachable\n";else cout<<answer<<'\n';
    }
    return 0;
}
