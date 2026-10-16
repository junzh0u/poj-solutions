// POJ 2885 - Prince of Persia
// Model: gpt-6.1-sol
// Enumerate ray paths, placing each physical mirror at most once.
// A placed mirror blocks later crossings; a previously crossed cell cannot
// acquire a mirror. Whole-line input also handles grids wider than 40.
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
using namespace std;
vector<string> a;
vector<vector<int> > crossed, occupied, lit;
int turnto[10][4], n,m,leftplates;
int dr[4]={-1,0,1,0},dc[4]={0,1,0,-1};
void searchray(int r,int c,int d,int used) {
    vector<pair<int,int> > trail;
    r+=dr[d]; c+=dc[d];
    while(leftplates && r>=0 && r<n && c>=0 && c<m) {
        char ch=a[r][c];
        if(r==0 || r==n-1 || c==0 || c==m-1) {
            if((ch=='-' || ch=='|') && !lit[r][c]) {lit[r][c]=1; --leftplates;}
            break;
        }
        if(occupied[r][c]) break;
        if(ch>='0' && ch<='9') {
            int id=ch-'0';
            if(!(used&(1<<id)) && !crossed[r][c] && turnto[id][d]>=0) {
                occupied[r][c]=1;
                searchray(r,c,turnto[id][d],used|(1<<id));
                occupied[r][c]=0;
            }
        }
        ++crossed[r][c]; trail.push_back(make_pair(r,c));
        r+=dr[d];c+=dc[d];
    }
    for(size_t i=0;i<trail.size();++i) --crossed[trail[i].first][trail[i].second];
}
int main(){
    int t; string s; cin>>t; getline(cin,s);
    while(t--){
        memset(turnto,-1,sizeof(turnto)); a.clear();
        do {if(!getline(cin,s)) return 0; if(!s.empty()&&s[s.size()-1]=='\r')s.erase(s.size()-1);} while(s.empty());
        while(s[0]>='0' && s[0]<='9'){
            int id=s[0]-'0'; string v=s.substr(2);
            if(v=="-/") {turnto[id][3]=2;turnto[id][0]=1;}
            if(v=="/-") {turnto[id][1]=0;turnto[id][2]=3;}
            if(v=="-\\") {turnto[id][3]=0;turnto[id][2]=1;}
            if(v=="\\-") {turnto[id][1]=2;turnto[id][0]=3;}
            getline(cin,s); if(!s.empty()&&s[s.size()-1]=='\r')s.erase(s.size()-1);
        }
        a.push_back(s);
        while(getline(cin,s)){
            if(!s.empty()&&s[s.size()-1]=='\r')s.erase(s.size()-1);
            a.push_back(s);
            bool bottom=true; for(size_t j=0;j<s.size();++j) if(s[j]!='#'&&s[j]!='-'&&s[j]!=' ')bottom=false;
            if(bottom)break;
        }
        n=a.size();m=a[0].size();leftplates=0;
        crossed.assign(n,vector<int>(m,0));occupied=crossed;lit=crossed;
        int sr=0,sc=0,sd=0;
        for(int r=0;r<n;++r)for(int c=0;c<m;++c)if(r==0||r==n-1||c==0||c==m-1){
            if(a[r][c]=='-'||a[r][c]=='|')++leftplates;
            if(a[r][c]==' '){sr=r;sc=c;sd=r==0?2:r==n-1?0:c==0?1:3;}
        }
        searchray(sr,sc,sd,0);cout<<(leftplates?"NO":"YES")<<'\n';
    }
}
