// POJ 2259 - Team Queue
// Model: gpt-5.6-terra
// Keep a FIFO queue of active teams and one FIFO queue per team.  An entering
// element appends to its team queue; the team enters the outer queue only when
// it was previously absent, preserving all teammate blocks in O(1) time.

#include <cstring>
#include <iostream>
#include <queue>
#include <string>

using namespace std;

int main() {
    const int MAX_ELEMENT = 1000000;
    int teamOf[MAX_ELEMENT];
    int teamCount;
    int scenario = 1;

    while (cin >> teamCount && teamCount != 0) {
        memset(teamOf, -1, sizeof(teamOf));
        queue<int> members[1000];
        queue<int> activeTeams;

        for (int team = 0; team < teamCount; ++team) {
            int count;
            cin >> count;
            while (count--) {
                int member;
                cin >> member;
                teamOf[member] = team;
            }
        }

        cout << "Scenario #" << scenario++ << '\n';
        string command;
        while (cin >> command && command != "STOP") {
            if (command == "ENQUEUE") {
                int member;
                cin >> member;
                int team = teamOf[member];
                if (members[team].empty()) {
                    activeTeams.push(team);
                }
                members[team].push(member);
            } else {
                int team = activeTeams.front();
                int member = members[team].front();
                members[team].pop();
                cout << member << '\n';
                if (members[team].empty()) {
                    activeTeams.pop();
                }
            }
        }
        cout << '\n';
    }

    return 0;
}
