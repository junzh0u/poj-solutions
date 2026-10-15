// POJ 2156 - National Day Celebration
// Model: gpt-6.1-sol
// Recover the K-th removal by reversing the preceding rotations. Track the
// king forward in the shrinking circle until his position is removed.
// N counts soldiers, so the initial circle has N+1 people; counting is inclusive.
#include <cstdio>
int main() {
    int n, m, k, j;
    if (std::scanf("%d%d%d%d", &n, &m, &k, &j) != 4) return 0;
    ++n;
    int size = n-k+1;
    int winner = (m-1)%size;
    for (++size; size<=n; ++size) winner = (winner+m)%size;
    int pos = j-1, term = 1;
    for (size=n; ; --size, ++term) {
        int removed = (m-1)%size;
        if (pos == removed) break;
        pos = (pos-removed-1+size)%size;
    }
    std::printf("%d %d\n", winner+1, term);
    return 0;
}
