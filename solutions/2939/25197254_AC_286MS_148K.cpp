// POJ 2939 - Flavius Josephus Reloaded
// Model: gpt-5.6-terra
// Follow the deterministic sequence with Floyd's cycle finder.  Only the
// soldiers on its eventual cycle are calculated twice and therefore die.

#include <iostream>

typedef long long int64;

static int64 n, a, b;

static int64 next_soldier(int64 x) {
    // Reduce after squaring so every intermediate remains below 10^18.
    return (a * ((x * x) % n) + b) % n;
}

int main() {
    while (std::cin >> n && n) {
        std::cin >> a >> b;

        int64 tortoise = next_soldier(0);
        int64 hare = next_soldier(next_soldier(0));
        while (tortoise != hare) {
            tortoise = next_soldier(tortoise);
            hare = next_soldier(next_soldier(hare));
        }

        int64 cycle_length = 1;
        hare = next_soldier(tortoise);
        while (tortoise != hare) {
            hare = next_soldier(hare);
            ++cycle_length;
        }
        std::cout << n - cycle_length << '\n';
    }
    return 0;
}
