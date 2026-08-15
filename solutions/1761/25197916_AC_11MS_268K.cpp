// POJ 1761 - Easy Task
// Model: claude-sonnet-5
//
// For each of n submissions (time, team, problem, result), given in
// nondecreasing time order: once a team has an accepted submission for a
// problem, any further submissions of that problem by that team are not
// counted in the statistics at all (they are simply ignored, not even
// counted toward the submission count). So for each (team, problem) pair we
// track whether it has already been solved; while unsolved we increment a
// per-team-per-problem submission counter, and on the accepting submission
// we fold that counter and the submission time into the problem's totals.
//
// No statement ambiguity found; the sample verifies both the "ignore
// post-acceptance submissions" rule and the rounding, and the board raised
// no pitfalls beyond output rounding, which plain printf("%.2f", ...)
// handles.
#include <cstdio>
#include <cstring>
#include <string>
#include <map>
using namespace std;

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;

    // team index -> problem index (0..8) -> (solved?, submission count)
    map<string, int> teamId;
    int nextId = 0;
    static bool solved[70][9];
    static int cnt[70][9];
    memset(solved, 0, sizeof(solved));
    memset(cnt, 0, sizeof(cnt));

    int acceptedCnt[9] = {0};
    long sumSub[9] = {0};
    long sumTime[9] = {0};

    for (int i = 0; i < n; ++i) {
        int t;
        char teamBuf[16], probCh, resCh;
        scanf("%d %s %c %c", &t, teamBuf, &probCh, &resCh);
        string team(teamBuf);
        map<string,int>::iterator it = teamId.find(team);
        int tid;
        if (it == teamId.end()) {
            tid = nextId++;
            teamId[team] = tid;
        } else {
            tid = it->second;
        }
        int p = probCh - 'A';
        if (solved[tid][p]) continue; // ignore further submissions
        cnt[tid][p]++;
        if (resCh == 'A') {
            solved[tid][p] = true;
            acceptedCnt[p]++;
            sumSub[p] += cnt[tid][p];
            sumTime[p] += t;
        }
    }

    for (int p = 0; p < 9; ++p) {
        if (acceptedCnt[p] == 0) {
            printf("%c 0\n", 'A' + p);
        } else {
            double avgSub = (double)sumSub[p] / acceptedCnt[p];
            double avgTime = (double)sumTime[p] / acceptedCnt[p];
            printf("%c %d %.2f %.2f\n", 'A' + p, acceptedCnt[p], avgSub, avgTime);
        }
    }

    return 0;
}
