// POJ 3441 - Vacation Rentals
// Model: claude-sonnet-5
// Approach: For the requested stay (nights L..R, where L=arrival, R=departure-1),
// compute suffixCost[day][unit] = minimum number of transfers needed to cover
// nights day..R given that night `day` is spent in `unit` (available that day).
// This is computed backward: suffixCost[R][u]=0 for available u; otherwise
// suffixCost[day][u] = min over available v at day+1 of
//   suffixCost[day+1][v] + (u==v?0:1).
// The overall minimum transfer count is min over available u at day L of
// suffixCost[L][u]; if none finite, "Not available".
// Tie-break (lowest unit label at day L, then day L+1, etc.) is realized by a
// forward greedy: pick the smallest unit at day L achieving the global
// minimum, then at each subsequent day pick the smallest unit v that keeps
// the running suffix cost exactly consistent with the value already fixed at
// the previous day (i.e. suffixCost[day+1][v] + transferCost == suffixCost[day][u]).
// Since suffixCost fixes the achievable optimum from each state independently
// of earlier choices, this greedy cannot increase the total cost, and always
// picking the smallest feasible label reproduces exactly the requested
// tie-break rule (compare day by day, not just by segment boundaries) --
// confirmed against a board-reported case (message 84684) where two
// equal-cost schedules differ at an interior day and the one keeping the
// earlier (alphabetically smaller) unit longer is the accepted output.
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main(){
    int M, N;
    int caseNo = 1;
    bool firstCase = true;
    while (cin >> M >> N) {
        if (M == 0 && N == 0) break;
        vector<string> grid(M+1); // 1-indexed days
        for (int i = 1; i <= M; i++) cin >> grid[i];
        int arrival, departure;
        cin >> arrival >> departure;
        int L = arrival;
        int R = departure - 1;

        const int INF = 1000000;
        // suffixCost[day][u], day in [L,R], u in [0,N-1]
        vector< vector<int> > suffixCost(R+2, vector<int>(N, INF));

        for (int day = R; day >= L; day--) {
            for (int u = 0; u < N; u++) {
                if (grid[day][u] != 'O') continue;
                if (day == R) {
                    suffixCost[day][u] = 0;
                } else {
                    int best = INF;
                    for (int v = 0; v < N; v++) {
                        if (grid[day+1][v] != 'O') continue;
                        if (suffixCost[day+1][v] >= INF) continue;
                        int cost = suffixCost[day+1][v] + (u == v ? 0 : 1);
                        if (cost < best) best = cost;
                    }
                    suffixCost[day][u] = best;
                }
            }
        }

        int globalMin = INF;
        for (int u = 0; u < N; u++) {
            if (grid[L][u] == 'O' && suffixCost[L][u] < globalMin) {
                globalMin = suffixCost[L][u];
            }
        }

        if (!firstCase) cout << "\n";
        firstCase = false;

        cout << "Case " << caseNo << ":\n\n";
        caseNo++;

        if (globalMin >= INF) {
            cout << "Not available\n";
        } else {
            // reconstruct
            int currentUnit = -1;
            for (int u = 0; u < N; u++) {
                if (grid[L][u] == 'O' && suffixCost[L][u] == globalMin) {
                    currentUnit = u;
                    break;
                }
            }
            vector<int> segUnit;
            vector<int> segStart, segEnd; // end is exclusive move-out date
            int curSegStart = L;
            for (int day = L; day < R; day++) {
                int needed = suffixCost[day][currentUnit];
                int nextUnit = -1;
                for (int v = 0; v < N; v++) {
                    if (grid[day+1][v] != 'O') continue;
                    if (suffixCost[day+1][v] >= INF) continue;
                    int cost = suffixCost[day+1][v] + (currentUnit == v ? 0 : 1);
                    if (cost == needed) {
                        nextUnit = v;
                        break; // smallest v found first since loop is ascending
                    }
                }
                // nextUnit must be found (guaranteed by DP correctness)
                if (nextUnit != currentUnit) {
                    segUnit.push_back(currentUnit);
                    segStart.push_back(curSegStart);
                    segEnd.push_back(day+1);
                    curSegStart = day+1;
                    currentUnit = nextUnit;
                }
            }
            segUnit.push_back(currentUnit);
            segStart.push_back(curSegStart);
            segEnd.push_back(R+1);

            for (size_t i = 0; i < segUnit.size(); i++) {
                cout << (char)('A' + segUnit[i]) << ": " << segStart[i] << "-" << segEnd[i] << "\n";
            }
        }
    }
    return 0;
}
