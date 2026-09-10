// POJ 2925 - Kick-Tipp
// Model: claude-sonnet-5
// Approach: straightforward simulation. For each game, a participant gets 1
// point for correctly guessing the outcome category (home win / draw / away
// win, determined by sign of X-Y) and 2 further points (3 total) for an
// exact score match. After each round, every participant whose round score
// equals the round's maximum (even if that maximum is 0, per the board's
// discussion of the all-zero case) receives a dot. Final ranking: points
// desc, then dots desc, then original input order. No statement ambiguity
// beyond the all-zero-round dot question, which the discuss board confirms
// resolves to "everyone still gets the dot".
#include <cstdio>
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
using namespace std;

static string readLine(istream &in) {
    string line;
    getline(in, line);
    if (!line.empty() && line[line.size() - 1] == '\r') line.erase(line.size() - 1);
    return line;
}

struct Participant {
    string name;
    int points;
    int dots;
    int idx;
};

bool cmpParticipant(const Participant &a, const Participant &b) {
    if (a.points != b.points) return a.points > b.points;
    if (a.dots != b.dots) return a.dots > b.dots;
    return a.idx < b.idx;
}

int sign(int x, int y) {
    if (x > y) return 1;
    if (x < y) return -1;
    return 0;
}

int main() {
    ios::sync_with_stdio(false);
    int T;
    {
        string l = readLine(cin);
        sscanf(l.c_str(), "%d", &T);
    }
    for (int tc = 1; tc <= T; ++tc) {
        int p, r;
        {
            string l = readLine(cin);
            sscanf(l.c_str(), "%d %d", &p, &r);
        }
        vector<Participant> part(p);
        for (int i = 0; i < p; ++i) {
            part[i].name = readLine(cin);
            part[i].points = 0;
            part[i].dots = 0;
            part[i].idx = i;
        }
        for (int rd = 0; rd < r; ++rd) {
            int g;
            {
                string l = readLine(cin);
                sscanf(l.c_str(), "%d", &g);
            }
            vector<int> roundScore(p, 0);
            for (int gm = 0; gm < g; ++gm) {
                int ax, ay;
                {
                    string l = readLine(cin);
                    sscanf(l.c_str(), "%d : %d", &ax, &ay);
                }
                int actualSign = sign(ax, ay);
                for (int i = 0; i < p; ++i) {
                    int px, py;
                    string l = readLine(cin);
                    sscanf(l.c_str(), "%d : %d", &px, &py);
                    int pts = 0;
                    if (sign(px, py) == actualSign) {
                        pts += 1;
                        if (px == ax && py == ay) pts += 2;
                    }
                    roundScore[i] += pts;
                }
            }
            int maxRound = roundScore[0];
            for (int i = 1; i < p; ++i) if (roundScore[i] > maxRound) maxRound = roundScore[i];
            for (int i = 0; i < p; ++i) {
                part[i].points += roundScore[i];
                if (roundScore[i] == maxRound) part[i].dots += 1;
            }
        }
        sort(part.begin(), part.end(), cmpParticipant);
        printf("Scenario #%d:\n", tc);
        for (int i = 0; i < p; ++i) {
            printf("%d %d %s\n", part[i].points, part[i].dots, part[i].name.c_str());
        }
        printf("\n");
    }
    return 0;
}
