// POJ 1432 - Decoding Morse Sequences
// Model: claude-sonnet-5
// Approach: build a trie over the dictionary words' Morse encodings, then a
// prefix-count DP over the Morse sequence. dp[i] = number of ways to parse
// sequence[0..i) into a sequence of dictionary words. For every i with
// dp[i] > 0, walk the trie following sequence[i], sequence[i+1], ...; each
// trie node marking one or more word-ends contributes dp[i]*count to
// dp[that position]. Answer is dp[len(sequence)].
//
// Ambiguity / board notes (poj.org/bbs?problem_id=1432): the statement
// asserts "No word occurs in the dictionary more than once", but real judge
// data violates it (msg 51603, 51609, 99424, 170576). The correct handling,
// confirmed by two independently-written accepted solutions found for this
// exact problem (cnblogs.com/Sunshine-tcf/p/5805181.html, which keys a
// map<string,int> on the word's Morse string and does "mp[cur]++" for every
// word read with no dedup step; and abitofcs.blogspot.com's writeup of the
// SPOJ mirror MORSE, which states explicitly "duplicates are counted
// separately in the final answer") is to count EVERY occurrence of a
// dictionary word separately, NOT dedupe repeated words to one. A first
// submission that deduped via std::set got Wrong Answer; this is the fix.
// Distinct words that merely happen to share a Morse encoding are already
// handled the same way, naturally: each word read increments its trie
// node's end-count by 1, so X and "-..-"-encoding NA both add 1 to the same
// node regardless (board test: "-..-" with dict {X, NA} -> answer 2, msg
// 147521).
//
// Read every token (sequence, word count, words) into std::string via
// operator>>, never into a fixed-size char buffer - a board post
// (msg 353300) warns a fixed-size static trie can RE if undersized, and the
// same "data can exceed the stated bound" risk applies to any fixed
// word/sequence buffer, so all reads are dynamically sized. Trie itself is
// also a dynamically growing vector, not a fixed array.

#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

static const char* MORSE[26] = {
    ".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--",
    "-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int d;
    if (!(cin >> d)) return 0;
    while (d--) {
        string seq;
        cin >> seq;
        int len = (int)seq.size();

        int n;
        cin >> n;

        // build trie (vector-based, grows as needed)
        vector<int> ch0, ch1, cnt;
        ch0.push_back(-1); ch1.push_back(-1); cnt.push_back(0); // root = 0

        for (int wi = 0; wi < n; wi++) {
            string w;
            cin >> w;
            int node = 0;
            for (size_t k = 0; k < w.size(); k++) {
                const char* m = MORSE[w[k] - 'A'];
                for (const char* p = m; *p; p++) {
                    int c = (*p == '.') ? 0 : 1;
                    int nxt = (c == 0) ? ch0[node] : ch1[node];
                    if (nxt == -1) {
                        ch0.push_back(-1); ch1.push_back(-1); cnt.push_back(0);
                        nxt = (int)ch0.size() - 1;
                        if (c == 0) ch0[node] = nxt; else ch1[node] = nxt;
                    }
                    node = nxt;
                }
            }
            cnt[node] += 1;
        }

        vector<unsigned long long> dp(len + 1, 0ULL);
        dp[0] = 1;
        for (int i = 0; i < len; i++) {
            if (dp[i] == 0) continue;
            int node = 0;
            int j = i;
            while (j < len) {
                int c = (seq[j] == '.') ? 0 : 1;
                int nxt = (c == 0) ? ch0[node] : ch1[node];
                if (nxt == -1) break;
                node = nxt;
                j++;
                if (cnt[node] > 0) {
                    dp[j] += dp[i] * (unsigned long long)cnt[node];
                }
            }
        }

        cout << dp[len] << "\n";
    }
    return 0;
}
