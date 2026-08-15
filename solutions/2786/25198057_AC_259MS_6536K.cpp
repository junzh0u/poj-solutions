// POJ 2786 - Keep the Customer Satisfied
// Model: claude-sonnet-5
//
// Classic Moore-Hodgson algorithm for single-machine scheduling to
// minimize the number of late (rejected) jobs, equivalently maximize
// the number of accepted jobs that can all meet their due dates.
//
// Sort orders by non-decreasing due date. Scan in that order, maintaining
// a running total of processing time and a max-heap of the processing
// times of the currently "tentatively accepted" orders. After adding
// order i's q to both the heap and the running total, if the running
// total exceeds order i's due date, remove the largest q seen so far
// from the heap (it may or may not be order i itself) and subtract it
// from the running total. At the end the heap's size is the maximum
// number of orders that can be scheduled without any tardiness. This
// greedy exchange argument is a standard result (Hodgson's algorithm);
// the hints in the statement (schedule accepted jobs in EDD order, and
// that a larger-q/earlier-d job being accepted implies a smaller-q/
// later-d job is also accepted in some optimal solution) are exactly
// the exchange properties that make it correct.
//
// The board's accepted solutions consistently loop with
// while(scanf("%d", &n) != EOF) even though the statement only shows a
// single sample case, so the judge input holds multiple datasets read
// to EOF; this program follows that pattern and prints one line per
// dataset. n can be up to 800000, so I/O is done with a hand-rolled
// buffered reader instead of iostream/scanf per token.
//
// No statement ambiguity beyond the multi-dataset framing above: the
// running sum of q's is always trimmed back below the current due date
// (<= 2*10^6) whenever it would overflow it, so it stays well within
// the range of a 32-bit int and plain int arithmetic suffices.
#include <cstdio>
#include <algorithm>
#include <vector>

static inline int readInt(bool &ok) {
    int c = getchar();
    while (c != EOF && (c < '0' || c > '9') && c != '-') c = getchar();
    if (c == EOF) { ok = false; return 0; }
    ok = true;
    bool neg = false;
    if (c == '-') { neg = true; c = getchar(); }
    int x = 0;
    while (c >= '0' && c <= '9') {
        x = x * 10 + (c - '0');
        c = getchar();
    }
    return neg ? -x : x;
}

struct Order {
    int q, d;
};

bool cmpOrder(const Order &a, const Order &b) {
    return a.d < b.d;
}

int main() {
    static std::vector<Order> orders;
    static std::vector<int> heap; // max-heap via std::push_heap/pop_heap

    bool ok;
    for (;;) {
        int n = readInt(ok);
        if (!ok) break;
        orders.assign(n, Order());
        for (int i = 0; i < n; ++i) {
            orders[i].q = readInt(ok);
            orders[i].d = readInt(ok);
        }
        std::sort(orders.begin(), orders.end(), cmpOrder);

        heap.clear();
        heap.reserve(n);
        int total = 0;
        for (int i = 0; i < n; ++i) {
            heap.push_back(orders[i].q);
            std::push_heap(heap.begin(), heap.end());
            total += orders[i].q;
            if (total > orders[i].d) {
                std::pop_heap(heap.begin(), heap.end());
                total -= heap.back();
                heap.pop_back();
            }
        }
        printf("%d\n", (int)heap.size());
    }
    return 0;
}
