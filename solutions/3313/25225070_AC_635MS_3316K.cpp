// POJ 3313 - The Mastermind Master's Mind
// Model: gpt-6.1-sol
// Enumerate consistent secrets and minimize response-bucket sizes over all codes.
// A fully correct response ends the game, so its uncertainty is zero.
// Cache multiset intersections; packed digit comparisons count black pegs.
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cstring>
using namespace std;
static unsigned char pc[65536];
struct Code { unsigned int bits; int hist; string text; };
int main() {
    for (int i=1;i<65536;++i) pc[i]=pc[i>>1]+(i&1);
    int cases; cin>>cases;
    while(cases--) {
        int l,c,n; cin>>l>>c>>n;
        vector<string> guesses(n); vector<int> blacks(n), totals(n);
        for(int i=0;i<n;++i) { int w; cin>>guesses[i]>>blacks[i]>>w; totals[i]=blacks[i]+w; }
        int width=1; while((1<<width)<c) ++width; ++width;
        unsigned int low=0, high=0;
        for(int i=0;i<l;++i) { low|=((1U<<(width-1))-1)<<(i*width); high|=1U<<(i*width+width-1); }
        int count=1; for(int i=0;i<l;++i) count*=c;
        vector<Code> codes(count); vector<string> histograms; map<string,int> ids;
        vector<int> possible;
        for(int id=0;id<count;++id) {
            int v=id; string s(l,'A'), h(c,0); unsigned int bits=0;
            for(int i=l-1;i>=0;--i) { int d=v%c; v/=c; s[i]+=d; ++h[d]; bits|=(unsigned int)d<<(i*width); }
            map<string,int>::iterator it=ids.find(h); int hid;
            if(it==ids.end()) { hid=histograms.size(); ids[h]=hid; histograms.push_back(h); } else hid=it->second;
            codes[id].bits=bits; codes[id].hist=hid; codes[id].text=s;
            bool ok=true;
            for(int j=0;j<n && ok;++j) {
                int b=0, t=0; int gh[20]={0};
                for(int k=0;k<l;++k) { b+=(s[k]==guesses[j][k]); ++gh[guesses[j][k]-'A']; }
                for(int k=0;k<c;++k) t+=min((int)h[k],gh[k]);
                if(b!=blacks[j] || t!=totals[j]) ok=false;
            }
            if(ok) possible.push_back(id);
        }
        int nh=histograms.size();
        vector< vector<unsigned char> > common(nh,vector<unsigned char>(nh));
        for(int i=0;i<nh;++i) for(int j=0;j<=i;++j) {
            int t=0; for(int k=0;k<c;++k) t+=min(histograms[i][k],histograms[j][k]);
            common[i][j]=common[j][i]=t;
        }
        int best=(int)possible.size()+1, answer=0;
        for(int id=0;id<count;++id) {
            int bucket[256]; memset(bucket,0,sizeof(bucket)); int worst=0;
            const unsigned char *row=&common[codes[id].hist][0];
            for(int k=0;k<(int)possible.size();++k) {
                const Code &s=codes[possible[k]];
                unsigned int x=codes[id].bits^s.bits;
                unsigned int matches=~((x+low)|x|low)&high;
                int b=pc[matches&65535]+pc[matches>>16];
                if(b==l) continue;
                int size=++bucket[b*16+row[s.hist]-b];
                if(size>worst) worst=size;
                if(worst>=best) break;
            }
            if(worst<best) { best=worst; answer=id; }
            if(best==0) break;
        }
        cout<<codes[answer].text<<' '<<best<<'\n';
    }
}
