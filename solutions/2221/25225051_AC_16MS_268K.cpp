// POJ 2221 - Frogger's For Dinner
// Model: gpt-6.1-sol
// Mark every column swept by a car during the turn the frog enters its lane.
// Both endpoints and intermediate squares count as collisions, including wraps.
#include <iostream>
#include <string>
using namespace std;
int main() {
    string word;
    while (cin >> word) {
        bool blocked[10] = {false};
        for (int row = 1; row <= 8; ++row) {
            int direction = row <= 4 ? -1 : 1;
            for (int col = 0; col < 10; ++col) {
                int speed;
                cin >> speed;
                if (speed == 0) continue;
                for (int step = 0; step <= speed; ++step) {
                    int position = col + direction * (speed * (row - 1) + step);
                    position = (position % 10 + 10) % 10;
                    blocked[position] = true;
                }
            }
        }
        cin >> word;
        bool possible = false;
        for (int col = 0; col < 10; ++col)
            if (!blocked[col]) possible = true;
        cout << (possible ? "LEFTOVER POSSUM" : "FROGGER") << '\n';
    }
    return 0;
}
