// POJ 3453 - Soccer Tournament
// Model: claude-sonnet-5
//
// Straightforward simulation/formatting problem. For each tournament: read N
// team names (order matters - used both for table row/col order and as the
// final tie-break in the standings), read M played games (host - guest h:g),
// then print a fixed-width ASCII results grid (row = host, column = guest,
// diagonal is "X", unplayed cells blank) followed by a standings table sorted
// by points, goal difference, goals scored, wins, then reverse input order.
//
// Column widths in the results grid are always: first column = length of the
// longest team name (used for both the row labels and the blank corner
// cell), and every other column is exactly 3 characters wide (a 3-character
// name abbreviation in the header, "d:d" results, or the centered "X").
// Standings columns are each sized to the longest value that column holds in
// this tournament (games/goals can exceed one digit once N is large), with
// every column right-aligned except the team-name column, which is
// left-aligned; columns are separated by a single space.
//
// Ambiguity: the discuss board (message 345924, reply to 114783) reports
// that an extra blank line is needed "at the very end" beyond the blank line
// each tournament block already ends with per the spec. Implemented as one
// additional blank line printed once, after the last tournament, rather than
// after every tournament (which the sample's own single blank line between
// blocks would otherwise contradict).
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

struct Team {
    string name;
    int idx;        // original input order, 0-based
    int played, won, tied, lost;
    int scored, conceded;
    int points;
};

bool cmpStandings(const Team &a, const Team &b) {
    if (a.points != b.points) return a.points > b.points;
    int da = a.scored - a.conceded, db = b.scored - b.conceded;
    if (da != db) return da > db;
    if (a.scored != b.scored) return a.scored > b.scored;
    if (a.won != b.won) return a.won > b.won;
    return a.idx > b.idx; // later in input ranks better (first)
}

string padLeft(const string &s, int w) {
    if ((int)s.size() >= w) return s;
    return string(w - s.size(), ' ') + s;
}
string padRight(const string &s, int w) {
    if ((int)s.size() >= w) return s;
    return s + string(w - s.size(), ' ');
}

