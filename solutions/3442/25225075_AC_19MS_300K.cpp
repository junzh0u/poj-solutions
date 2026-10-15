// POJ 3442 - Baseball
// Model: gpt-6.1-sol
// Simulate every plate appearance, preserving batting order between innings.
// A single seeded generator supplies both teams. Sacrifices advance only the
// existing runners; hits also put the batter on first. Complete each half
// inning, except the explicitly omitted home half when already ahead in ninth.
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;
struct Player { string name; double hit, sacrifice; };
struct Event { int team, player; Event(int t,int p):team(t),player(p) {} };
Player p[2][9];
string team[2];
int nextBatter[2], runs[2], hits[2], seed;
vector<Event> hitEvents, runEvents;
bool success(double probability) {
    seed = (seed * 25173 + 13849) % 65536;
    return seed / 65536.0 <= probability;
}
void halfInning(int t) {
    int base[3] = {-1,-1,-1}, outs=0;
    while (outs<3) {
        int batter=nextBatter[t];
        nextBatter[t]=(batter+1)%9;
        bool sacrifice=(base[1]>=0 && outs==0) || (base[2]>=0 && outs<=1);
        bool advance=success(sacrifice ? p[t][batter].sacrifice : p[t][batter].hit);
        if(sacrifice || !advance) ++outs;
        if(!sacrifice && advance) {
            ++hits[t]; hitEvents.push_back(Event(t,batter));
        }
        if(advance && outs<3) {
            if(base[2]>=0) {
                ++runs[t]; runEvents.push_back(Event(t,base[2]));
            }
            base[2]=base[1]; base[1]=base[0];
            base[0]=sacrifice ? -1 : batter;
        }
    }
}
void printEvents(const vector<Event>& events) {
    if(events.empty()) cout << "  none\n";
    for(size_t i=0;i<events.size();++i) {
        int t=events[i].team,b=events[i].player;
        cout << "  " << setw(15) << p[t][b].name << ' ' << setw(15) << team[t] << '\n';
    }
}
int main() {
    int games; cin >> games;
    for(int game=1;game<=games;++game) {
        for(int t=0;t<2;++t) {
            cin >> team[t];
            for(int i=0;i<9;++i) cin >> p[t][i].name >> p[t][i].hit >> p[t][i].sacrifice;
            nextBatter[t]=runs[t]=hits[t]=0;
        }
        seed=1;
        if(game>1) cout << string(60,'=') << '\n';
        cout << "Game " << game << ": " << team[0] << " vs. " << team[1] << "\n\n";
        for(int inning=1;;++inning) {
            hitEvents.clear(); runEvents.clear();
            halfInning(0);
            if(!(inning==9 && runs[1]>runs[0])) halfInning(1);
            cout << "Inning " << inning << ":\nHits:\n";
            printEvents(hitEvents);
            cout << "\nRuns:\n";
            printEvents(runEvents);
            cout << '\n';
            if(inning>=9 && runs[0]!=runs[1]) break;
        }
        cout << "End of Game:\n";
        for(int t=0;t<2;++t)
            cout << "  " << setw(15) << team[t] << ' ' << runs[t] << " runs, " << hits[t] << " hits\n";
    }
    return 0;
}
