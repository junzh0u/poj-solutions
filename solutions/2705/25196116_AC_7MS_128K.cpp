// POJ 2705 - Overflowing Bookshelf
// Model: claude-sonnet-5
//
// Books are added at the left end of the shelf, pushing any touching
// (overlapping) books to the right in a chain; anything not entirely on
// the shelf after the push falls off and is removed. Because holes left
// by earlier removals only ever absorb pushes (they never cause a book
// that would otherwise fit to be removed early -- provable: a book that
// isn't touched by a push keeps its already-valid position, and a book
// that is touched ends up at exactly (sum of widths pushed so far), the
// same value whether or not there was slack behind it), the surviving
// set and left-to-right order are exactly reproduced by a simple deque:
// keep a running total width, push the new book on the front, and pop
// books off the back (oldest surviving = rightmost) while the total
// exceeds the shelf width. No positions need to be tracked at all.
#include <cstdio>
#include <deque>
using namespace std;

struct Book {
    int id;
    int width;
};

int main() {
    int s;
    int caseNo = 0;
    while (scanf("%d", &s) == 1) {
        if (s == -1) break;
        caseNo++;
        deque<Book> shelf; // front = leftmost, back = rightmost
        int total = 0;
        char cmd[8];
        while (scanf("%s", cmd) == 1) {
            if (cmd[0] == 'E') {
                break;
            } else if (cmd[0] == 'A') {
                int id, w;
                scanf("%d %d", &id, &w);
                total += w;
                while (total > s) {
                    total -= shelf.back().width;
                    shelf.pop_back();
                }
                Book b;
                b.id = id;
                b.width = w;
                shelf.push_front(b);
            } else if (cmd[0] == 'R') {
                int id;
                scanf("%d", &id);
                for (size_t i = 0; i < shelf.size(); ++i) {
                    if (shelf[i].id == id) {
                        total -= shelf[i].width;
                        shelf.erase(shelf.begin() + i);
                        break;
                    }
                }
            }
        }
        printf("PROBLEM %d:", caseNo);
        for (size_t i = 0; i < shelf.size(); ++i) {
            printf(" %d", shelf[i].id);
        }
        printf("\n");
    }
    return 0;
}
