// POJ 3886 - Baby's Blocks
// Model: gpt-6.1-sol
// Count distinct lexicographically earlier permutations by their first differing
// letter. Repeated blocks are indistinguishable; the first word starts at zero.
// 20! fits signed 64-bit arithmetic, so use G++ and long long.
#include <iostream>
#include <string>
using namespace std;
long long fact[21];
long long ways(int n, const int count[]) {
    long long result = fact[n];
    for (int i = 0; i < 26; ++i) result /= fact[count[i]];
    return result;
}
int main() {
    fact[0] = 1;
    for (int i = 1; i <= 20; ++i) fact[i] = fact[i-1] * i;
    int tests;
    cin >> tests;
    while (tests--) {
        string s;
        cin >> s;
        int count[26] = {0};
        for (int i = 0; i < (int)s.size(); ++i) ++count[s[i]-'A'];
        long long answer = 0;
        for (int i = 0; i < (int)s.size(); ++i) {
            int letter = s[i]-'A';
            for (int c = 0; c < letter; ++c) if (count[c]) {
                --count[c];
                answer += ways((int)s.size()-i-1, count);
                ++count[c];
            }
            --count[letter];
        }
        cout << answer << '\n';
    }
    return 0;
}
