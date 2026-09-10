// POJ 2994 - Failing Roads
// Model: claude-sonnet-5
//
// Each scenario builds a cograph via disjoint union (U) and complement (C)
// of expressions rooted at single vertices (V). We want, for the final
// graph, the maximum independent set (max towns with no maintained road
// between any two of them).
//
// Standard cograph facts, tracked bottom-up while parsing:
//   V:        alpha = 1, omega = 1                (alpha = max independent
//                                                    set, omega = max clique)
//   U e1 e2:  alpha = alpha1 + alpha2, omega = max(omega1, omega2)
//             (no edges between the two parts, so independent sets add and
//              a clique must live entirely on one side)
//   C e:      alpha(G) = omega(e), omega(G) = alpha(e)
//             (complementing swaps independent sets and cliques)
// The vertex count itself is never needed by these recurrences.
//
// Parsing is done iteratively with an explicit stack of "frames" (a U frame
// waiting for 2 children, a C frame waiting for 1) instead of recursion:
// the discuss board reports this problem is tight on time/stack when
// solved recursively by value, and a chain of nested C's can be up to
// ~100000 deep (200000 character limit / 2 chars per token), which risks
// stack overflow under naive recursion. The iterative version is O(line
// length) with no recursion at all.
//
// Tokens are always the single characters 'V', 'U', 'C'; every other
// character (spaces, '\r', etc.) is skipped, which also makes CRLF input
// harmless without explicit trimming.
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

struct Val { int a, o; };
struct Frame { char type; int got; Val child[2]; };

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::string line;
    std::vector<Frame> stk;
    stk.reserve(1 << 17);
    while (std::getline(std::cin, line)) {
        stk.clear();
        Val finalVal;
        finalVal.a = -1;
        finalVal.o = -1;
        bool got = false;
        for (size_t i = 0; i < line.size(); ++i) {
            char ch = line[i];
            Val val;
            if (ch == 'V') {
                val.a = 1;
                val.o = 1;
            } else if (ch == 'U') {
                Frame f;
                f.type = 'U';
                f.got = 0;
                stk.push_back(f);
                continue;
            } else if (ch == 'C') {
                Frame f;
                f.type = 'C';
                f.got = 0;
                stk.push_back(f);
                continue;
            } else {
                continue;
            }

            // Resolve val up through any frames it completes.
            while (true) {
                if (stk.empty()) {
                    finalVal = val;
                    got = true;
                    break;
                }
                Frame &f = stk.back();
                f.child[f.got++] = val;
                int need = (f.type == 'U') ? 2 : 1;
                if (f.got == need) {
                    Frame fc = f; // copy before popping invalidates f
                    stk.pop_back();
                    Val nv;
                    if (fc.type == 'U') {
                        nv.a = fc.child[0].a + fc.child[1].a;
                        nv.o = std::max(fc.child[0].o, fc.child[1].o);
                    } else {
                        nv.a = fc.child[0].o;
                        nv.o = fc.child[0].a;
                    }
                    val = nv;
                    // loop again to try resolving further up
                } else {
                    break;
                }
            }
        }
        if (got) {
            printf("%d\n", finalVal.a);
        }
        // A line with no V/U/C tokens (blank) produces no output line.
    }
    return 0;
}
