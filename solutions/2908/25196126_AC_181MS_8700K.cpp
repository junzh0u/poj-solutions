// POJ 2908 - Quantum
// Model: gpt-5.6-terra
// Each binary word is a vertex.  An operation is a directed weighted edge,
// represented with masks so applying it is constant time.  Dijkstra stops only
// when the requested target is extracted from the heap.
// The discuss board confirms that early discovery of the target is unsound and
// that avoiding string states and stale priority-queue entries is important.
#include <cstdio>
#include <cstring>

const int MAXS = 1 << 20;

int dista[MAXS];
int heapNode[MAXS + 1];
int heapPos[MAXS];
unsigned int variableMask[32], flipMask[32], setMask[32];
int cost[32], opCount;
int heapSize;

void swapHeap(int a, int b) {
    int t = heapNode[a]; heapNode[a] = heapNode[b]; heapNode[b] = t;
    heapPos[heapNode[a]] = a;
    heapPos[heapNode[b]] = b;
}

void siftUp(int p) {
    while (p > 1 && dista[heapNode[p]] < dista[heapNode[p / 2]]) {
        swapHeap(p, p / 2);
        p /= 2;
    }
}

void siftDown(int p) {
    for (;;) {
        int q = p, l = p * 2, r = l + 1;
        if (l <= heapSize && dista[heapNode[l]] < dista[heapNode[q]]) q = l;
        if (r <= heapSize && dista[heapNode[r]] < dista[heapNode[q]]) q = r;
        if (q == p) break;
        swapHeap(p, q);
        p = q;
    }
}

void decreaseKey(int v) {
    if (heapPos[v] == 0) {
        heapNode[++heapSize] = v;
        heapPos[v] = heapSize;
    }
    siftUp(heapPos[v]);
}

int extractMin() {
    int v = heapNode[1];
    heapPos[v] = -1;
    heapNode[1] = heapNode[heapSize--];
    if (heapSize) {
        heapPos[heapNode[1]] = 1;
        siftDown(1);
    }
    return v;
}

int shortestPath(int start, int goal, int states) {
    std::memset(dista, 0x3f, states * sizeof(int));
    std::memset(heapPos, 0, states * sizeof(int));
    heapSize = 0;
    dista[start] = 0;
    decreaseKey(start);
    while (heapSize) {
        int u = extractMin();
        if (u == goal) return dista[u];
        for (int i = 0; i < opCount; ++i) {
            int v = ((unsigned int)u & variableMask[i]) ^ flipMask[i];
            v |= setMask[i];
            if (heapPos[v] != -1 && dista[v] > dista[u] + cost[i]) {
                dista[v] = dista[u] + cost[i];
                decreaseKey(v);
            }
        }
    }
    return -1;
}

int parseWord(const char *s, int length) {
    int value = 0;
    for (int i = 0; i < length; ++i) value = (value << 1) | (s[i] - '0');
    return value;
}

int main() {
    int cases;
    if (std::scanf("%d", &cases) != 1) return 0;
    while (cases--) {
        int length, wordCount;
        std::scanf("%d%d%d", &length, &opCount, &wordCount);
        for (int i = 0; i < opCount; ++i) {
            char s[21];
            std::scanf("%s%d", s, &cost[i]);
            variableMask[i] = flipMask[i] = setMask[i] = 0;
            for (int j = 0; j < length; ++j) {
                unsigned int bit = 1U << (length - 1 - j);
                if (s[j] == 'N' || s[j] == '0') variableMask[i] |= bit;
                else if (s[j] == 'F') { variableMask[i] |= bit; flipMask[i] |= bit; }
                else if (s[j] == 'S') setMask[i] |= bit;
            }
        }
        for (int i = 0; i < wordCount; ++i) {
            char from[21], to[21];
            std::scanf("%s%s", from, to);
            int answer = shortestPath(parseWord(from, length), parseWord(to, length), 1 << length);
            if (i) std::putchar(' ');
            if (answer < 0) std::printf("NP"); else std::printf("%d", answer);
        }
        std::putchar('\n');
    }
    return 0;
}
