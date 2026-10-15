// POJ 3717 - Decrypt the Dragon Scroll
// Model: gpt-6.1-sol
// Store the greatest feasible first number of every suffix, then greedily
// reconstruct the least sequence. A question mark may also become a comma.
// Decimal strings avoid any bound on the magnitude of a number.
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
string s, bound; int startp, leng, direction; int memo[505][2];
bool search(int p,int strict) {
    if(p==leng) return strict!=0;
    int &v=memo[p][strict]; if(v!=-1) return v!=0;
    v=0;
    for(int d=0;d<=9;++d) {
        if(p==0 && d==0) continue;
        if(s[startp+p]!='?' && s[startp+p]!=char('0'+d)) continue;
        int b=bound[p]-'0';
        if(!strict && ((direction==1 && d<b)||(direction==-1 && d>b))) continue;
        if(search(p+1,strict || d!=b)) {v=1; break;}
    }
    return v!=0;
}
string choose(int st,int len,const string &b,int dir) {
    startp=st; leng=len; direction=dir;
    bool free=false;
    if(b.empty()) free=true;
    else if(len!=int(b.size())) {
        if((dir==1 && len<int(b.size()))||(dir==-1 && len>int(b.size()))) return "";
        free=true;
    }
    if(free) {
        string r=s.substr(st,len);
        for(int i=0;i<len;++i) if(r[i]=='?') r[i]=char('0'+(dir==1?(i==0?1:0):9));
        if(r[0]=='0') return "";
        return r;
    }
    bound=b; memset(memo,-1,sizeof(memo));
    if(!search(0,0)) return "";
    string r; int strict=0;
    for(int p=0;p<len;++p) {
        for(int k=0;k<10;++k) {
            int d=dir==1?k:9-k, bv=bound[p]-'0';
            if((p==0 && d==0)||(s[st+p]!='?' && s[st+p]!=char('0'+d))) continue;
            if(!strict && ((dir==1 && d<bv)||(dir==-1 && d>bv))) continue;
            int ns=strict || d!=bv;
            if(search(p+1,ns)) {r+=char('0'+d); strict=ns; break;}
        }
    }
    return r;
}
bool lessnum(const string &a,const string &b) {return a.size()!=b.size()?a.size()<b.size():a<b;}
int main() {
    while(cin>>s) {
        int n=s.size(); string best[505];
        for(int i=n-1;i>=0;--i) {
            int end=i; while(end<n && s[end]!=',') ++end;
            for(int j=end;j>i;--j) {
                if(j<n && s[j]!=',' && s[j]!='?') continue;
                if(j<n && (j+1>=n || best[j+1].empty())) continue;
                string x=choose(i,j-i,j==n?string():best[j+1],-1);
                if(!x.empty()) {best[i]=x;break;}
            }
        }
        if(best[0].empty()) {cout<<"impossible\n";continue;}
        int i=0; string prev,answer;
        while(i<n) {
            int end=i; while(end<n && s[end]!=',') ++end;
            for(int j=i+1;j<=end;++j) {
                if(j<n && s[j]!=',' && s[j]!='?') continue;
                if(j<n && (j+1>=n || best[j+1].empty())) continue;
                string x=choose(i,j-i,prev,1);
                if(x.empty() || (j<n && !lessnum(x,best[j+1]))) continue;
                if(!answer.empty()) answer+=',';
                answer+=x;prev=x;i=j+1;break;
            }
        }
        cout<<answer<<'\n';
    }
}
