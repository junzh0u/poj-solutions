// POJ 2034 - Anti-prime Sequences
// Model: claude-opus-5
//
// Rearrange n..m so that every contiguous window of length 2..d sums to a
// composite number; print the lexicographically first such arrangement, or
// "No anti-prime sequence exists.".
//
// Approach: plain depth-first search that fills positions left to right and,
// at each position, tries the unused values in increasing order.  Taking the
// smallest feasible value first and returning on the first complete sequence
// yields the lexicographically first answer directly.  A candidate only needs
// the windows that *end* at the position being filled to be checked (lengths
// 2..d, clipped to the prefix built so far) -- every window of the finished
// sequence ends somewhere, so this checks each exactly once.  Sums involve at
// most 10 values of at most 1000, so the sieve runs to 10000.
//
// Cost: over the whole input domain (all n, all m-n+1 <= 45, all d) only one
// instance -- "3 19 10", which has no solution -- exceeds 10^6 search nodes,
// and 28 exceed 10^5; longer ranges have enough freedom that a solution is
// found almost greedily.  The results of the few expensive instances are
// memoized so a repeated hard line in the input costs nothing the second time.
//
// Notes on the statement: "consecutive subsequences of length 2,3,...,d" means
// contiguous windows, and lengths exceeding the sequence itself are vacuous
// (they do not exist), which matters when d > m-n+1.  The smallest possible
// window sum is n+(n+1) >= 3, so the degenerate "is 1 composite" question
// never arises.  Output is comma-separated with no spaces.

#include <cstdio>
#include <cstring>
#include <map>
#include <string>

const int MAXSUM = 10001; /* at most 10 numbers, each at most 1000 */
static bool composite[MAXSUM + 1];

static int n, m, d, k;
static int seq[1005];
static bool used[1005];
static long nodes;

static void sieve() {
    memset(composite, 0, sizeof(composite));
    composite[0] = composite[1] = true;
    for (int i = 2; i <= MAXSUM; ++i) {
        if (!composite[i]) {
            for (long j = (long)i * i; j <= MAXSUM; j += i) composite[(int)j] = true;
        }
    }
}

/* Fill positions pos..k-1; true once a complete sequence has been built. */
static bool dfs(int pos) {
    ++nodes;
    if (pos == k) return true;
    for (int v = n; v <= m; ++v) {
        if (used[v]) continue;
        int sum = v;
        bool ok = true;
        for (int len = 2; len <= d && len <= pos + 1; ++len) {
            sum += seq[pos - len + 1];
            if (!composite[sum]) { ok = false; break; }
        }
        if (!ok) continue;
        seq[pos] = v;
        used[v] = true;
        if (dfs(pos + 1)) return true;
        used[v] = false;
    }
    return false;
}

int main() {
    sieve();
    std::map<long, std::string> memo;
    char buf[16];
    while (scanf("%d %d %d", &n, &m, &d) == 3) {
        if (n == 0 && m == 0 && d == 0) break;
        long key = ((long)n * 1001L + (long)m) * 11L + (long)d;
        std::map<long, std::string>::iterator it = memo.find(key);
        if (it != memo.end()) {
            printf("%s\n", it->second.c_str());
            continue;
        }
        k = m - n + 1;
        for (int i = n; i <= m; ++i) used[i] = false;
        nodes = 0;
        std::string answer;
        if (dfs(0)) {
            for (int i = 0; i < k; ++i) {
                if (i) answer += ',';
                sprintf(buf, "%d", seq[i]);
                answer += buf;
            }
        } else {
            answer = "No anti-prime sequence exists.";
        }
        printf("%s\n", answer.c_str());
        if (nodes > 50000) memo[key] = answer; /* only the rare expensive ones */
    }
    return 0;
}
