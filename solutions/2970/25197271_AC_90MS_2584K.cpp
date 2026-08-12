// POJ 2970 - The lazy programmer
// Model: gpt-5.6-terra
// Sort contracts by deadline.  Every deadline prefix must shed enough work;
// within a prefix, shedding one unit from the largest a costs least.

#include <algorithm>
#include <cstdio>
#include <queue>
#include <vector>

struct Job {
    int a;
    int b;
    int d;
};

static bool earlier(const Job& x, const Job& y) {
    return x.d < y.d;
}

int main() {
    int n;
    if (std::scanf("%d", &n) != 1) return 0;

    std::vector<Job> jobs(n);
    for (int i = 0; i < n; ++i) {
        std::scanf("%d%d%d", &jobs[i].a, &jobs[i].b, &jobs[i].d);
    }
    std::sort(jobs.begin(), jobs.end(), earlier);

    std::priority_queue< std::pair<int, int> > available;
    int normalTime = 0;
    int shortened = 0;
    double answer = 0.0;

    for (int i = 0; i < n; ++i) {
        normalTime += jobs[i].b;
        available.push(std::make_pair(jobs[i].a, jobs[i].b));
        int required = normalTime - jobs[i].d;
        while (shortened < required) {
            std::pair<int, int> best = available.top();
            available.pop();
            int take = required - shortened;
            if (take > best.second) take = best.second;
            shortened += take;
            answer += (double)take / best.first;
            best.second -= (int)take;
            if (best.second != 0) available.push(best);
        }
    }

    std::printf("%.2f\n", answer + 1e-9);
    return 0;
}
