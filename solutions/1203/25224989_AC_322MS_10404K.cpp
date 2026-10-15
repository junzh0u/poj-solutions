// POJ 1203 - Timetable
// Model: gpt-6.1-sol
// Sweep minutes, processing arrivals before departures to permit instant transfers.
// Each city retains the latest origin departure reaching it so far. A train
// captures that label on departure and propagates it only upon arrival.
// Strict improvements at the destination are exactly the nondominated pairs;
// equal-time arrivals are combined before emitting a single pair.
#include <cstdio>
#include <cstring>
struct Train { int from, to, label, nextDepart, nextArrive; };
Train trains[1000000];
int departures[1440], arrivals[1440], best[100001];
int ansA[1440], ansB[1440];
int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    memset(departures, -1, sizeof(departures));
    memset(arrivals, -1, sizeof(arrivals));
    memset(best, -1, sizeof(best));
    int count = 0;
    for (int city = 1; city <= n; ++city) {
        int m; scanf("%d", &m);
        while (m--) {
            int ah, am, bh, bm, to;
            scanf("%d:%d %d:%d %d", &ah, &am, &bh, &bm, &to);
            int a = ah * 60 + am, b = bh * 60 + bm;
            Train &e = trains[count];
            e.from = city; e.to = to; e.label = -1;
            e.nextDepart = departures[a]; departures[a] = count;
            e.nextArrive = arrivals[b]; arrivals[b] = count++;
        }
    }
    int size = 0, previous = -1;
    for (int time = 0; time < 1440; ++time) {
        for (int k = arrivals[time]; k != -1; k = trains[k].nextArrive) {
            Train &e = trains[k];
            if (e.label > best[e.to]) best[e.to] = e.label;
        }
        if (best[n] > previous) {
            ansA[size] = best[n]; ansB[size++] = time;
            previous = best[n];
        }
        for (int k = departures[time]; k != -1; k = trains[k].nextDepart) {
            Train &e = trains[k];
            e.label = e.from == 1 ? time : best[e.from];
        }
    }
    printf("%d\n", size);
    for (int i = 0; i < size; ++i)
        printf("%02d:%02d %02d:%02d\n", ansA[i]/60, ansA[i]%60,
               ansB[i]/60, ansB[i]%60);
    return 0;
}
