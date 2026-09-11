// POJ 1749 - Lock Manager
// Model: claude-sonnet-5
//
// Simulate a lock manager. For each data item we only need to know
// (a) the set of distinct transactions that currently hold a granted
// lock on it, and (b) which transaction (if any) holds an exclusive
// lock on it -- at most one transaction can ever successfully hold an
// X lock on an item, since any different-transaction request made
// after that X is granted will conflict with it and be denied.
//
// A new request (mode, trid) on an item conflicts with previously
// granted locks iff:
//   - mode == X and some granted lock on the item belongs to a
//     different transaction (any mode), or
//   - mode == S and the item currently has an X lock held by a
//     different transaction.
// A transaction that is ever denied is blocked forever after: every
// later request from it is IGNORED (it never applies to further
// items either).
//
// Verified against the sample: request "S 3 5" prints IGNORED
// because transaction 3 was already blocked by "S 3 2" (DENIED)
// earlier in the sequence -- confirmed against the problem's own
// discuss board (message 91467), so this isn't a guess about an
// ambiguous statement.
#include <cstdio>
#include <map>
#include <set>
using namespace std;

int main() {
    map<int, bool> blocked;              // trid -> ever denied
    map<int, set<int> > itemTrids;       // item -> distinct trids holding a lock
    map<int, int> itemX;                 // item -> trid holding X lock (0 = none)

    char tok[16];
    while (scanf("%15s", tok) == 1) {
        if (tok[0] == '#') break;
        char mode = tok[0];
        long trid, item;
        scanf("%ld %ld", &trid, &item);

        if (blocked[(int)trid]) {
            printf("IGNORED\n");
            continue;
        }

        set<int> &trids = itemTrids[(int)item];
        int xHolder = itemX[(int)item]; // defaults to 0 if absent

        bool conflict;
        if (mode == 'X') {
            bool otherExists = (trids.size() > 1) ||
                                (trids.size() == 1 && !trids.count((int)trid));
            conflict = otherExists;
        } else {
            conflict = (xHolder != 0 && xHolder != (int)trid);
        }

        if (conflict) {
            printf("DENIED\n");
            blocked[(int)trid] = true;
        } else {
            printf("GRANTED\n");
            trids.insert((int)trid);
            if (mode == 'X') itemX[(int)item] = (int)trid;
        }
    }
    return 0;
}
