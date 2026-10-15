// POJ 1541 - Dart-a-Mania
// Model: gpt-6.1-sol
// Enumerate nondecreasing triples of distinct attainable score values.
// Equal scores from different board regions count as the same outcome.
// Each triple contributes one combination and 1, 3, or 6 permutations.
#include <cstdio>
#include <string>
int main() {
    bool possible[61] = {false};
    possible[0] = possible[50] = true;
    for (int n = 1; n <= 20; ++n)
        for (int m = 1; m <= 3; ++m) possible[n*m] = true;
    int combinations[181] = {0}, permutations[181] = {0};
    for (int a = 0; a <= 60; ++a) if (possible[a])
        for (int b = a; b <= 60; ++b) if (possible[b])
            for (int c = b; c <= 60; ++c) if (possible[c]) {
                ++combinations[a+b+c];
                permutations[a+b+c] += a == c ? 1 : (a == b || b == c ? 3 : 6);
            }
    int score;
    while (std::scanf("%d", &score) == 1 && score > 0) {
        if (score <= 180 && combinations[score]) {
            std::printf("NUMBER OF COMBINATIONS THAT SCORES %d IS %d.\n", score, combinations[score]);
            std::printf("NUMBER OF PERMUTATIONS THAT SCORES %d IS %d.\n", score, permutations[score]);
        } else std::printf("THE SCORE OF %d CANNOT BE MADE WITH THREE DARTS.\n", score);
        std::puts(std::string(70, '*').c_str());
    }
    std::puts("END OF OUTPUT");
    return 0;
}
