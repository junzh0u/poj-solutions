// POJ 3874 - Working at the Restaurant
// Model: gpt-6-astra
// Simulate a FIFO queue using two stacks, tracked only by their sizes.
// Drop onto pile 2. Take from pile 1; refill it only when it is empty.
// Each plate is dropped once, moved at most once, and taken at most once.
// At most three lines per request and 3M plate movements are needed.
// Print blank lines between cases as required by the output specification.
#include <cstdio>
#include <algorithm>
int main() {
    int n;
    bool first = true;
    while (std::scanf("%d", &n) == 1 && n) {
        if (!first) std::printf("\n");
        first = false;
        int incoming = 0, outgoing = 0;
        for (int i = 0; i < n; ++i) {
            char op[10];
            int m;
            std::scanf("%9s%d", op, &m);
            if (op[0] == 'D') {
                std::printf("DROP 2 %d\n", m);
                incoming += m;
            } else {
                if (outgoing) {
                    int take = std::min(m, outgoing);
                    std::printf("TAKE 1 %d\n", take);
                    outgoing -= take;
                    m -= take;
                }
                if (m) {
                    std::printf("MOVE 2->1 %d\n", incoming);
                    outgoing = incoming;
                    incoming = 0;
                    std::printf("TAKE 1 %d\n", m);
                    outgoing -= m;
                }
            }
        }
    }
    return 0;
}
