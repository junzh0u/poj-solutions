// POJ 1680 - Fork() Makes Trouble
// Model: gpt-5.6-terra
// Depth-first simulation of the fork/wait execution order.  Each call holds
// one process's private A value; its child completes before the parent loops.
#include <cstdio>

int n;
int wanted;
int lineNumber;
int nextPid;

bool simulate(int pid, int a, int firstLoop) {
    int loop;
    for (loop = firstLoop; loop < n; ++loop) {
        ++lineNumber;
        if (lineNumber == wanted) {
            std::printf("Loop %d: Process ID=%d\n", loop, pid);
            return true;
        }

        a += 7;
        ++nextPid;
        ++lineNumber;
        if (lineNumber == wanted) {
            std::printf("Process ID=%d, A=%d\n", nextPid, a);
            return true;
        }

        if (simulate(nextPid, a, loop + 1)) return true;
    }
    return false;
}

int main() {
    int tests;
    if (std::scanf("%d", &tests) != 1) return 0;
    while (tests--) {
        std::scanf("%d%d", &n, &wanted);
        lineNumber = 0;
        nextPid = 1000;
        simulate(1000, 0, 0);
    }
    return 0;
}
