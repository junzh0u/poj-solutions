// POJ 2529 - Queuing at the doctors
// Model: gpt-6.1-sol
// Process office arrivals by time, breaking ties by visitor number.
// Reserve each office's next free unit interval and schedule the next arrival.
// Dynamic route and office storage also handles the larger bounds noted on
// the discussion board; simultaneous arrivals join behind earlier arrivals.
#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;
struct Event {
    int time, id;
    Event(int t, int i): time(t), id(i) {}
    bool operator<(const Event& e) const {
        return time != e.time ? time > e.time : id > e.id;
    }
};
int main() {
    int cases;
    if (scanf("%d", &cases) != 1) return 0;
    while (cases--) {
        int n, m;
        scanf("%d%d", &n, &m);
        vector<vector<int> > route(n);
        vector<int> pos(n, 0), freeAt(m + 1, 0);
        priority_queue<Event> events;
        int answer = 0;
        for (int i = 0; i < n; ++i) {
            int t, k;
            scanf("%d%d", &t, &k);
            route[i].resize(k);
            for (int j = 0; j < k; ++j) {
                scanf("%d", &route[i][j]);
                if (route[i][j] >= (int)freeAt.size())
                    freeAt.resize(route[i][j] + 1, 0);
            }
            if (k) events.push(Event(t, i));
            else answer = max(answer, t);
        }
        while (!events.empty()) {
            Event e = events.top();
            events.pop();
            int office = route[e.id][pos[e.id]++];
            int finish = max(e.time, freeAt[office]) + 1;
            freeAt[office] = finish;
            if (pos[e.id] < (int)route[e.id].size())
                events.push(Event(finish, e.id));
            else answer = max(answer, finish);
        }
        printf("%d\n", answer);
    }
    return 0;
}
