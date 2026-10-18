// POJ 2510 - Tullia and Chocolate II
// Model: gpt-6-astra
// Read all events before answering sorted queries. Each consumption contributes
// its own nonnegative effect only at or after its timestamp; clamp the sum to 1.
// Times and their squares use double to avoid integer overflow. Standard %.1f
// formatting performs the requested rounding without an extra rounding step.
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>
using namespace std;
struct Event { double time, quantity; bool chocolate; };
int main() {
    vector<Event> events;
    vector<double> queries;
    char type[32];
    double time;
    while (scanf("%31s%lf", type, &time) == 2) {
        if (strcmp(type, "Query") == 0) queries.push_back(time);
        else {
            Event e;
            e.time = time;
            if (scanf("%lf", &e.quantity) != 1) return 0;
            e.chocolate = strcmp(type, "Chocolate") == 0;
            events.push_back(e);
        }
    }
    sort(queries.begin(), queries.end());
    for (size_t i = 0; i < queries.size(); ++i) {
        double total = 0;
        for (size_t j = 0; j < events.size(); ++j) {
            double dt = queries[i] - events[j].time;
            if (dt < 0) continue;
            double effect = events[j].chocolate
                ? 8 * events[j].quantity - dt / 12
                : 2 * events[j].quantity - dt * dt / 79;
            if (effect > 0) total += effect;
        }
        printf("%.0f %.1f\n", queries[i], max(1.0, total));
    }
    return 0;
}
