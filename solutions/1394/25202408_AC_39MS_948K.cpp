// POJ 1394 - Railroad
// Model: claude-sonnet-5
//
// For each scenario: build C cities and T trains, each train a chronological
// list of (time, city) stops where boarding/alighting is possible (transfers
// between trains are instantaneous). Starting no earlier than a given time
// from city S, find the connection to city D with the earliest arrival time,
// and among all connections achieving that arrival, the one whose FIRST
// boarding (departure from S) is as late as possible.
//
// Forward pass: dist[c] = earliest time one can be present at city c
// (dist[S] = start time). Repeatedly sweep all trains; for a train, once a
// stop's city has dist[.] <= that stop's time we are "boarded", and every
// later stop's city gets dist[.] relaxed down to that stop's time. Since a
// used path never needs to revisit a city, C-1 sweeps (until no change)
// suffice, matching a Bellman-Ford bound on the number of hops.
//
// ArrBest = dist[D]. If infinite, "No connection".
//
// The forward dist[] only keeps the single earliest arrival per city, which
// can hide an alternative, later-departing path that reaches the same city
// no earlier than an already-recorded better arrival but still ultimately
// reaches D at the same ArrBest via a different, later first departure (this
// is exactly the situation in a board-posted test case: a direct train from
// A gives the earliest arrival at B, but boarding a later train from A that
// reaches B slightly later still connects onward to the same ArrBest at C,
// with a strictly later initial departure from A). So a second, backward
// pass is needed to get the departure tie-break right:
//
// Backward pass: latest[c] = the latest time one may be present at city c
// and still be able to reach D by ArrBest (latest[D] = ArrBest). Sweep every
// train from its last stop back to its first; once a stop's city already has
// latest[.] >= that stop's time (either because it's D itself with enough
// slack, or a downstream stop of this same train already proved reachable),
// then boarding at any earlier stop of the train also succeeds, so its
// city's latest[.] is relaxed up to that earlier stop's time. Repeat until
// no change (same C-1 sweep bound). latest[S] is then exactly the answer's
// initial departure time: it's guaranteed to be an actually achievable
// departure (proved via the forward computation reaching ArrBest from
// start_time) and it's the maximum such departure, by monotonicity of the
// "earliest arrival given a departure floor" step function.
//
// Verified by hand against both the official sample and a board-posted test
// case (message 359879 on the discuss board) which specifically separates
// the naive single-forward-pass approach (would answer 0830) from the
// correct backward-pass tie-break (0840).
//
// Ambiguity: none in the statement itself; the subtlety is purely algorithmic
// (the departure tie-break needs its own reachability computation, not just
// bookkeeping bolted onto the forward earliest-arrival relaxation).

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
using namespace std;

const int INF = 1000000;
const int NEG_INF = -1;
char buf[128];

int main() {
    int C;
    int scenario = 1;
    while (scanf("%d", &C) == 1 && C != 0) {
        map<string, int> idx;
        vector<string> name(C);
        for (int i = 0; i < C; i++) {
            scanf("%s", buf);
            name[i] = buf;
            idx[buf] = i;
        }
        int T;
        scanf("%d", &T);
        vector<vector<pair<int, int> > > trains(T);
        for (int i = 0; i < T; i++) {
            int ti;
            scanf("%d", &ti);
            trains[i].resize(ti);
            for (int j = 0; j < ti; j++) {
                int t;
                scanf("%d %s", &t, buf);
                trains[i][j] = make_pair(t, idx[buf]);
            }
        }
        int startTime;
        scanf("%d", &startTime);
        scanf("%s", buf);
        int S = idx[buf];
        scanf("%s", buf);
        int D = idx[buf];

        // Forward: earliest arrival time reachable at each city.
        vector<int> dist(C, INF);
        dist[S] = startTime;
        int maxPasses = C + 5;
        for (int pass = 0; pass < maxPasses; pass++) {
            bool changed = false;
            for (int i = 0; i < T; i++) {
                vector<pair<int, int> > &st = trains[i];
                bool boarded = false;
                for (size_t j = 0; j < st.size(); j++) {
                    int t = st[j].first, c = st[j].second;
                    if (!boarded && dist[c] <= t) boarded = true;
                    if (boarded && t < dist[c]) {
                        dist[c] = t;
                        changed = true;
                    }
                }
            }
            if (!changed) break;
        }

        int arrBest = dist[D];

        printf("Scenario #%d\n", scenario++);
        if (arrBest >= INF) {
            printf("No connection\n");
        } else {
            // Backward: latest time one may be at a city and still reach D by arrBest.
            vector<int> latest(C, NEG_INF);
            latest[D] = arrBest;
            for (int pass = 0; pass < maxPasses; pass++) {
                bool changed = false;
                for (int i = 0; i < T; i++) {
                    vector<pair<int, int> > &st = trains[i];
                    bool reachable = false;
                    for (int j = (int)st.size() - 1; j >= 0; j--) {
                        int t = st[j].first, c = st[j].second;
                        if (latest[c] >= t) reachable = true;
                        if (reachable && t > latest[c]) {
                            latest[c] = t;
                            changed = true;
                        }
                    }
                }
                if (!changed) break;
            }
            int depart = latest[S];
            printf("Departure %04d %s\n", depart, name[S].c_str());
            printf("Arrival   %04d %s\n", arrBest, name[D].c_str());
        }
        printf("\n");
    }
    return 0;
}
