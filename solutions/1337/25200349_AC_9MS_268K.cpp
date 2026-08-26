// POJ 1337 - A Lazy Worker
// Model: gpt-5.6-terra
// Backward dynamic programming: at each integer time, an available job must
// start; otherwise the worker idles until the next time. The discussion-board
// test data also shows that declared array bounds should not be trusted.
#include <iostream>
#include <vector>
#include <climits>

using namespace std;

struct Job {
    int time;
    int arrive;
    int deadline;
};

int main() {
    ios::sync_with_stdio(false);
    int cases;
    if (!(cin >> cases)) return 0;
    while (cases--) {
        int n;
        cin >> n;
        vector<Job> jobs(n);
        int last = 0;
        for (int i = 0; i < n; ++i) {
            cin >> jobs[i].time >> jobs[i].arrive >> jobs[i].deadline;
            if (jobs[i].deadline > last) last = jobs[i].deadline;
        }
        vector<int> best(last + 1, 0);
        for (int now = last - 1; now >= 0; --now) {
            int answer = INT_MAX;
            for (int i = 0; i < n; ++i) {
                const Job &job = jobs[i];
                if (job.arrive <= now && now + job.time <= job.deadline) {
                    int candidate = job.time + best[now + job.time];
                    if (candidate < answer) answer = candidate;
                }
            }
            best[now] = (answer == INT_MAX) ? best[now + 1] : answer;
        }
        cout << best[0] << '\n';
    }
    return 0;
}
