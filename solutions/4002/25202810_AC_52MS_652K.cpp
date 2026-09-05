// POJ 4002 - Alice's mooncake shop
// Model: gpt-5.6-terra
// For every sales hour, minimize production cost plus linear storage cost over
// the still-usable production hours. A monotone deque maintains that window.
// Dates are converted from the stated Jan 1st 2000, 0 o'clock origin.
#include <cstdio>
#include <cstring>
#include <deque>
#include <vector>
#include <string>
using namespace std;

struct Order {
    int hour;
    int amount;
};

static bool leap(int y) {
    return y % 400 == 0 || (y % 4 == 0 && y % 100 != 0);
}

static int monthNumber(const char *s) {
    static const char *names[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    for (int i = 0; i < 12; ++i)
        if (strcmp(s, names[i]) == 0) return i + 1;
    return 0;
}

static int absoluteHour(const char *mon, int day, int year, int h) {
    static const int daysBeforeMonth[] = {0, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    int days = 0;
    for (int y = 2000; y < year; ++y) days += leap(y) ? 366 : 365;
    int m = monthNumber(mon);
    days += daysBeforeMonth[m] + day - 1;
    if (m > 2 && leap(year)) ++days;
    return days * 24 + h + 1;
}

int main() {
    int n, m;
    while (scanf("%d%d", &n, &m) == 2 && (n || m)) {
        vector<Order> orders(n);
        for (int i = 0; i < n; ++i) {
            char mon[8];
            int day, year, h, amount;
            scanf("%7s%d%d%d%d", mon, &day, &year, &h, &amount);
            orders[i].hour = absoluteHour(mon, day, year, h);
            orders[i].amount = amount;
        }
        int t, s;
        scanf("%d%d", &t, &s);
        vector<int> cost(m + 1);
        for (int i = 1; i <= m; ++i) scanf("%d", &cost[i]);

        deque<int> q;
        long long answer = 0;
        int oi = 0;
        for (int h = 1; h <= m; ++h) {
            while (!q.empty() && q.front() < h - t) q.pop_front();
            long long value = (long long)cost[h] - (long long)h * s;
            while (!q.empty() && (long long)cost[q.back()] - (long long)q.back() * s >= value) q.pop_back();
            q.push_back(h);
            while (oi < n && orders[oi].hour == h) {
                int best = q.front();
                answer += (long long)orders[oi].amount * ((long long)cost[best] + (long long)(h - best) * s);
                ++oi;
            }
        }
        printf("%lld\n", answer);
    }
    return 0;
}
