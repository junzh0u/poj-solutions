// POJ 1217 - FOUR QUARTERS
// Model: gpt-5.6-terra
// Each round changes A's lead by one of six values.  Dynamic programming over
// that lead gives the probability of an A win, B win, or tie after every round.
// Heads/Tails and Tails/Heads are one outcome class with probability 1/2.
#include <cstdio>
#include <cstring>

int main() {
    const int OFFSET = 64;
    const int SIZE = 129;
    const int delta[6] = {-3, -1, 0, 1, 2, 3};
    const double probability[6] = {1.0 / 16.0, 2.0 / 16.0,
                                   6.0 / 16.0, 4.0 / 16.0,
                                   2.0 / 16.0, 1.0 / 16.0};
    double current[SIZE], next[SIZE];
    int round, lead, k;

    std::memset(current, 0, sizeof(current));
    current[OFFSET] = 1.0;
    std::printf("Round   A wins    B wins    Tie\n");
    for (round = 1; round <= 20; ++round) {
        double aWins = 0.0, bWins = 0.0;
        std::memset(next, 0, sizeof(next));
        for (lead = -3 * (round - 1); lead <= 3 * (round - 1); ++lead) {
            for (k = 0; k < 6; ++k)
                next[OFFSET + lead + delta[k]] += current[OFFSET + lead] * probability[k];
        }
        std::memcpy(current, next, sizeof(current));
        for (lead = -3 * round; lead < 0; ++lead)
            bWins += current[OFFSET + lead];
        for (lead = 1; lead <= 3 * round; ++lead)
            aWins += current[OFFSET + lead];
        std::printf("%5d   %7.4f%%  %7.4f%%  %7.4f%%\n", round,
                    aWins * 100.0, bWins * 100.0, current[OFFSET] * 100.0);
    }
    return 0;
}
