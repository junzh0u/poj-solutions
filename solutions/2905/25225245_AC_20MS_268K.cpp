// POJ 2905 - Parallel Computer Simulator
// Model: gpt-6.1-sol
// Simulate FIFO ready and blocked queues with a program counter per program.
// Instructions finish even when they overrun the quantum; a failed lock does
// not advance its program counter. Unlock wakes one waiter at the ready head.
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <cstdlib>
using namespace std;
int main() {
    int n, cost[5], quantum;
    if (!(cin >> n)) return 0;
    for (int i=0;i<5;++i) cin >> cost[i];
    cin >> quantum;
    string line;
    getline(cin,line);
    vector< vector<string> > p(n);
    for (int i=0;i<n;++i) {
        while (getline(cin,line)) {
            string s;
            for (unsigned j=0;j<line.size();++j)
                if (line[j]!=' ' && line[j]!='\t' && line[j]!='\r') s+=line[j];
            if (s.empty()) continue;
            p[i].push_back(s);
            if (s=="end") break;
        }
    }
    vector<int> pc(n,0);
    int value[26]={0};
    bool locked=false;
    deque<int> ready, blocked;
    for (int i=0;i<n;++i) ready.push_back(i);
    while (!ready.empty()) {
        int id=ready.front(); ready.pop_front();
        int left=quantum;
        bool suspend=false, ended=false;
        while (left>0) {
            const string &s=p[id][pc[id]];
            int type;
            if (s=="lock" || s=="look") type=2;
            else if (s=="unlock") type=3;
            else if (s=="end") type=4;
            else if (s.substr(0,5)=="print") type=1;
            else type=0;
            if (type==2 && locked) {
                blocked.push_back(id); suspend=true; break;
            }
            if (type==0) value[s[0]-'a']=atoi(s.c_str()+2);
            else if (type==1) cout << id+1 << ": " << value[s[5]-'a'] << '\n';
            else if (type==2) locked=true;
            else if (type==3) {
                locked=false;
                if (!blocked.empty()) {
                    ready.push_front(blocked.front()); blocked.pop_front();
                }
            } else ended=true;
            ++pc[id]; left-=cost[type];
            if (ended) break;
        }
        if (!suspend && !ended) ready.push_back(id);
    }
    return 0;
}
