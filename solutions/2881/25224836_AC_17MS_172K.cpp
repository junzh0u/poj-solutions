// POJ 2881 - Wiping Words
// Model: gpt-6.1-sol
// Track supporting characters for each word occurrence. Removing a word removes
// all its occurrences and releases their dependants; asterisks remain forever.
// Words match whole, case-sensitive tokens; missing columns are blank.
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <queue>
using namespace std;
struct Occ { int row, begin, end, type, supports; vector<int> above; };
bool letter(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); }
int main() {
    int t; cin >> t; string s; getline(cin,s);
    while(t--) {
        vector<string> lines;
        while(getline(cin,s)) { if(!s.empty() && s[s.size()-1]=='\r') s.erase(s.size()-1); if(s=="#") break; lines.push_back(s); }
        map<string,int> ids; vector<vector<int> > groups, cells; vector<Occ> occ;
        for(int r=0;r<(int)lines.size();++r) {
            cells.push_back(vector<int>(lines[r].size(),-1));
            for(int c=0;c<(int)lines[r].size();) {
                if(!letter(lines[r][c])) { if(lines[r][c]=='*') cells[r][c]=-2; ++c; continue; }
                int b=c; while(c<(int)lines[r].size() && letter(lines[r][c])) ++c;
                string word=lines[r].substr(b,c-b);
                map<string,int>::iterator it=ids.find(word); int id;
                if(it==ids.end()) { id=groups.size(); ids[word]=id; groups.push_back(vector<int>()); } else id=it->second;
                Occ o; o.row=r; o.begin=b; o.end=c; o.type=id; o.supports=0;
                int k=occ.size(); occ.push_back(o); groups[id].push_back(k);
                for(int j=b;j<c;++j) cells[r][j]=k;
            }
        }
        for(int k=0;k<(int)occ.size();++k) {
            int r=occ[k].row+1;
            if(r==(int)lines.size()) continue;
            for(int c=occ[k].begin;c<occ[k].end && c<(int)cells[r].size();++c) {
                int below=cells[r][c];
                if(below==-1) continue;
                ++occ[k].supports;
                if(below>=0) occ[below].above.push_back(k);
            }
        }
        queue<int> q; vector<bool> removed(groups.size(),false);
        for(int k=0;k<(int)occ.size();++k) if(!occ[k].supports && !removed[occ[k].type]) { removed[occ[k].type]=true; q.push(occ[k].type); }
        while(!q.empty()) {
            int id=q.front(); q.pop();
            for(int j=0;j<(int)groups[id].size();++j) {
                Occ &o=occ[groups[id][j]];
                for(int c=o.begin;c<o.end;++c) lines[o.row][c]=' ';
                for(int a=0;a<(int)o.above.size();++a) {
                    Occ &up=occ[o.above[a]];
                    if(--up.supports==0 && !removed[up.type]) { removed[up.type]=true; q.push(up.type); }
                }
            }
        }
        for(int r=0;r<(int)lines.size();++r) cout << lines[r] << '\n';
        cout << "#\n";
    }
}
