// POJ 1509 - Glass Beads
// Model: gpt-5.6-terra
// Approach: Booth's two-candidate elimination algorithm finds the first
// lexicographically minimum rotation in linear time and O(n) storage.
// Equal minimum rotations are resolved by the smaller zero-based start index.
#include <iostream>
#include <string>

using namespace std;

int minimum_rotation(const string &necklace) {
    const int n = static_cast<int>(necklace.size());
    const string doubled = necklace + necklace;
    int first = 0;
    int second = 1;
    int matched = 0;

    while (first < n && second < n && matched < n) {
        const char a = doubled[first + matched];
        const char b = doubled[second + matched];
        if (a == b) {
            ++matched;
        } else {
            if (a > b) {
                first += matched + 1;
                if (first == second) {
                    ++first;
                }
            } else {
                second += matched + 1;
                if (first == second) {
                    ++second;
                }
            }
            matched = 0;
        }
    }
    return first < second ? first : second;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    int cases;
    cin >> cases;
    while (cases--) {
        string necklace;
        cin >> necklace;
        cout << minimum_rotation(necklace) + 1 << '\n';
    }
    return 0;
}
