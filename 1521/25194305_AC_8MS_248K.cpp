// POJ 1521 - Entropy
//
// For each input line (terminated by a line "END"), compute the length of an
// 8-bit ASCII encoding, the length of an optimal prefix-free (Huffman)
// encoding, and the ratio between them to one decimal place.
//
// Standard Huffman-length construction: count character frequencies, push
// them into a min-heap, and repeatedly pop the two smallest weights, sum
// them into a new node, add the sum to the running total bit count, and push
// the sum back — until one node remains. That running total is the number of
// bits the optimal code needs.
//
// Trap: a line using only one distinct character has no combining step to
// perform, so the loop above never executes and would report 0 bits, but a
// single symbol still needs 1 bit per occurrence to be encoded/decoded at
// all. That case is special-cased to `n` bits (the character count).
#include <cstdio>
#include <string>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    string s;
    while (cin >> s) {
        if (s == "END") break;
        int freq[256];
        for (int i = 0; i < 256; ++i) freq[i] = 0;
        int n = (int)s.size();
        for (int i = 0; i < n; ++i) freq[(unsigned char)s[i]]++;

        priority_queue<long, vector<long>, greater<long> > pq;
        int distinct = 0;
        for (int c = 0; c < 256; ++c) {
            if (freq[c] > 0) {
                pq.push((long)freq[c]);
                distinct++;
            }
        }

        long huff = 0;
        if (distinct <= 1) {
            // A single distinct character still needs one bit per occurrence;
            // the general Huffman construction would otherwise yield 0.
            huff = n;
        } else {
            while (pq.size() > 1) {
                long a = pq.top(); pq.pop();
                long b = pq.top(); pq.pop();
                huff += a + b;
                pq.push(a + b);
            }
        }

        long ascii = (long)n * 8;
        double ratio = (huff > 0) ? ((double)ascii / (double)huff) : 0.0;
        printf("%ld %ld %.1f\n", ascii, huff, ratio);
    }
    return 0;
}
