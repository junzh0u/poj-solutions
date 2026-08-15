// POJ 3504 - Obfuscation
// Model: claude-sonnet-5
//
// Approach: A sentence was formed by shuffling the interior letters of each
// word (keeping first/last letters fixed) and then removing spaces. Given
// the concatenated string s and a dictionary, we must recover the original
// split into dictionary words, deciding unique / impossible / ambiguous.
//
// A dictionary word w of length L "matches" a substring of s of length L
// iff they share the same first char, same last char, and the multiset of
// interior characters (positions 1..L-2) is equal. Words of length 1 or 2
// have an empty interior, so they must match exactly.
//
// For each dictionary word we compute a fingerprint of its interior
// multiset using two independent randomized order-invariant hashes (sum of
// per-letter random 64-bit values), then bucket words by (length, first
// char, last char, fingerprint). For the input string we precompute prefix
// sums of the same random values so any substring's interior fingerprint is
// O(1) to compute.
//
// dp[i] = number of distinct ways (capped at 2, i.e. "0/1/>=2") to parse
// s[0..i) into a sequence of dictionary-matching words. dp[0]=1; for every
// starting position i with dp[i]>0 and every dictionary word-length L with
// i+L<=n, if the substring s[i..i+L) matches some bucket with k (1 or,
// capped, 2) distinct dictionary words, dp[i+L] += dp[i]*k (capped at 2).
// dp[n]==0 -> impossible, dp[n]>=2 -> ambiguous, dp[n]==1 -> unique, and we
// reconstruct the unique parse by walking backward: since the total count
// is exactly 1, at every position on the true parse there is exactly one
// incoming edge (j,L) with dp[j]==1 and bucket-count==1; any edge found
// with those two properties is therefore *the* edge, so we can rebuild the
// sentence directly.
//
// The board's ambiguity ("abcdefgh" with acbd/egfh/acbde/fgh, from message
// 95114) confirms ambiguity also arises purely from two different
// *segmentations* of the same string, not only from two words matching one
// segment -- the capped-count DP over all i naturally covers both, since it
// sums over every valid transition, not just within one fixed split point.
// Verified by hand: dp[8] gets a contribution of 1 from the 4+4 split
// (acbd,egfh) and 1 from the 5+3 split (acbde,fgh), giving 2 -> ambiguous,
// matching the posted answer. Message 202878's "mostof" case (most/msot/of
// vs msoto/f) similarly needs both a same-segment-multiple-words
// contribution (most and msot both match "most") and a competing split to
// reach the correct total of >=2 -> ambiguous.
// Message 100846's all-length-1-word case ("abcd" vs a,b,c,d) exercises the
// length<=2 (empty interior) special case explicitly.
//
// Complexity: for each test case, building the buckets is O(sum of word
// lengths); the DP scans each start position against each distinct
// dictionary word length (bucket lookup by exact (L,f,l) array index, then
// a map lookup within that bucket keyed by the two 64-bit fingerprints),
// comfortably within the limits (|s|<=1000, n<=10000 words, len<=100,
// <=100 testcases).

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

typedef unsigned long long ull;

static const ull RV1[26] = {
0x5714e7ffbb48e6e1ULL, 0x9a38d246c5e4f8e6ULL, 0xbc49fe4285d7f80cULL, 0x5c8b626408acd7adULL,
0x81bf5712d7becaa6ULL, 0xdd1b9f45cc95d15bULL, 0x92e5a7c8a419e4d1ULL, 0xccd7d9990f080e8dULL,
0x6ecd0d1295a0433dULL, 0x845eb320b23cbb1cULL, 0x1ab353691e348de1ULL, 0xef695f282784179bULL,
0xfe9c8f20420e0a01ULL, 0x3353d99c0a10eca2ULL, 0xae7abc14224cc4fdULL, 0x04153ed7e5a5a925ULL,
0x2cf4a2753ab106e5ULL, 0xb13ebc08a359a49cULL, 0xfcfdac73a6c2ae67ULL, 0x3713549d808d72c2ULL,
0xfd751da375c8dde7ULL, 0x9bde053962ad70eaULL, 0x9cbf622983ceb350ULL, 0x0cff4b9cf2518d71ULL,
0xb1ebce08f956546cULL, 0x8210a77ed075677cULL
};
static const ull RV2[26] = {
0x4592b9985e56004cULL, 0x22677c8a9711927fULL, 0x5abe6e1c1183dab6ULL, 0xc795023f6e08e6b6ULL,
0x1719229ddd90797dULL, 0x0e3a9f685fda79f8ULL, 0x1fcee28d0bc79788ULL, 0x8f334fd51d419aa2ULL,
0x188fb78addb4627eULL, 0x05da0f751aafd836ULL, 0x6628b51a04ea5474ULL, 0x7040e9d9ea54f4c5ULL,
0x49a72414500d9224ULL, 0xe7cccd495205e578ULL, 0x03a0d9c4400b9e9cULL, 0x97d14c5a122cb1aaULL,
0x9c45b5a89d037c61ULL, 0x38f792447744dfd9ULL, 0x13e172b5d0b1ec72ULL, 0xdb617e74fb3b98d6ULL,
0xe14bfc71114fd1ffULL, 0xb15361dad95598b9ULL, 0x067b49601627896bULL, 0xd654a4f6f6a00fa3ULL,
0xf8e5272666066b31ULL, 0x76b999e1e8f5990aULL
};

