// POJ 3776 - Passing the Message
// Model: gpt-6.1-sol
// Build the maximum Cartesian tree with a decreasing stack in O(N) time.
// Its children are the tallest shorter kids before the nearest taller barrier.
#include <cstdio>
#include <cstring>
const int MAXN = 50005;
int h[MAXN], leftChild[MAXN], rightChild[MAXN], st[MAXN];
int main() {
    int t, n;
    if (scanf("%d", &t) != 1) return 0;
    for (int tc = 1; tc <= t; ++tc) {
        scanf("%d", &n);
        int top = 0;
        for (int i = 1; i <= n; ++i) {
            scanf("%d", &h[i]);
            leftChild[i] = rightChild[i] = 0;
            int last = 0;
            while (top && h[st[top - 1]] < h[i]) last = st[--top];
            leftChild[i] = last;
            if (top) rightChild[st[top - 1]] = i;
            st[top++] = i;
        }
        printf("Case %d:\n", tc);
        for (int i = 1; i <= n; ++i)
            printf("%d %d\n", leftChild[i], rightChild[i]);
    }
    return 0;
}
