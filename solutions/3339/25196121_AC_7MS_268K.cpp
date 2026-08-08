// POJ 3339 - Team Arrangement
// Model: claude-sonnet-5
//
// For each of the 22 available players, parse role and total years of
// team membership (sum of inclusive year-interval lengths, e.g. 2003-2003
// counts as 1 year, not 0). The desired formation gives counts for
// defenders/midfielders/strikers; the goalkeeper count is always 1
// (there may be no goalkeeper at all in the roster, which makes the
// arrangement impossible). Within each role the required number of
// players is taken in ascending order of player number. If any role has
// too few available players, the arrangement is impossible.
//
// The captain is the selected player with the longest total record,
// ties broken by the larger player number (confirmed against the
// problem's discuss board, which flags this as counter to a first
// misreading). The captain is printed first; the remaining 10 selected
// players follow in the required output order (goalkeeper, defenders,
// midfielders, strikers, each ascending by number), skipping the
// captain's own slot since it was already printed.
//
// Input tokenizing does not rely on line boundaries (the board notes
// cases run back-to-back with no separating blank line, and whitespace
// within a line is not fixed either): tokens are read one at a time via
// cin >>, and a token is treated as a year-interval "YYYY-YYYY" iff it
// contains exactly one '-' with digits on both sides; this cleanly
// distinguishes it from the next player's bare number, from the
// two-dash formation token "d-m-s", and from the terminating "0".
//
// Ambiguity resolved from the statement: "Note that the captain is
// chosen among the players that are selected in the arrange" plus the
// worked sample fixes both the +1 inclusive year count and the
// bigger-number tie-break; both were double-checked against the sample's
// own captain (player 7, record 4+3=7 years) which only comes out ahead
// of player 9's tied-shaped case correctly under the +1 rule.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct Player {
    int number;
    string name;
    char role;
    int record;
};

static string pendingTok;
static bool havePending = false;

static bool readTok(string &out) {
    if (havePending) {
        out = pendingTok;
        havePending = false;
        return true;
    }
    if (cin >> out) return true;
    return false;
}

static void pushBack(const string &s) {
    pendingTok = s;
    havePending = true;
}

static bool isDigits(const string &s, int a, int b) {
    if (a >= b) return false;
    for (int i = a; i < b; i++) if (!isdigit((unsigned char)s[i])) return false;
    return true;
}

// Returns true and fills y1,y2 if token matches YYYY-YYYY (exactly one dash,
// digits on both sides, non-empty).
static bool parseYearPair(const string &t, int &y1, int &y2) {
    int dashCount = 0, dashPos = -1;
    for (size_t i = 0; i < t.size(); i++) {
        if (t[i] == '-') { dashCount++; dashPos = (int)i; }
    }
    if (dashCount != 1) return false;
    if (!isDigits(t, 0, dashPos)) return false;
    if (!isDigits(t, dashPos + 1, (int)t.size())) return false;
    y1 = atoi(t.substr(0, dashPos).c_str());
    y2 = atoi(t.substr(dashPos + 1).c_str());
    return true;
}

struct Group {
    vector<int> idx; // indices into players[], role members
};

struct NumberCmp {
    Player *players;
    NumberCmp(Player *p) : players(p) {}
    bool operator()(int a, int b) const { return players[a].number < players[b].number; }
};

int main() {
    ios::sync_with_stdio(false);

    while (true) {
        string first;
        if (!readTok(first)) break;
        if (first == "0") break;

        Player players[22];
        for (int i = 0; i < 22; i++) {
            string numTok;
            if (i == 0) numTok = first;
            else { if (!readTok(numTok)) return 0; }
            players[i].number = atoi(numTok.c_str());

            string nameTok;
            readTok(nameTok);
            players[i].name = nameTok;

            string roleTok;
            readTok(roleTok);
            players[i].role = roleTok.empty() ? '?' : roleTok[0];

            players[i].record = 0;
            while (true) {
                string t;
                if (!readTok(t)) break; // shouldn't happen before formation, but be safe
                int y1, y2;
                if (parseYearPair(t, y1, y2)) {
                    players[i].record += (y2 - y1 + 1);
                } else {
                    pushBack(t);
                    break;
                }
            }
        }

        string formation;
        readTok(formation);
        int d = 0, m = 0, s = 0;
        {
            // formation like "4-4-2": split on '-'
            int dash1 = -1, dash2 = -1;
            for (size_t i = 0; i < formation.size(); i++) {
                if (formation[i] == '-') {
                    if (dash1 == -1) dash1 = (int)i;
                    else dash2 = (int)i;
                }
            }
            d = atoi(formation.substr(0, dash1).c_str());
            m = atoi(formation.substr(dash1 + 1, dash2 - dash1 - 1).c_str());
            s = atoi(formation.substr(dash2 + 1).c_str());
        }

        vector<int> Gidx, Didx, Midx, Sidx;
        for (int i = 0; i < 22; i++) {
            switch (players[i].role) {
                case 'G': Gidx.push_back(i); break;
                case 'D': Didx.push_back(i); break;
                case 'M': Midx.push_back(i); break;
                case 'S': Sidx.push_back(i); break;
                default: break;
            }
        }
        NumberCmp cmp(players);
        sort(Gidx.begin(), Gidx.end(), cmp);
        sort(Didx.begin(), Didx.end(), cmp);
        sort(Midx.begin(), Midx.end(), cmp);
        sort(Sidx.begin(), Sidx.end(), cmp);

        bool possible = true;
        if ((int)Gidx.size() < 1) possible = false;
        if ((int)Didx.size() < d) possible = false;
        if ((int)Midx.size() < m) possible = false;
        if ((int)Sidx.size() < s) possible = false;

        if (!possible) {
            cout << "IMPOSSIBLE TO ARRANGE\n";
        } else {
            vector<int> arr;
            arr.push_back(Gidx[0]);
            for (int i = 0; i < d; i++) arr.push_back(Didx[i]);
            for (int i = 0; i < m; i++) arr.push_back(Midx[i]);
            for (int i = 0; i < s; i++) arr.push_back(Sidx[i]);

            int captainPos = 0;
            for (size_t i = 1; i < arr.size(); i++) {
                Player &cur = players[arr[i]];
                Player &best = players[arr[captainPos]];
                if (cur.record > best.record ||
                    (cur.record == best.record && cur.number > best.number)) {
                    captainPos = (int)i;
                }
            }

            Player &cap = players[arr[captainPos]];
            cout << cap.number << " " << cap.name << " " << cap.role << "\n";
            for (size_t i = 0; i < arr.size(); i++) {
                if ((int)i == captainPos) continue;
                Player &p = players[arr[i]];
                cout << p.number << " " << p.name << " " << p.role << "\n";
            }
        }
        cout << "\n";
    }
    return 0;
}
