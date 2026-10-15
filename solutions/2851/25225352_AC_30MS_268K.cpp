// POJ 2851 - No Fold'em Hold'em
// Model: gpt-6.1-sol
// Enumerate every unused river card and every five-card subset of each
// player's seven cards. Compare category and rank kickers lexicographically.
// Prefer all winning rivers; only if none win, output all tying rivers.
// A player may use the entire board, and A2345 is a five-high straight.
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
int value(const int *a) {
    int cnt[15]={0},r[5],i,j;
    bool flush=true;
    for(i=0;i<5;++i){r[i]=a[i]%13+2; ++cnt[r[i]]; if(a[i]/13!=a[0]/13) flush=false;}
    sort(r,r+5,greater<int>());
    int straight=0;
    for(i=14;i>=6;--i) if(cnt[i]&&cnt[i-1]&&cnt[i-2]&&cnt[i-3]&&cnt[i-4]){straight=i;break;}
    if(!straight&&cnt[14]&&cnt[2]&&cnt[3]&&cnt[4]&&cnt[5]) straight=5;
    int groups[5],n=0;
    for(i=4;i>=1;--i) for(j=14;j>=2;--j) if(cnt[j]==i) groups[n++]=j;
    int cat=0, digits[5]={0},len=0;
    if(flush&&straight){cat=8;digits[len++]=straight;}
    else if(cnt[groups[0]]==4){cat=7;digits[len++]=groups[0];digits[len++]=groups[1];}
    else if(cnt[groups[0]]==3&&cnt[groups[1]]==2){cat=6;digits[len++]=groups[0];digits[len++]=groups[1];}
    else if(flush){cat=5;for(i=0;i<5;++i) digits[len++]=r[i];}
    else if(straight){cat=4;digits[len++]=straight;}
    else {if(cnt[groups[0]]==3)cat=3;else if(cnt[groups[0]]==2)cat=(cnt[groups[1]]==2?2:1);
        for(i=0;i<n;++i) digits[len++]=groups[i];}
    int ans=cat;
    for(i=0;i<5;++i)ans=ans*15+digits[i];
    return ans;
}
int best(int *a){int ans=0,b[5];for(int x=0;x<7;++x)for(int y=x+1;y<7;++y){int n=0;for(int z=0;z<7;++z)if(z!=x&&z!=y)b[n++]=a[z];ans=max(ans,value(b));}return ans;}
int main(){
    const string ranks="23456789TJQKA",suits="SDHC";
    int tests;cin>>tests;
    for(int tc=1;tc<=tests;++tc){
        string s[3];cin>>s[0]>>s[1]>>s[2];
        bool used[52]={false};int a[7],b[7];
        for(int l=0;l<3;++l)for(int p=0;p<(int)s[l].size();p+=2){int c=(int)suits.find(s[l][p+1])*13+(int)ranks.find(s[l][p]);used[c]=true;
            if(l==0)a[p/2]=c;else if(l==1)b[p/2]=c;else a[2+p/2]=b[2+p/2]=c;}
        vector<int> win,tie;
        for(int c=0;c<52;++c)if(!used[c]){a[6]=b[6]=c;int x=best(a),y=best(b);if(x>y)win.push_back(c);else if(x==y)tie.push_back(c);}
        vector<int> &out=win.empty()?tie:win;
        cout<<tc<<":";
        if(out.empty())cout<<" LOSER";
        for(int i=0;i<(int)out.size();++i){if(i&&i%15==0)cout<<"\n  ";cout<<' '<<ranks[out[i]%13]<<suits[out[i]/13];}
        cout<<'\n';
    }
    return 0;
}
