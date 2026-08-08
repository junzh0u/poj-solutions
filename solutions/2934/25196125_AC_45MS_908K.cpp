// POJ 2934 - Automatic Correction of Misspellings
// Model: gpt-5.6-terra
// For each query, enumerate every string one permitted edit away and look it
// up in a compact chained hash table.  The minimum dictionary position wins.
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
using namespace std;

const int MOD = 32771;
vector<string> word;
vector<int> nextPos;
int head[MOD];

unsigned int hashWord(const char *s) {
    unsigned int h = 2166136261U;
    while (*s) { h ^= (unsigned char)*s++; h *= 16777619U; }
    return h;
}

int findWord(const char *s) {
    int h = (int)(hashWord(s) % MOD);
    for (int p = head[h]; p != -1; p = nextPos[p])
        if (word[p] == s) return p;
    return -1;
}

void consider(const char *s, int &best) {
    int p = findWord(s);
    if (p != -1 && (best == -1 || p < best)) best = p;
}

int main() {
    int n, q;
    if (scanf("%d", &n) != 1) return 0;
    word.resize(n); nextPos.resize(n);
    memset(head, -1, sizeof(head));
    char in[27], buf[27];
    for (int i = 0; i < n; ++i) {
        scanf("%25s", in); word[i] = in;
        int h = (int)(hashWord(in) % MOD);
        nextPos[i] = head[h]; head[h] = i;
    }
    scanf("%d", &q);
    while (q--) {
        scanf("%25s", in);
        int len = (int)strlen(in);
        if (findWord(in) != -1) {
            printf("%s is correct\n", in);
            continue;
        }
        int best = -1;
        // The query has one extra letter.
        for (int i = 0; i < len; ++i) {
            memcpy(buf, in, i); memcpy(buf + i, in + i + 1, len - i);
            consider(buf, best);
        }
        // One letter is wrong.
        for (int i = 0; i < len; ++i) for (char c = 'a'; c <= 'z'; ++c) {
            if (c == in[i]) continue;
            strcpy(buf, in); buf[i] = c; consider(buf, best);
        }
        // Two different adjacent letters are reversed.
        for (int i = 0; i + 1 < len; ++i) if (in[i] != in[i + 1]) {
            strcpy(buf, in); char t = buf[i]; buf[i] = buf[i + 1]; buf[i + 1] = t;
            consider(buf, best);
        }
        // The query is missing one letter.  Dictionary words have length <= 25.
        if (len < 25) for (int i = 0; i <= len; ++i) for (char c = 'a'; c <= 'z'; ++c) {
            memcpy(buf, in, i); buf[i] = c; memcpy(buf + i + 1, in + i, len - i + 1);
            consider(buf, best);
        }
        if (best == -1) printf("%s is unknown\n", in);
        else printf("%s is a misspelling of %s\n", in, word[best].c_str());
    }
    return 0;
}
