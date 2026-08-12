// POJ 3359 - Wordfish
// Model: gpt-5.6-terra
// For each input word, inspect its ten lexicographic predecessors, itself,
// and its ten successors.  The answer maximizes the smallest ASCII distance
// between adjacent letters; lexical order resolves an equal score.
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int distance_of(const string &word) {
    int result = 1000;
    int i;
    for (i = 1; i < (int)word.size(); ++i) {
        int d = abs(word[i] - word[i - 1]);
        if (d < result) result = d;
    }
    return result;
}

int main() {
    string username;
    while (cin >> username) {
        vector<string> choices;
        string p = username;
        int i;
        for (i = 0; i < 10; ++i) {
            prev_permutation(p.begin(), p.end());
            choices.push_back(p);
        }
        choices.push_back(username);
        p = username;
        for (i = 0; i < 10; ++i) {
            next_permutation(p.begin(), p.end());
            choices.push_back(p);
        }

        string answer = choices[0];
        int best = distance_of(answer);
        for (i = 1; i < (int)choices.size(); ++i) {
            int score = distance_of(choices[i]);
            if (score > best || (score == best && choices[i] < answer)) {
                best = score;
                answer = choices[i];
            }
        }
        cout << answer << best << '\n';
    }
    return 0;
}
