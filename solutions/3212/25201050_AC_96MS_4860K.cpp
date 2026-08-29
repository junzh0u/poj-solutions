// POJ 3212 - Rescue Alice
// Model: claude-sonnet-5
// Approach: Chebyshev distance max(|dx|,|dy|) equals half the Manhattan
// distance under the 45-degree rotation (x,y) -> (u,v) = (x+y, x-y).
// So the sum of Chebyshev distances from point i to all others equals
// (sum_j |u_i-u_j| + sum_j |v_i-v_j|) / 2, and the two sums separate by
// dimension. Sort by u (resp. v) and use prefix sums to get, for every
// point in O(1) after an O(n log n) sort, the total absolute deviation
// from all others. Take the minimum of (Su[i]+Sv[i]) over i, divide by 2
// (always exact, since it's a sum of even terms 2*Chebyshev).
// No statement ambiguity; board confirms this exact transform.
#include <cstdio>
#include <algorithm>
using namespace std;

typedef long long ll;

int n;
ll u[100005], v[100005];
ll su[100005], sv[100005];
int idxU[100005], idxV[100005];

bool cmpU(int a, int b) { return u[a] < u[b]; }
bool cmpV(int a, int b) { return v[a] < v[b]; }

void computeSums(ll* val, int* order, ll* result) {
    // order sorted by val ascending
    static ll prefix[100006];
    prefix[0] = 0;
    for (int k = 0; k < n; k++) {
        prefix[k+1] = prefix[k] + val[order[k]];
    }
    ll total = prefix[n];
    for (int k = 0; k < n; k++) {
        ll x = val[order[k]];
        ll leftCount = k;
        ll leftSum = prefix[k];
        ll rightCount = n - k - 1;
        ll rightSum = total - prefix[k+1];
        ll s = (x * leftCount - leftSum) + (rightSum - x * rightCount);
        result[order[k]] = s;
    }
}

int main() {
    if (scanf("%d", &n) != 1) return 0;
    for (int i = 0; i < n; i++) {
        ll x, y;
        scanf("%lld %lld", &x, &y);
        u[i] = x + y;
        v[i] = x - y;
        idxU[i] = i;
        idxV[i] = i;
    }
    sort(idxU, idxU + n, cmpU);
    sort(idxV, idxV + n, cmpV);
    computeSums(u, idxU, su);
    computeSums(v, idxV, sv);

    ll best = -1;
    for (int i = 0; i < n; i++) {
        ll total = su[i] + sv[i];
        if (best == -1 || total < best) best = total;
    }
    if (n <= 1) best = 0;
    printf("%lld\n", best / 2);
    return 0;
}
