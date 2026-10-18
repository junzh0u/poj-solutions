// POJ 2558 - Genetic Code
// Model: gpt-6-astra
// Generate the Hall-Thue square-free word by iterating
// N -> NOP, O -> NP, P -> O, starting with N. Every prefix is
// square-free, so one word supplies every requested length up to 5000.
// The special judge permits any valid word; sample spelling is not unique.
#include <iostream>
#include <string>
using namespace std;

int main() {
    string word = "N";
    while (word.size() < 5000) {
        string next;
        for (size_t i = 0; i < word.size(); ++i) {
            if (word[i] == 'N') next += "NOP";
            else if (word[i] == 'O') next += "NP";
            else next += "O";
        }
        word.swap(next);
    }
    int n;
    while (cin >> n && n != 0)
        cout << word.substr(0, n) << '\n';
    return 0;
}
