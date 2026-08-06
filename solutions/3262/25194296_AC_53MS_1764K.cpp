// POJ 3262 - Protecting the Flowers
//
// FJ ferries N cows to their barns one at a time; taking cow i round-trip
// costs 2*Ti minutes, during which every cow still waiting destroys its Di
// flowers per minute. The total damage is sum over cows of
// Di * (time elapsed before that cow is picked up), and the time elapsed
// before a cow is exactly the sum of 2*Tj over all cows j taken earlier.
//
// This is a classic adjacent-swap exchange argument: for two cows i, j
// picked back-to-back after some fixed elapsed time S, doing i then j costs
// Di*S + Dj*(S + 2*Ti), while j then i costs Dj*S + Di*(S + 2*Tj). Their
// difference is 2*(Dj*Ti - Di*Tj), so i should precede j iff Ti*Dj < Tj*Di,
// i.e. sort ascending by Ti/Di (cross-multiplied to stay in integers). This
// comparator is a strict weak ordering, so the adjacent-swap argument
// extends to a global optimum via sorting.
//
// After sorting, accumulate: prefix = running sum of 2*Ti already spent,
// answer += Di * prefix, then prefix += 2*Ti for the current cow.
//
// N <= 100,000, Ti <= 2,000,000, Di <= 100: the total can reach ~2e18,
// well past 32 bits, so everything is carried in long long. POJ's old judge
// backends are inconsistent about %lld vs %I64d for long long, so the
// answer is printed by hand, digit by digit, to sidestep the format
// specifier entirely.
//
// One point easy to miss in the statement: "2 x Ti minutes (Ti to get there
// and Ti to return)" -- the round trip, not a one-way Ti, is what blocks
// the other cows while FJ is gone.

#include <cstdio>
#include <algorithm>
using namespace std;

struct Cow {
    long long t, d;
};

bool cmp(const Cow &a, const Cow &b) {
    // a before b is better iff a.t/a.d < b.t/b.d  <=>  a.t*b.d < b.t*a.d
    return a.t * b.d < b.t * a.d;
}

void printLL(long long x) {
    if (x == 0) {
        putchar('0');
        putchar('\n');
        return;
    }
    char buf[32];
    int len = 0;
    while (x > 0) {
        buf[len++] = static_cast<char>('0' + (x % 10));
        x /= 10;
    }
    for (int i = len - 1; i >= 0; --i) {
        putchar(buf[i]);
    }
    putchar('\n');
}

int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    Cow *cows = new Cow[n];
    for (int i = 0; i < n; ++i) {
        int t, d;
        scanf("%d %d", &t, &d);
        cows[i].t = t;
        cows[i].d = d;
    }
    sort(cows, cows + n, cmp);

    long long total = 0;
    long long prefix = 0;
    for (int i = 0; i < n; ++i) {
        total += cows[i].d * prefix;
        prefix += 2 * cows[i].t;
    }
    delete[] cows;
    printLL(total);
    return 0;
}
