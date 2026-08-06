// POJ 3342 - Party at Hali-Bula
// Model: gpt-5.6-terra
// Tree DP computes the maximum independent set for each employee subtree.
// A state is unique only when every forced child state is unique and no child tie occurs.

#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct State {
    int count;
    bool unique;
    State(int value = 0, bool only = true) : count(value), unique(only) {}
};

vector< vector<int> > children;

void solve(int node, State& invited, State& absent) {
    invited = State(1, true);
    absent = State(0, true);

    for (size_t i = 0; i < children[node].size(); ++i) {
        State childInvited;
        State childAbsent;
        solve(children[node][i], childInvited, childAbsent);

        invited.count += childAbsent.count;
        invited.unique = invited.unique && childAbsent.unique;

        if (childInvited.count > childAbsent.count) {
            absent.count += childInvited.count;
            absent.unique = absent.unique && childInvited.unique;
        } else if (childInvited.count < childAbsent.count) {
            absent.count += childAbsent.count;
            absent.unique = absent.unique && childAbsent.unique;
        } else {
            absent.count += childInvited.count;
            absent.unique = false;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    while (cin >> n && n != 0) {
        map<string, int> id;
        children.assign(n, vector<int>());
        int nextId = 0;

        string boss;
        cin >> boss;
        id[boss] = nextId++;
        int root = id[boss];

        for (int i = 1; i < n; ++i) {
            string employee;
            cin >> employee >> boss;
            if (id.find(employee) == id.end()) {
                id[employee] = nextId++;
            }
            if (id.find(boss) == id.end()) {
                id[boss] = nextId++;
            }
            children[id[boss]].push_back(id[employee]);
        }

        State invited;
        State absent;
        solve(root, invited, absent);
        if (invited.count > absent.count) {
            cout << invited.count << " " << (invited.unique ? "Yes" : "No") << "\n";
        } else if (invited.count < absent.count) {
            cout << absent.count << " " << (absent.unique ? "Yes" : "No") << "\n";
        } else {
            cout << invited.count << " No\n";
        }
    }
    return 0;
}
