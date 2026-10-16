// POJ 2878 - IOI Photos
// Model: gpt-6.1-sol
// Count copies per negative across all clients, including repeated items.
// At each copy layer choose single prints, whole rolls, or one complete set.
// A shop order is independent of a client's input line (discussion cases).
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    int t; cin >> t;
    while (t--) {
        int n; long long s,r,a; cin >> n >> s >> r >> a;
        string line; getline(cin,line);
        map<string, vector<int> > rolls;
        int highest=0;
        for(int i=0;i<n;++i) {
            getline(cin,line);
            size_t pos=0;
            while(pos<line.size()) {
                while(pos<line.size() && (line[pos]==' ' || line[pos]=='\t' || line[pos]=='\r')) ++pos;
                if(pos==line.size()) break;
                size_t colon=line.find(':',pos);
                if(colon==string::npos) break;
                string place=line.substr(pos,colon-pos);
                while(!place.empty() && (place[place.size()-1]==' ' || place[place.size()-1]=='\t')) place.erase(place.size()-1);
                pos=colon+1;
                int roll=atoi(line.c_str()+pos);
                colon=line.find(':',pos); pos=colon+1;
                while(pos<line.size() && (line[pos]==' ' || line[pos]=='\t')) ++pos;
                int first=atoi(line.c_str()+pos), last=first;
                while(pos<line.size() && line[pos]>='0' && line[pos]<='9') ++pos;
                if(pos+1<line.size() && line[pos]=='.' && line[pos+1]=='.') {
                    pos+=2; last=atoi(line.c_str()+pos);
                    while(pos<line.size() && line[pos]>='0' && line[pos]<='9') ++pos;
                }
                string key=place+":"+char('A'+roll);
                vector<int>& v=rolls[key]; if(v.empty()) v.resize(37,0);
                for(int p=first;p<=last;++p) highest=max(highest,++v[p]);
            }
        }
        long long answer=0;
        for(int layer=1;layer<=highest;++layer) {
            long long cost=0;
            for(map<string,vector<int> >::iterator it=rolls.begin();it!=rolls.end();++it) {
                int count=0;
                for(int p=1;p<=36;++p) if(it->second[p]>=layer) ++count;
                cost+=min(r,s*count);
            }
            answer+=min(a,cost);
        }
        cout << answer << '\n';
    }
    return 0;
}
