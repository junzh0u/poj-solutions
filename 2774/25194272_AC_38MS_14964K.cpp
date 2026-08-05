// POJ 2774 - Long Long Message
//
// Longest common substring of two strings of up to 100000 lowercase letters.
//
// Approach: build a suffix automaton of the first string, then feed the second
// string through it.  The SAM recognises exactly the substrings of s1, so
// walking s2 while tracking the length `l` of the current match gives, at every
// position, the longest suffix of the prefix of s2 read so far that is also a
// substring of s1.  On a missing transition follow suffix links (each hop drops
// to a shorter suffix, so `l` is reset to len[link]) until a transition exists
// or the root is reached.  The answer is the maximum `l` seen.  Both the build
// and the walk are O(n * 26), i.e. linear for a fixed alphabet.
//
// Why a SAM rather than a suffix array over s1#s2: it needs no sorting pass and
// no LCP array, and the memory limit (128MB) comfortably fits the 2*|s1| states
// with 26 int transitions each (~22MB of static arrays).
//
// Statement notes: the two lines are read with scanf("%s"), which also copes
// with CRLF input.  "The maximum length of the original text" is just the LCS
// length; when the strings share nothing the answer is 0, not an error.
#include <cstdio>
#include <cstring>

static const int MAXS = 200005; /* 2*|s1| states is the SAM bound */

static int sam_next[MAXS][26];
static int sam_link[MAXS];
static int sam_len[MAXS];
static int sam_size;
static int sam_last;

static void sam_init()
{
    sam_size = 1;
    sam_last = 0;
    sam_len[0] = 0;
    sam_link[0] = -1;
    memset(sam_next[0], -1, sizeof(sam_next[0]));
}

static void sam_extend(int c)
{
    int cur = sam_size++;
    sam_len[cur] = sam_len[sam_last] + 1;
    sam_link[cur] = -1;
    memset(sam_next[cur], -1, sizeof(sam_next[cur]));

    int p = sam_last;
    while (p != -1 && sam_next[p][c] == -1) {
        sam_next[p][c] = cur;
        p = sam_link[p];
    }
    if (p == -1) {
        sam_link[cur] = 0;
    } else {
        int q = sam_next[p][c];
        if (sam_len[p] + 1 == sam_len[q]) {
            sam_link[cur] = q;
        } else {
            int clone = sam_size++;
            sam_len[clone] = sam_len[p] + 1;
            sam_link[clone] = sam_link[q];
            memcpy(sam_next[clone], sam_next[q], sizeof(sam_next[q]));
            while (p != -1 && sam_next[p][c] == q) {
                sam_next[p][c] = clone;
                p = sam_link[p];
            }
            sam_link[q] = clone;
            sam_link[cur] = clone;
        }
    }
    sam_last = cur;
}

static char a[100005];
static char b[100005];

int main()
{
    a[0] = '\0';
    b[0] = '\0';
    if (scanf("%100004s", a) != 1)
        a[0] = '\0';
    if (scanf("%100004s", b) != 1)
        b[0] = '\0';

    sam_init();
    for (int i = 0; a[i]; ++i)
        sam_extend(a[i] - 'a');

    int v = 0, l = 0, best = 0;
    for (int i = 0; b[i]; ++i) {
        int c = b[i] - 'a';
        while (v != 0 && sam_next[v][c] == -1) {
            v = sam_link[v];
            l = sam_len[v];
        }
        if (sam_next[v][c] != -1) {
            v = sam_next[v][c];
            ++l;
        } else {
            v = 0;
            l = 0;
        }
        if (l > best)
            best = l;
    }

    printf("%d\n", best);
    return 0;
}
