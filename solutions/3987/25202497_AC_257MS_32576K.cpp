// POJ 3987 - Computer Virus on Planet Pandora
// Model: claude-sonnet-5
// Aho-Corasick automaton over the (at most 250) virus patterns; each node's
// "match mask" (a 250-bit set of pattern indices, stored as 8 x uint32) is
// merged with its failure-link ancestor's mask once, during the BFS that
// turns the trie into a full-transition automaton, so scanning text is O(1)
// per character with no runtime suffix-chain walk (the classic TLE trap on
// this problem, per the discuss board). A "visited" stamp per node skips
// re-ORing a mask into the running answer once that state has already been
// seen, since the memory/time limits here are notoriously tight (2000ms and
// 128MB on the HDU mirror of the same problem, vs 1000ms/64MB on POJ).
// The program is guaranteed to be at most 5,100,000 characters whether
// compressed or decompressed, so it is simply decompressed in full and the
// automaton is run once forward and once again feeding characters from the
// end backward (equivalent to running on the reversed string, without ever
// materializing it) to test the "substring of the reverse" condition.
#include <cstdio>
#include <cstring>

static const int MAXNODES = 250005;
static const int MAXTEXT = 5100005;
static const int MW = 8; // 8 * 32 = 256 bits, enough for 250 patterns

static int ch[MAXNODES][26];
static int fail_[MAXNODES];
static unsigned int mask[MAXNODES][MW];
static bool visited[MAXNODES];
static int bfsq[MAXNODES];
static int nodeCount;

static char rawline[MAXTEXT];
static char text[MAXTEXT];
static char pat[1005];

static inline int gc() {
    static char buf[1 << 20];
    static int len = 0, pos = 0;
    if (pos == len) {
        len = (int)fread(buf, 1, sizeof(buf), stdin);
        pos = 0;
        if (!len) return -1;
    }
    return buf[pos++];
}

static inline void readToken(char *dst) {
    int c = gc();
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t') c = gc();
    int i = 0;
    while (c != -1 && c != ' ' && c != '\n' && c != '\r' && c != '\t') {
        dst[i++] = (char)c;
        c = gc();
    }
    dst[i] = '\0';
}

static inline int readInt() {
    int c = gc();
    while (c == ' ' || c == '\n' || c == '\r' || c == '\t') c = gc();
    int sign = 1;
    if (c == '-') { sign = -1; c = gc(); }
    int x = 0;
    while (c >= '0' && c <= '9') { x = x * 10 + (c - '0'); c = gc(); }
    return x * sign;
}

static inline int newNode() {
    int u = nodeCount++;
    memset(ch[u], -1, sizeof(ch[u]));
    memset(mask[u], 0, sizeof(mask[u]));
    return u;
}

int main() {
    int T = readInt();
    while (T--) {
        int n = readInt();
        nodeCount = 0;
        newNode(); // root = 0

        for (int i = 0; i < n; i++) {
            readToken(pat);
            int cur = 0;
            for (int j = 0; pat[j]; j++) {
                int c = pat[j] - 'A';
                if (ch[cur][c] == -1) ch[cur][c] = newNode();
                cur = ch[cur][c];
            }
            mask[cur][i >> 5] |= (1U << (i & 31));
        }

        // Build fail links and convert trie into a full-transition automaton.
        int head = 0, tail = 0;
        fail_[0] = 0;
        for (int c = 0; c < 26; c++) {
            if (ch[0][c] == -1) {
                ch[0][c] = 0;
            } else {
                fail_[ch[0][c]] = 0;
                bfsq[tail++] = ch[0][c];
            }
        }
        while (head < tail) {
            int u = bfsq[head++];
            for (int w = 0; w < MW; w++) mask[u][w] |= mask[fail_[u]][w];
            for (int c = 0; c < 26; c++) {
                int v = ch[u][c];
                if (v == -1) {
                    ch[u][c] = ch[fail_[u]][c];
                } else {
                    fail_[v] = ch[fail_[u]][c];
                    bfsq[tail++] = v;
                }
            }
        }

        // Read and decompress the program.
        readToken(rawline);
        int len = 0;
        for (int i = 0; rawline[i];) {
            if (rawline[i] == '[') {
                i++;
                long num = 0;
                while (rawline[i] >= '0' && rawline[i] <= '9') {
                    num = num * 10 + (rawline[i] - '0');
                    i++;
                }
                char letter = rawline[i];
                i++;     // letter
                i++;     // ']'
                for (long k = 0; k < num; k++) text[len++] = letter;
            } else {
                text[len++] = rawline[i++];
            }
        }

        memset(visited, 0, sizeof(bool) * (size_t)nodeCount);
        unsigned int res[MW];
        memset(res, 0, sizeof(res));

        int state = 0;
        for (int i = 0; i < len; i++) {
            state = ch[state][text[i] - 'A'];
            if (!visited[state]) {
                visited[state] = true;
                for (int w = 0; w < MW; w++) res[w] |= mask[state][w];
            }
        }

        state = 0;
        for (int i = len - 1; i >= 0; i--) {
            state = ch[state][text[i] - 'A'];
            if (!visited[state]) {
                visited[state] = true;
                for (int w = 0; w < MW; w++) res[w] |= mask[state][w];
            }
        }

        int count = 0;
        for (int i = 0; i < n; i++) {
            if (res[i >> 5] & (1U << (i & 31))) count++;
        }
        printf("%d\n", count);
    }
    return 0;
}