int main() {
    int n;
    bool hadOutput = false;
    while (scanf("%d", &n) == 1 && n != 0) {
        hadOutput = true;
        vector<Team> teams(n);
        int nameWidth = 0;
        for (int i = 0; i < n; ++i) {
            char buf[210];
            scanf("%s", buf);
            teams[i].name = buf;
            teams[i].idx = i;
            teams[i].played = teams[i].won = teams[i].tied = teams[i].lost = 0;
            teams[i].scored = teams[i].conceded = 0;
            teams[i].points = 0;
            if ((int)teams[i].name.size() > nameWidth) nameWidth = (int)teams[i].name.size();
        }

        // results grid: gridResult[i][j] non-empty means host i vs guest j played
        vector<vector<string> > gridResult(n, vector<string>(n, ""));

        int m;
        scanf("%d", &m);
        // map name -> index
        // n <= 80, linear search is fine, but use a small map for clarity/perf
        vector<pair<string,int> > nameIndex(n);
        for (int i = 0; i < n; ++i) nameIndex[i] = make_pair(teams[i].name, i);
        sort(nameIndex.begin(), nameIndex.end());

        for (int g = 0; g < m; ++g) {
            char hostBuf[210], dashBuf[10], guestBuf[210], resBuf[10];
            scanf("%s", hostBuf);
            scanf("%s", dashBuf); // literal "-"
            scanf("%s", guestBuf);
            scanf("%s", resBuf);

            // find indices
            string hostName(hostBuf), guestName(guestBuf);
            int hi = -1, gi = -1;
            {
                pair<string,int> key(hostName, -1);
                vector<pair<string,int> >::iterator it =
                    lower_bound(nameIndex.begin(), nameIndex.end(), key);
                if (it != nameIndex.end() && it->first == hostName) hi = it->second;
            }
            {
                pair<string,int> key(guestName, -1);
                vector<pair<string,int> >::iterator it =
                    lower_bound(nameIndex.begin(), nameIndex.end(), key);
                if (it != nameIndex.end() && it->first == guestName) gi = it->second;
            }

            int hg = resBuf[0] - '0';
            int gg = resBuf[2] - '0';

            gridResult[hi][gi] = resBuf; // exactly "d:d"

            teams[hi].played++;
            teams[gi].played++;
            teams[hi].scored += hg;
            teams[hi].conceded += gg;
            teams[gi].scored += gg;
            teams[gi].conceded += hg;
            if (hg > gg) {
                teams[hi].won++;
                teams[gi].lost++;
                teams[hi].points += 3;
            } else if (hg < gg) {
                teams[gi].won++;
                teams[hi].lost++;
                teams[gi].points += 3;
            } else {
                teams[hi].tied++;
                teams[gi].tied++;
                teams[hi].points += 1;
                teams[gi].points += 1;
            }
        }

        printf("RESULTS:\n");
        // border line
        string border = "+" + string(nameWidth, '-') + "+";
        for (int j = 0; j < n; ++j) border += "---+";
        printf("%s\n", border.c_str());

        // header row
        {
            string line = "|" + string(nameWidth, ' ') + "|";
            for (int j = 0; j < n; ++j) {
                string ab = teams[j].name.substr(0, 3);
                ab = padRight(ab, 3);
                line += ab + "|";
            }
            printf("%s\n", line.c_str());
        }
        printf("%s\n", border.c_str());

        for (int i = 0; i < n; ++i) {
            string line = "|" + padRight(teams[i].name, nameWidth) + "|";
            for (int j = 0; j < n; ++j) {
                string cell;
                if (i == j) cell = " X ";
                else if (!gridResult[i][j].empty()) cell = gridResult[i][j];
                else cell = "   ";
                line += cell + "|";
            }
            printf("%s\n", line.c_str());
            printf("%s\n", border.c_str());
        }

        printf("\n");
        printf("STANDINGS:\n");
        printf("----------\n");

        vector<Team> sorted(teams);
        sort(sorted.begin(), sorted.end(), cmpStandings);

        int n_ = n;
        vector<string> c1(n_), c2(n_), c3(n_), c4(n_), c5(n_), c6(n_), c7(n_), c8(n_);
        int w1=0,w2=0,w3=0,w4=0,w5=0,w6=0,w7=0,w8=0;
        char tmp[64];
        for (int r = 0; r < n_; ++r) {
            sprintf(tmp, "%d.", r + 1);
            c1[r] = tmp;
            c2[r] = sorted[r].name;
            sprintf(tmp, "%d", sorted[r].played); c3[r] = tmp;
            sprintf(tmp, "%d", sorted[r].won); c4[r] = tmp;
            sprintf(tmp, "%d", sorted[r].tied); c5[r] = tmp;
            sprintf(tmp, "%d", sorted[r].lost); c6[r] = tmp;
            sprintf(tmp, "%d:%d", sorted[r].scored, sorted[r].conceded); c7[r] = tmp;
            sprintf(tmp, "%d", sorted[r].points); c8[r] = tmp;

            w1 = max(w1, (int)c1[r].size());
            w2 = max(w2, (int)c2[r].size());
            w3 = max(w3, (int)c3[r].size());
            w4 = max(w4, (int)c4[r].size());
            w5 = max(w5, (int)c5[r].size());
            w6 = max(w6, (int)c6[r].size());
            w7 = max(w7, (int)c7[r].size());
            w8 = max(w8, (int)c8[r].size());
        }
        for (int r = 0; r < n_; ++r) {
            string line = padLeft(c1[r], w1) + " " + padRight(c2[r], w2) + " " +
                          padLeft(c3[r], w3) + " " + padLeft(c4[r], w4) + " " +
                          padLeft(c5[r], w5) + " " + padLeft(c6[r], w6) + " " +
                          padLeft(c7[r], w7) + " " + padLeft(c8[r], w8);
            printf("%s\n", line.c_str());
        }
        printf("\n");
    }
    // Board note: one extra blank line at the very end of the whole output.
    if (hadOutput) printf("\n");
    return 0;
}
