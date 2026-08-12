// POJ 1644 - To Bet or Not To Bet
// Model: gpt-5.6-terra
// Dynamic programming keeps the position before a turn and a separate state
// for a square whose Lose-a-turn instruction is pending.  A displacement
// instruction is applied only to the square reached by the coin flip; the
// instruction on its destination is deliberately not chained.
#include <cstdio>
#include <cstring>

int main() {
    int cases;
    if (std::scanf("%d", &cases) != 1) return 0;
    while (cases--) {
        int m, turns;
        std::scanf("%d%d", &m, &turns);
        int move[55];
        bool lose[55];
        std::memset(move, 0, sizeof(move));
        std::memset(lose, 0, sizeof(lose));
        char token[32];
        int i;
        for (i = 1; i <= m; ++i) {
            std::scanf("%s", token);
            if (token[0] == 'L') lose[i] = true;
            else std::sscanf(token, "%d", &move[i]);
        }

        double normal[55], pending[55], nextNormal[55], nextPending[55];
        std::memset(normal, 0, sizeof(normal));
        std::memset(pending, 0, sizeof(pending));
        normal[0] = 1.0;
        int step;
        for (step = 0; step < turns; ++step) {
            std::memset(nextNormal, 0, sizeof(nextNormal));
            std::memset(nextPending, 0, sizeof(nextPending));
            nextNormal[m + 1] = normal[m + 1];
            for (i = 0; i <= m; ++i) {
                nextNormal[i] += pending[i];
                if (normal[i] == 0.0) continue;
                int d;
                for (d = 1; d <= 2; ++d) {
                    int landed = i + d;
                    if (landed > m + 1) landed = m + 1;
                    if (landed == m + 1) {
                        nextNormal[m + 1] += normal[i] * 0.5;
                    } else if (lose[landed]) {
                        nextPending[landed] += normal[i] * 0.5;
                    } else {
                        nextNormal[landed + move[landed]] += normal[i] * 0.5;
                    }
                }
            }
            std::memcpy(normal, nextNormal, sizeof(normal));
            std::memcpy(pending, nextPending, sizeof(pending));
        }
        double answer = normal[m + 1];
        if (answer > 0.5) std::printf("Bet for. %.4f\n", answer);
        else if (answer < 0.5) std::printf("Bet against. %.4f\n", answer);
        else std::printf("Push. 0.5000\n");
    }
    return 0;
}
