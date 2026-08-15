// POJ 2637 - WorstWeather Ever
// Model: claude-sonnet-5
//
// For a query (Y,X) with Y<X we must classify the statement
// "r(X) <= r(Y) and every year Z strictly between them has r(Z) < r(X)"
// as true / maybe / false given partial knowledge of rainfall per year.
//
// Let maxInterior be the maximum known rainfall among years strictly
// between Y and X (or -1, a sentinel below any legal value >=1, if none
// are known). Let vX/vY be the known rainfall of X/Y, if known.
//
// - If X is known: the statement can never be forced true if
//   maxInterior >= vX (some known interior year already violates the
//   strict-less condition, unfixable) or if Y is known and vY < vX
//   (fixed values already violate r(X)<=r(Y)). Either makes it FALSE.
// - If X is unknown but Y is known: any assignment must satisfy
//   maxInterior < r(X) <= vY, which is only possible when
//   maxInterior < vY; otherwise it's FALSE (this is the case the
//   board's message 168579/349080 thread argues over: Y known=10,
//   an interior known year =10 forces r(X) to be both >10 and <=10,
//   impossible, hence FALSE regardless of X being unlisted).
// - If X is unknown and Y is unknown, it's always satisfiable (pick
//   both large enough), never FALSE from this pair alone.
//
// Otherwise (not FALSE): if every year in [Y,X] is known (which,
// given the years are strictly increasing integers, happens exactly
// when index(X)-index(Y) == X-Y for both endpoints found), the
// conditions above being non-violated is exactly the TRUE condition
// (all data fixed, no freedom left). If the range isn't fully known,
// it's MAYBE (the free years can always be filled to make it true
// once the fixed/known part doesn't already contradict).
//
// RMQ over known rainfall via a static sparse table (no updates),
// binary search (manual, on the strictly increasing year array) to
// locate Y, X and the interior index range. All quantities fit in a
// 32-bit int (years in [-1e9,1e9], so X-Y <= 2e9 < INT_MAX; rain in
// [1,1e9]), so no 64-bit types are used at all.

#include <cstdio>
#include <cstring>

const int MAXN = 50005;
const int LOG = 17; // 2^17 > 50000

int n, m;
int Yr[MAXN], Ra[MAXN];
int sp[LOG][MAXN];
int lg2[MAXN];

void buildSparse() {
    for (int i = 0; i < n; ++i) sp[0][i] = Ra[i];
    for (int k = 1; (1 << k) <= n; ++k) {
        int half = 1 << (k - 1);
        for (int i = 0; i + (1 << k) <= n; ++i) {
            int a = sp[k - 1][i];
            int b = sp[k - 1][i + half];
            sp[k][i] = (a > b) ? a : b;
        }
    }
}

// max over indices [l, r] inclusive; assumes 0 <= l <= r < n
int rangeMax(int l, int r) {
    int len = r - l + 1;
    int k = lg2[len];
    int a = sp[k][l];
    int b = sp[k][r - (1 << k) + 1];
    return (a > b) ? a : b;
}

// first index idx such that Yr[idx] >= val; returns n if none
int lowerBoundIdx(int val) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (Yr[mid] >= val) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

// first index idx such that Yr[idx] > val; returns n if none
int upperBoundIdx(int val) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = (lo + hi) / 2;
        if (Yr[mid] > val) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

int main() {
    lg2[1] = 0;
    for (int i = 2; i < MAXN; ++i) lg2[i] = lg2[i / 2] + 1;

    bool firstCase = true;
    while (scanf("%d", &n) == 1) {
        if (n == 0) {
            scanf("%d", &m);
            break; // n=0 and m=0 terminator; do not process
        }
        for (int i = 0; i < n; ++i) {
            scanf("%d %d", &Yr[i], &Ra[i]);
        }
        buildSparse();
        scanf("%d", &m);

        if (!firstCase) printf("\n");
        firstCase = false;

        for (int q = 0; q < m; ++q) {
            int Y, X;
            scanf("%d %d", &Y, &X);

            // locate Y, X in the known-year array
            int posY = lowerBoundIdx(Y);
            bool yKnown = (posY < n && Yr[posY] == Y);
            int posX = lowerBoundIdx(X);
            bool xKnown = (posX < n && Yr[posX] == X);

            // interior known years strictly between Y and X: indices [lo, hi]
            int lo = upperBoundIdx(Y);
            int hi = lowerBoundIdx(X) - 1;
            int maxInterior = -1; // sentinel: less than any legal rain value (>=1)
            if (lo <= hi) maxInterior = rangeMax(lo, hi);

            bool isFalse = false;
            if (xKnown) {
                int vX = Ra[posX];
                if (maxInterior >= vX) isFalse = true;
                if (yKnown && Ra[posY] < vX) isFalse = true;
            } else {
                if (yKnown) {
                    int vY = Ra[posY];
                    if (maxInterior >= vY) isFalse = true;
                }
            }

            const char *ans;
            if (isFalse) {
                ans = "false";
            } else {
                bool fullRangeKnown = yKnown && xKnown && (posX - posY == X - Y);
                ans = fullRangeKnown ? "true" : "maybe";
            }
            printf("%s\n", ans);
        }
    }
    return 0;
}
