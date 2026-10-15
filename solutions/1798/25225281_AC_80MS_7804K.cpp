// POJ 1798 - Dory's Phonebook
// Model: gpt-6.1-sol
// Sort compact dictionary records by digit code, then original ASCII spelling.
// Suffix DP rejects dead decompositions. Merge matching word ranges while
// traversing feasible decompositions to stream encodings in ASCII order.
// Ignore punctuation only for matching; preserve it and case when printing.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;
struct Entry { char word[51], code[51]; };
vector<Entry> dict;
bool cmpEntry(const Entry& a,const Entry& b) {
    int c=strcmp(a.code,b.code);
    return c ? c<0 : strcmp(a.word,b.word)<0;
}
int bound(const char* s,bool upper) {
    int l=0,r=(int)dict.size();
    while(l<r) { int m=(l+r)/2; int c=strcmp(dict[m].code,s);
        if(c<0 || (upper && c==0)) l=m+1; else r=m;
    }
    return l;
}
int lo[51][51],hi[51][51],n;
bool can[51];
string phone;
struct Cursor { int at,end,next; Cursor(int a,int e,int p):at(a),end(e),next(p){} };
struct Later {
    bool operator()(const Cursor& a,const Cursor& b) const {
        return strcmp(dict[a.at].word,dict[b.at].word)>0;
    }
};
void emit(int p,const string& text) {
    if(p==n) { printf("%s: %s\n",phone.c_str(),text.c_str()); return; }
    priority_queue<Cursor,vector<Cursor>,Later> q;
    for(int e=p+1;e<=n;++e)
        if(can[e] && lo[p][e]<hi[p][e]) q.push(Cursor(lo[p][e],hi[p][e],e));
    while(!q.empty()) {
        Cursor c=q.top(); q.pop();
        string s=text;
        if(!s.empty()) s+=' ';
        s+=dict[c.at].word;
        emit(c.next,s);
        ++c.at;
        if(c.at<c.end) q.push(c);
    }
}
int main() {
    const char* map="57630499617851881234762239";
    int cases; scanf("%d",&cases);
    for(int tc=1;tc<=cases;++tc) {
        int count; scanf("%d",&count); dict.resize(count);
        for(int i=0;i<count;++i) {
            scanf("%50s",dict[i].word); int k=0;
            for(int j=0;dict[i].word[j];++j) {
                char c=dict[i].word[j]; if(c>='A' && c<='Z') c+=32;
                if(c>='a' && c<='z') dict[i].code[k++]=map[c-'a'];
            }
            dict[i].code[k]=0;
        }
        sort(dict.begin(),dict.end(),cmpEntry);
        printf("Scenario #%d:\n",tc);
        scanf("%d",&count);
        for(int t=0;t<count;++t) {
            char raw[51]; scanf("%50s",raw); phone=raw;
            string digits;
            for(int j=0;raw[j];++j) if(raw[j]>='0' && raw[j]<='9') digits+=raw[j];
            n=(int)digits.size();
            memset(can,0,sizeof(can)); can[n]=true;
            for(int p=n-1;p>=0;--p) {
                string key;
                for(int e=p+1;e<=n;++e) {
                    key+=digits[e-1];
                    lo[p][e]=bound(key.c_str(),false);
                    hi[p][e]=bound(key.c_str(),true);
                    if(can[e] && lo[p][e]<hi[p][e]) can[p]=true;
                }
            }
            if(n && can[0]) emit(0,"");
            else printf("%s cannot be encoded.\n",raw);
        }
        puts("");
    }
    return 0;
}
