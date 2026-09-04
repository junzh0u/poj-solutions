// POJ 1348 - Computing
// Model: claude-sonnet-5
// Approach: exhaustive search. Repeatedly pick any two of the current
// remaining values, combine them with +, -, *, / (both orders for the
// non-commutative ops) into one new value, and recurse until a single
// value remains; check it against n5 within a small epsilon. Since the
// pick-any-two-remaining-values recursion ranges over every possible
// binary-tree shape and every assignment of n1..n4 to leaves (the
// board's "order can be permuted" and "don't forget pairing two
// sub-results" reminders), no separate permutation loop is needed.
// Division uses real (floating point) division, per the discuss board:
// POJ's judge data includes cases that only pass under float division,
// not integer division. Only exact zero denominators are rejected;
// multiplying by zero is fine. Intermediate results may be negative,
// non-integer, or exceed 100 (n5 itself can exceed 100 in the judge
// data, despite the stated 0<=ni<=100 bound), so plain doubles with no
// bound-based array sizing are used throughout.
#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;

static double target;
static const double EPS = 1e-6;

static bool dfs(vector<double> v) {
    int n = (int)v.size();
    if (n == 1) {
        return fabs(v[0] - target) < EPS;
    }
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double a = v[i], b = v[j];
            vector<double> nv;
            for (int k = 0; k < n; k++) {
                if (k != i && k != j) nv.push_back(v[k]);
            }
            nv.push_back(a + b);
            if (dfs(nv)) return true;
            nv.back() = a - b;
            if (dfs(nv)) return true;
            nv.back() = b - a;
            if (dfs(nv)) return true;
            nv.back() = a * b;
            if (dfs(nv)) return true;
            if (b != 0.0) {
                nv.back() = a / b;
                if (dfs(nv)) return true;
            }
            if (a != 0.0) {
                nv.back() = b / a;
                if (dfs(nv)) return true;
            }
        }
    }
    return false;
}

int main() {
    int n1;
    while (scanf("%d", &n1) == 1 && n1 != -1) {
        int n2, n3, n4, n5;
        scanf("%d %d %d %d", &n2, &n3, &n4, &n5);
        target = (double)n5;
        vector<double> v;
        v.push_back((double)n1);
        v.push_back((double)n2);
        v.push_back((double)n3);
        v.push_back((double)n4);
        bool ok = dfs(v);
        printf("%d %d %d %d %d %s\n", n1, n2, n3, n4, n5, ok ? "OK!" : "NO!");
    }
    return 0;
}
