// POJ 1886 - Borrowers
// Model: claude-sonnet-5
// Approach: Read the fixed stock of books, sort once by (author, title) using
// ASCII ordering, and assign each book a rank in that sorted order. "On shelf"
// membership is tracked with a std::set<int> of ranks, which gives O(log n)
// access to the nearest smaller/larger rank still on the shelf. BORROW removes
// a rank from the shelf set (and, defensively, from the pending-return set,
// in case a book is borrowed again straight off the front desk before being
// reshelved). RETURN adds the rank to a pending set. SHELVE walks the pending
// set in ascending rank order (its natural iteration order), and for each
// book finds the predecessor already on the shelf via lower_bound, emitting
// "first" when there is none; the book is inserted into the shelf set
// immediately so later books in the same SHELVE can reference it. The
// pending set is cleared and an END line printed even when no book was
// returned since the last SHELVE (confirmed by the discuss board's worked
// trace in message 95807, which also confirms returned-but-not-yet-shelved
// books must not linger across SHELVE calls -- matched here since the
// pending set is fully drained on every SHELVE). No statement ambiguity was
// found; the board mainly warned about the empty-SHELVE-still-prints-END case
// and the "first" special case, both handled directly by this design.
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>
using namespace std;

struct Book {
    string title;
    string author;
};

bool cmpBook(const Book &a, const Book &b) {
    if (a.author != b.author) return a.author < b.author;
    return a.title < b.title;
}

static string trimCR(string s) {
    while (!s.empty() && (s[s.size()-1] == '\r' || s[s.size()-1] == '\n'))
        s.erase(s.size()-1);
    return s;
}

int main() {
    ios::sync_with_stdio(false);
    vector<Book> books;
    string line;

    while (getline(cin, line)) {
        line = trimCR(line);
        if (line == "END") break;
        size_t p1 = line.find('"');
        size_t p2 = line.find('"', p1 + 1);
        Book b;
        b.title = line.substr(p1 + 1, p2 - p1 - 1);
        b.author = line.substr(p2 + 5); // skip closing quote + " by "
        books.push_back(b);
    }

    int n = (int)books.size();
    sort(books.begin(), books.end(), cmpBook);

    map<string, int> rankOf;
    vector<string> titleOf(n);
    for (int i = 0; i < n; ++i) {
        titleOf[i] = books[i].title;
        rankOf[books[i].title] = i;
    }

    set<int> onShelf;
    for (int i = 0; i < n; ++i) onShelf.insert(i);
    set<int> pending;

    while (getline(cin, line)) {
        line = trimCR(line);
        if (line == "END") break;
        if (line.empty()) continue;
        if (line == "SHELVE") {
            for (set<int>::iterator it = pending.begin(); it != pending.end(); ++it) {
                int id = *it;
                set<int>::iterator sit = onShelf.lower_bound(id);
                if (sit == onShelf.begin()) {
                    printf("Put \"%s\" first\n", titleOf[id].c_str());
                } else {
                    set<int>::iterator prev = sit;
                    --prev;
                    printf("Put \"%s\" after \"%s\"\n", titleOf[id].c_str(), titleOf[*prev].c_str());
                }
                onShelf.insert(id);
            }
            pending.clear();
            printf("END\n");
        } else {
            bool isBorrow = (line[0] == 'B');
            size_t p1 = line.find('"');
            size_t p2 = line.find('"', p1 + 1);
            string title = line.substr(p1 + 1, p2 - p1 - 1);
            map<string, int>::iterator mit = rankOf.find(title);
            if (mit == rankOf.end()) continue; // defensive; shouldn't happen
            int id = mit->second;
            if (isBorrow) {
                onShelf.erase(id);
                pending.erase(id);
            } else {
                pending.insert(id);
            }
        }
    }

    return 0;
}
