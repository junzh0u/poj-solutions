// POJ 2129 - Invariant Polynomials
// Model: gpt-6.1-sol
// Use z=x+iy and w=x-iy: rotation multiplies z^i w^j by
// exp(2*pi*i*(i-j)/n). Count the fixed basis monomials.
// Complexification preserves the dimension of the real fixed space.
// Degree is total degree i+j, including the constant polynomial.
#include <iostream>
using namespace std;
int main() {
    int d, n;
    if (!(cin >> d >> n)) return 0;
    int answer = 0;
    for (int i = 0; i <= d; ++i)
        for (int j = 0; i + j <= d; ++j)
            if ((i - j) % n == 0) ++answer;
    cout << answer << '\n';
    return 0;
}
