// POJ 2257 - Balancing Bank Accounts
// Model: gpt-6-astra
// Accumulate money received minus money given, then repay net senders
// directly from net receivers. Each payment clears at least one balance;
// the last clears two, so at most n-1 nonzero payments are emitted.
// Transactions reverse the input transfers, not an equal expense share.
#include <iostream>
#include <string>
#include <map>
using namespace std;
int main() {
    int n, t, cs = 0;
    while (cin >> n >> t && (n || t)) {
        string name[20];
        int balance[20] = {0};
        map<string, int> index;
        for (int i = 0; i < n; ++i) {
            cin >> name[i];
            index[name[i]] = i;
        }
        for (int i = 0; i < t; ++i) {
            string a, b;
            int amount;
            cin >> a >> b >> amount;
            balance[index[a]] -= amount;
            balance[index[b]] += amount;
        }
        cout << "Case #" << ++cs << '\n';
        for (int i = 0; i < n; ++i) {
            if (balance[i] <= 0) continue;
            for (int j = 0; j < n && balance[i] > 0; ++j) {
                if (balance[j] >= 0) continue;
                int amount = balance[i] < -balance[j] ? balance[i] : -balance[j];
                cout << name[i] << ' ' << name[j] << ' ' << amount << '\n';
                balance[i] -= amount;
                balance[j] += amount;
            }
        }
        cout << '\n';
    }
    return 0;
}
