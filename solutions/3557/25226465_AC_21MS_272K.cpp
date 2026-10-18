// POJ 3557 - Map Generator
// Model: gpt-6-astra
// Classify disconnected graphs by the component containing vertex 1.
// Choose its other vertices, require that component to be connected, and
// forbid all crossing edges. The remaining graph is unrestricted.
// The statement gives no case count; process pairs through EOF, as on the board.
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;
int main() {
    double choose[21][21] = {{0}};
    for (int i = 0; i <= 20; ++i) {
        choose[i][0] = choose[i][i] = 1;
        for (int j = 1; j < i; ++j)
            choose[i][j] = choose[i-1][j-1] + choose[i-1][j];
    }
    int n; double p;
    while (cin >> n >> p) {
        double connected[21] = {0};
        connected[1] = 1;
        for (int size = 2; size <= n; ++size) {
            double disconnected = 0;
            for (int k = 1; k < size; ++k)
                disconnected += choose[size-1][k-1] * connected[k]
                    * pow(1-p, k*(size-k));
            connected[size] = 1-disconnected;
            if (connected[size] < 0) connected[size] = 0;
            if (connected[size] > 1) connected[size] = 1;
        }
        cout << setprecision(12) << connected[n] << '\n';
    }
    return 0;
}
