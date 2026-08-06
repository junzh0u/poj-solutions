// POJ 3487 - The Stable Marriage Problem
//
// n males and n females (n < 27), each with a full preference list over the
// opposite set; print the male-optimal stable matching.  That is exactly what
// the Gale-Shapley algorithm produces when the MALES propose, so:
//
//   keep a stack of free males; the man on top proposes to the next female on
//   his own list that he has not yet asked.  If she is free they engage; if she
//   is taken she consults HER list and keeps whichever of the two men she ranks
//   higher, pushing the loser back on the free stack.  A male never revisits a
//   female, so the total number of proposals is at most n^2 and the run is
//   O(n^2) per test case.
//
// Direction matters: this is the one point where the statement pins the answer
// down.  "Male-optimal" means the proposers are the males; had the females
// proposed, sample 2 would come out `a A / b B / c C` instead of `a B / b A`.
// A rejected proposal only advances that male's pointer -- the female's own
// engagement is untouched -- while an accepted one may free the incumbent.
//
// Statement details worth pinning down:
//   * Names are arbitrary letters, not necessarily `a..z` / `A..Z` prefixes,
//     and are read off the names line, so everything is indexed through a
//     char -> index map rather than assumed contiguous.
//   * Output is ordered by male name lexicographically (the names are sorted,
//     which makes the output loop a straight scan), one `m F` pair per line.
//   * "Output an empty line between test cases" is taken literally: the blank
//     line is printed before every case but the first, none after the last.
//   * Preference entries are parsed as `<name>:<list>` but tolerate whitespace
//     anywhere inside them -- tokens are pulled in until the list holds n names.
//
#include <cstdio>
#include <cstring>
#include <string>
#include <iostream>
#include <algorithm>
using namespace std;

int main() {
    int T;
    if (!(cin >> T)) return 0;
    bool first = true;
    while (T-- > 0) {
        int n;
        cin >> n;
        char mname[30], fname[30];
        int nm = 0, nf = 0;
        for (int i = 0; i < 2 * n; i++) {
            string tok;
            cin >> tok;
            char c = tok[0];
            if (c >= 'a' && c <= 'z') mname[nm++] = c; else fname[nf++] = c;
        }
        int mIdx[128], fIdx[128];
        for (int i = 0; i < 128; i++) { mIdx[i] = -1; fIdx[i] = -1; }
        sort(mname, mname + nm);
        sort(fname, fname + nf);
        for (int i = 0; i < nm; i++) mIdx[(int)(unsigned char)mname[i]] = i;
        for (int i = 0; i < nf; i++) fIdx[(int)(unsigned char)fname[i]] = i;

        static int pref[30][30];   /* pref[m][k] = k-th choice female of male m */
        static int frank[30][30];  /* frank[f][m] = rank of male m in female f's list */
        for (int i = 0; i < 2 * n; i++) {
            /* An entry is "<name>:<list>"; tolerate whitespace anywhere inside it
               by pulling in further tokens until the list holds n names. */
            string tok, t;
            cin >> tok;
            while (tok.find(':') == string::npos && (cin >> t)) tok += t;
            size_t p = tok.find(':');
            while ((int)(tok.size() - p - 1) < n && (cin >> t)) tok += t;
            char who = tok[0];
            string lst = tok.substr(p + 1);
            if (who >= 'a' && who <= 'z') {
                int m = mIdx[(int)(unsigned char)who];
                for (int k = 0; k < (int)lst.size() && k < n; k++)
                    pref[m][k] = fIdx[(int)(unsigned char)lst[k]];
            } else {
                int f = fIdx[(int)(unsigned char)who];
                for (int k = 0; k < (int)lst.size() && k < n; k++)
                    frank[f][ mIdx[(int)(unsigned char)lst[k]] ] = k;
            }
        }

        int next_[30], husband[30], wife[30];
        int stk[30], sp = 0;
        for (int i = 0; i < n; i++) { next_[i] = 0; wife[i] = -1; husband[i] = -1; }
        for (int i = n - 1; i >= 0; i--) stk[sp++] = i;

        while (sp > 0) {
            int m = stk[--sp];
            while (next_[m] < n) {
                int f = pref[m][next_[m]++];
                if (husband[f] == -1) { husband[f] = m; wife[m] = f; break; }
                int cur = husband[f];
                if (frank[f][m] < frank[f][cur]) {
                    husband[f] = m; wife[m] = f;
                    wife[cur] = -1; stk[sp++] = cur;
                    break;
                }
            }
        }

        if (!first) printf("\n");
        first = false;
        for (int i = 0; i < n; i++)
            printf("%c %c\n", mname[i], fname[wife[i]]);
    }
    return 0;
}
