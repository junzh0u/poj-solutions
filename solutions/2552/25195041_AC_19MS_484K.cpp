// POJ 2552 - Assistance Required
// Model: claude-sonnet-5
// Approach: simulate the Ulm sieve directly. The first remaining number is
// always "lucky" (freed) and every step-th number behind it (step = the
// lucky number's value) is removed from the remaining queue. Repeat: take
// the new front as the next lucky number, sieve again with its value as the
// step. Rebuild the remaining list with one linear pass per round instead of
// repeated erase() calls. The discuss board confirms the n-th (n<=3000)
// lucky number never exceeds 34000, so sieving candidates 2..40000 gives
// ample margin; precompute the first 3000 lucky numbers once, then answer
// each query (read until n=0) by table lookup.
#include <cstdio>
#include <vector>
using namespace std;

int main() {
    const int N = 40000;
    const int NEED = 3000;
    vector<int> v;
    v.reserve(N);
    for (int i = 2; i <= N; ++i) v.push_back(i);

    vector<int> lucky;
    lucky.reserve(NEED);
    while (!v.empty() && (int)lucky.size() < NEED) {
        int step = v[0];
        lucky.push_back(step);
        int sz = (int)v.size();
        vector<int> nv;
        nv.reserve(sz);
        int count = 0;
        for (int i = 1; i < sz; ++i) {
            ++count;
            if (count == step) {
                count = 0;
                continue;
            }
            nv.push_back(v[i]);
        }
        v.swap(nv);
    }

    int n;
    while (scanf("%d", &n) == 1 && n != 0) {
        printf("%d\n", lucky[n - 1]);
    }
    return 0;
}
