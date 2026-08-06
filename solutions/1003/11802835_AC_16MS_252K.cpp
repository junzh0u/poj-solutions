#include <iostream>

using namespace std;

int main() {
    float c;
    while (true) {
        cin >> c;
        if (c == 0.0)
            return 0;
        float sum = 0;
        short int i = 1;
        while (1) {
            sum += 1.0 / (i + 1);
            if (sum >= c) {
                cout << i << " card(s)" << endl;
                break;
            }
            i++;
        }
    }
}
