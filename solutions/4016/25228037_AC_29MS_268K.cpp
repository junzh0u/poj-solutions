// POJ 4016 - Flat
// Model: gpt-6-astra
// Accumulate total and bedroom area, and twice the reduced area.
// Balconies contribute half their area to price, but all to total area.
// The statement is Problem F in the contest PDF linked from POJ 4011.
#include <iostream>
#include <string>
using namespace std;
int main() {
    int n, c;
    if (!(cin >> n >> c)) return 0;
    int total = 0, bedrooms = 0, twiceReduced = 0;
    for (int i = 0; i < n; ++i) {
        int area;
        string type;
        cin >> area >> type;
        total += area;
        if (type == "bedroom") bedrooms += area;
        twiceReduced += area * (type == "balcony" ? 1 : 2);
    }
    // At most 2000 * 100000: the doubled price fits signed 32 bits.
    int twicePrice = twiceReduced * c;
    cout << total << '\n' << bedrooms << '\n';
    cout << twicePrice / 2 << (twicePrice % 2 ? ".5\n" : ".0\n");
    return 0;
}
