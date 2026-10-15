// POJ 1594 - Message Routing
// Model: gpt-6.1-sol
// Follow the first matching rule at each MTA; a self-rule delivers locally.
// Reaching a previously visited MTA is a cycle. Scenarios continue to EOF.
#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;
struct Rule { string next, field[4]; };
int main() {
    ios::sync_with_stdio(false);
    int m, scenario = 0;
    while (cin >> m && m) {
        vector<string> name(m);
        vector< vector<Rule> > table(m);
        map<string,int> index;
        for (int i=0;i<m;++i) {
            int n; cin >> name[i] >> n; index[name[i]]=i;
            for (int j=0;j<n;++j) {
                Rule r; cin >> r.next;
                for (int k=0;k<4;++k) cin >> r.field[k];
                table[i].push_back(r);
            }
        }
        int n; cin >> n;
        cout << "Scenario # " << ++scenario << '\n';
        for (int msg=1;msg<=n;++msg) {
            string start, field[4]; cin >> start;
            for (int k=0;k<4;++k) cin >> field[k];
            int cur=index[start];
            vector<bool> seen(m,false);
            string result;
            for (;;) {
                if (seen[cur]) { result="circular routing detected by "; break; }
                seen[cur]=true;
                int chosen=-1;
                for (int j=0;j<(int)table[cur].size();++j) {
                    bool match=true;
                    for (int k=0;k<4;++k)
                        if (table[cur][j].field[k]!="*" && table[cur][j].field[k]!=field[k]) match=false;
                    if (match) { chosen=j; break; }
                }
                if (chosen<0) { result="unable to route at "; break; }
                int next=index[table[cur][chosen].next];
                if (next==cur) { result="delivered to "; break; }
                cur=next;
            }
            cout << msg << " -- " << result << name[cur] << '\n';
        }
        cout << '\n';
    }
    return 0;
}
