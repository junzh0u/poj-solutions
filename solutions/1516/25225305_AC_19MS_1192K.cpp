// POJ 1516 - Parallel Deadlock
// Model: gpt-6.1-sol
// Simulate synchronous instruction starts, then FIFO message matching.
// A processor finishes only after all issued nonblocking actions complete;
// only unmatched actions already issued contribute to its blocking set.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
using namespace std;
struct Instruction { char mode, action; unsigned int pending; };
vector<Instruction> program[26];
deque<int> sends[26][26], receives[26][26];
int main() {
    int count;
    if (!(cin >> count)) return 0;
    bool present[26] = {false};
    string line;
    for (int p=0;p<count;++p) {
        char name; int n; cin >> name >> n; getline(cin,line);
        int a=name-'A'; present[a]=true;
        for(int i=0;i<n;++i) {
            getline(cin,line); istringstream in(line);
            Instruction x; in >> x.mode >> x.action; x.pending=0;
            char target; while(in >> target) x.pending |= 1U << (target-'A');
            program[a].push_back(x);
        }
    }
    int next[26]={0}, blocked[26], finished[26]={0};
    for(int a=0;a<26;++a) blocked[a]=-1;
    for(int t=1;;++t) {
        bool progress=false;
        for(int a=0;a<26;++a) if(present[a] && blocked[a]<0 && next[a]<(int)program[a].size()) {
            progress=true;
            int i=next[a]++; Instruction &x=program[a][i];
            for(int b=0;b<26;++b) if(x.pending & (1U<<b)) {
                if(x.action=='S') sends[a][b].push_back(i);
                else receives[b][a].push_back(i);
            }
            if(x.mode=='B') blocked[a]=i;
        }
        if(!progress) break;
        for(int a=0;a<26;++a) for(int b=0;b<26;++b) {
            while(!sends[a][b].empty() && !receives[a][b].empty()) {
                program[a][sends[a][b].front()].pending &= ~(1U<<b);
                program[b][receives[a][b].front()].pending &= ~(1U<<a);
                sends[a][b].pop_front(); receives[a][b].pop_front();
            }
        }
        for(int a=0;a<26;++a) if(present[a]) {
            if(blocked[a]>=0 && program[a][blocked[a]].pending==0) blocked[a]=-1;
            if(!finished[a] && next[a]==(int)program[a].size()) {
                unsigned int pending=0;
                for(int i=0;i<next[a];++i) pending |= program[a][i].pending;
                if(!pending) finished[a]=t;
            }
        }
    }
    for(int a=0;a<26;++a) if(present[a]) {
        cout << char('A'+a);
        if(finished[a]) cout << " finishes at t=" << finished[a] << '\n';
        else {
            cout << " never finishes because of ";
            unsigned int pending=0;
            for(int i=0;i<next[a];++i) pending |= program[a][i].pending;
            bool first=true;
            for(int b=0;b<26;++b) if(pending & (1U<<b)) {
                if(!first) cout << " and "; first=false; cout << char('A'+b);
            }
            cout << '\n';
        }
    }
    return 0;
}