static const int MAXL = 101; // dictionary word length is 1..100

typedef pair<ull, ull> HKey;
typedef map<HKey, pair<int, string> > Bucket;

static Bucket buckets[MAXL][26][26];
static vector<int> touched;

static inline int bucketCode(int L, int f, int l) {
    return (L * 26 + f) * 26 + l;
}

int main() {
    int T;
    if (scanf("%d", &T) != 1) return 0;
    string s;
    char buf[1100];
    char wbuf[110];
    vector<char> present(MAXL, 0);
    while (T--) {
        scanf("%s", buf);
        s = buf;
        int n = (int)s.size();

        int nw;
        scanf("%d", &nw);

        // clear touched buckets from previous test case
        for (size_t t = 0; t < touched.size(); ++t) {
            int code = touched[t];
            int L = code / (26 * 26);
            int rem = code % (26 * 26);
            int f = rem / 26, l = rem % 26;
            buckets[L][f][l].clear();
        }
        touched.clear();
        fill(present.begin(), present.end(), 0);

        for (int w = 0; w < nw; ++w) {
            scanf("%s", wbuf);
            int L = (int)strlen(wbuf);
            int f = wbuf[0] - 'a';
            int l = wbuf[L - 1] - 'a';
            ull h1 = 0, h2 = 0;
            for (int k = 1; k + 1 < L; ++k) {
                int c = wbuf[k] - 'a';
                h1 += RV1[c];
                h2 += RV2[c];
            }
            present[L] = 1;
            Bucket &b = buckets[L][f][l];
            if (b.empty()) touched.push_back(bucketCode(L, f, l));
            HKey key(h1, h2);
            pair<int, string> &val = b[key];
            if (val.first == 0) {
                val.first = 1;
                val.second = wbuf;
            } else {
                val.first = 2; // saturate; second word text no longer needed
                val.second.clear();
            }
        }

        vector<int> lens;
        for (int L = 1; L < MAXL; ++L) if (present[L]) lens.push_back(L);

        // prefix sums of random values over s
        vector<ull> P1(n + 1, 0), P2(n + 1, 0);
        for (int k = 0; k < n; ++k) {
            int c = s[k] - 'a';
            P1[k + 1] = P1[k] + RV1[c];
            P2[k + 1] = P2[k] + RV2[c];
        }

        vector<int> dp(n + 1, 0);
        dp[0] = 1;
        for (int i = 0; i < n; ++i) {
            if (dp[i] == 0) continue;
            for (size_t li = 0; li < lens.size(); ++li) {
                int L = lens[li];
                if (i + L > n) continue;
                int f = s[i] - 'a';
                int l = s[i + L - 1] - 'a';
                Bucket &b = buckets[L][f][l];
                if (b.empty()) continue;
                ull h1, h2;
                if (L <= 2) { h1 = 0; h2 = 0; }
                else { h1 = P1[i + L - 1] - P1[i + 1]; h2 = P2[i + L - 1] - P2[i + 1]; }
                Bucket::iterator it = b.find(HKey(h1, h2));
                if (it == b.end()) continue;
                int cnt = it->second.first;
                int add = dp[i] * cnt;
                int nv = dp[i + L] + add;
                if (nv > 2) nv = 2;
                dp[i + L] = nv;
            }
        }

        if (dp[n] == 0) {
            printf("impossible\n");
        } else if (dp[n] >= 2) {
            printf("ambiguous\n");
        } else {
            // unique: reconstruct backward
            vector<string> words;
            int pos = n;
            while (pos > 0) {
                bool found = false;
                for (size_t li = 0; li < lens.size() && !found; ++li) {
                    int L = lens[li];
                    if (L > pos) continue;
                    int j = pos - L;
                    if (dp[j] != 1) continue;
                    int f = s[j] - 'a';
                    int l = s[pos - 1] - 'a';
                    Bucket &b = buckets[L][f][l];
                    if (b.empty()) continue;
                    ull h1, h2;
                    if (L <= 2) { h1 = 0; h2 = 0; }
                    else { h1 = P1[pos - 1] - P1[j + 1]; h2 = P2[pos - 1] - P2[j + 1]; }
                    Bucket::iterator it = b.find(HKey(h1, h2));
                    if (it == b.end()) continue;
                    if (it->second.first != 1) continue;
                    words.push_back(it->second.second);
                    pos = j;
                    found = true;
                }
                // found is guaranteed true given dp[n]==1's derivation
            }
            for (int i = (int)words.size() - 1; i >= 0; --i) {
                if (i != (int)words.size() - 1) printf(" ");
                printf("%s", words[i].c_str());
            }
            printf("\n");
        }
    }
    return 0;
}
