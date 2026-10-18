// POJ 3515 - Moving Sticks
// Model: gpt-6-astra
// Enumerate operand digits within separate removal and insertion budgets,
// then derive and check the result. A moved stick leaves its original place
// empty, so both set differences must equal the requested move count.
// The statement's 9 has no bottom segment; leading zeros are preserved.
#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
using namespace std;
int masks[10]={119,36,93,109,46,107,123,37,127,47};
int remv[10][10],addv[10][10],n;
string a,b,r,cur,answer;
char op;
int bits(int x){int s=0;for(;x;x>>=1)s+=x&1;return s;}
bool check(long long av,long long bv,int rm,int ad){
    long long v;
    if(op=='+') v=av+bv;
    else if(op=='-') {if(av<bv)return false;v=av-bv;}
    else if(op=='*') {if(bv && av>2147483647LL/bv)return false;v=av*bv;}
    else {if(!bv || av % bv)return false;v=av/bv;}
    if(v>=2147483648LL)return false;
    string s(r.size(),'0');
    for(int i=(int)r.size()-1;i>=0;--i){int d=v%10;v/=10;s[i]=char('0'+d);rm+=remv[r[i]-'0'][d];ad+=addv[r[i]-'0'][d];if(rm>n||ad>n)return false;}
    if(v || rm!=n || ad!=n)return false;
    answer=cur.substr(0,a.size())+op+cur.substr(a.size())+"="+s;
    return true;
}
bool dfs(int p,long long av,long long bv,int rm,int ad){
    if(p==(int)(a.size()+b.size()))return check(av,bv,rm,ad);
    bool first=p<(int)a.size();
    int old=first?a[p]-'0':b[p-a.size()]-'0';
    for(int d=0;d<10;++d){
        int rr=rm+remv[old][d],aa=ad+addv[old][d];
        if(rr>n||aa>n)continue;
        cur[p]=char('0'+d);
        if(dfs(p+1,first?av*10+d:av,first?bv:bv*10+d,rr,aa))return true;
    }
    return false;
}
int main(){
    for(int i=0;i<10;++i)for(int j=0;j<10;++j){remv[i][j]=bits(masks[i]&~masks[j]);addv[i][j]=bits(masks[j]&~masks[i]);}
    string eq;int tc=0;char opening,closing;
    while(cin>>a && a!="EOF"){
        cin>>op>>b>>eq>>r>>opening>>n>>closing;
        cur=a+b;
        cout<<++tc<<". ";
        if(dfs(0,0,0,0,0))cout<<answer;else cout<<"UNSOLVABLE";
        cout<<'\n';
    }
}
