// POJ 3140 - Contestants Division
// Model: gpt-5.6-terra
// Approach: The communication graph is a tree. Root it iteratively, then sum
// each subtree bottom-up. Removing a parent edge leaves sums s and total-s.
// Use 64-bit totals because up to 100000 universities each hold 100000000 students.
#include <cstdio>

const int MAXN = 100000 + 5;
const int MAXE = 2000000 + 5;

int head[MAXN], to[MAXE], nextEdge[MAXE];
int parentNode[MAXN], order[MAXN];
long long people[MAXN], subtree[MAXN];
int edgeCount;

class FastInput {
public:
    FastInput() : pos(0), len(0) {}

    bool readInt(int &value) {
        int c;
        do {
            c = getChar();
            if (c == EOF) return false;
        } while (c <= ' ');
        value = 0;
        do {
            value = value * 10 + c - '0';
            c = getChar();
        } while (c > ' ');
        return true;
    }

private:
    char buffer[1 << 16];
    int pos, len;

    int getChar() {
        if (pos == len) {
            len = (int)std::fread(buffer, 1, sizeof(buffer), stdin);
            pos = 0;
            if (len == 0) return EOF;
        }
        return buffer[pos++];
    }
};

void addEdge(int u, int v) {
    to[edgeCount] = v;
    nextEdge[edgeCount] = head[u];
    head[u] = edgeCount++;
}

long long absoluteValue(long long value) {
    return value < 0 ? -value : value;
}

int main() {
    FastInput input;
    int n, m, testCase = 0;
    while (input.readInt(n) && input.readInt(m) && (n != 0 || m != 0)) {
        int i;
        long long total = 0;
        for (i = 1; i <= n; ++i) {
            int value;
            input.readInt(value);
            people[i] = value;
            total += people[i];
            head[i] = -1;
            parentNode[i] = 0;
        }
        edgeCount = 0;
        for (i = 0; i < m; ++i) {
            int u, v;
            input.readInt(u);
            input.readInt(v);
            addEdge(u, v);
            addEdge(v, u);
        }

        int begin = 0, end = 0;
        order[end++] = 1;
        parentNode[1] = -1;
        while (begin < end) {
            int u = order[begin++];
            int e;
            for (e = head[u]; e != -1; e = nextEdge[e]) {
                int v = to[e];
                if (v != parentNode[u]) {
                    parentNode[v] = u;
                    order[end++] = v;
                }
            }
        }

        for (i = 1; i <= n; ++i) subtree[i] = people[i];
        long long answer = total;
        for (i = end - 1; i > 0; --i) {
            int u = order[i];
            long long difference = absoluteValue(total - 2 * subtree[u]);
            if (difference < answer) answer = difference;
            subtree[parentNode[u]] += subtree[u];
        }
        std::printf("Case %d: %lld\n", ++testCase, answer);
    }
    return 0;
}
