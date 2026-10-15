// POJ 1506 - Substitution Cipher
// Model: gpt-6.1-sol
// Adjacent dictionary words constrain the order of their first differing
// encrypted letters. Transitive closure gives every letter's predecessors
// and successors. Its plaintext rank is fixed iff it is comparable with
// all other letters; only letters occurring in the message must be fixed.
// Preserve spaces and symbols outside the substitution alphabet. Reject
// inconsistent orders and a longer word preceding its own prefix.
#include <iostream>
#include <string>
#include <cstring>
#include <limits>
using namespace std;
int main() {
    ios::sync_with_stdio(false);
    int cases;
    cin >> cases;
    while (cases--) {
        int a, k;
        cin >> a >> k;
        bool less[26][26];
        memset(less, 0, sizeof(less));
        bool ok = true;
        string prev, cur;
        for (int w = 0; w < k; ++w) {
            cin >> cur;
            if (w) {
                size_t p = 0;
                while (p < prev.size() && p < cur.size() && prev[p] == cur[p]) ++p;
                if (p < prev.size() && p < cur.size()) less[prev[p]-'a'][cur[p]-'a'] = true;
                else if (prev.size() > cur.size()) ok = false;
            }
            prev = cur;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        string message;
        getline(cin, message);
        if (!message.empty() && message[message.size()-1] == '\r') message.erase(message.size()-1);
        for (int z = 0; z < a; ++z)
            for (int x = 0; x < a; ++x)
                for (int y = 0; y < a; ++y)
                    if (less[x][z] && less[z][y]) less[x][y] = true;
        int rank[26];
        for (int x = 0; x < a; ++x) {
            if (less[x][x]) ok = false;
            int before = 0, after = 0;
            for (int y = 0; y < a; ++y) {
                if (less[y][x]) ++before;
                if (less[x][y]) ++after;
            }
            rank[x] = before + after == a-1 ? before : -1;
        }
        for (size_t i = 0; i < message.size(); ++i) {
            int x = message[i]-'a';
            if (x >= 0 && x < a) {
                if (rank[x] < 0) ok = false;
                else message[i] = char('a' + rank[x]);
            }
        }
        if (ok) cout << message << '\n';
        else cout << "Message cannot be decrypted.\n";
    }
    return 0;
}
