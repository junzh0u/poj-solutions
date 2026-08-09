// POJ 1809 - Regetni
// Model: gpt-5.6-terra
// Count all triples, then remove the triples whose three point parities are
// distinct.  Over F_2 those, and only those, have odd doubled area.
#include <iostream>

using namespace std;

long long choose3(long long n) {
    return n * (n - 1) * (n - 2) / 6;
}

int main() {
    ios::sync_with_stdio(false);
    int scenarios;
    if (!(cin >> scenarios)) return 0;
    for (int scenario = 1; scenario <= scenarios; ++scenario) {
        int n;
        cin >> n;
        long long count[4] = {0, 0, 0, 0};
        for (int i = 0; i < n; ++i) {
            long long x, y;
            cin >> x >> y;
            int px = static_cast<int>((x % 2 + 2) % 2);
            int py = static_cast<int>((y % 2 + 2) % 2);
            int type = px * 2 + py;
            ++count[type];
        }
        long long nonIntegral = 0;
        for (int missing = 0; missing < 4; ++missing) {
            long long product = 1;
            for (int type = 0; type < 4; ++type)
                if (type != missing) product *= count[type];
            nonIntegral += product;
        }
        cout << "Scenario #" << scenario << ":\n";
        cout << choose3(n) - nonIntegral << "\n\n";
    }
    return 0;
}
