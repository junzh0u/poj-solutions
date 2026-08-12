// POJ 2015 - Permutation Code
// Model: gpt-5.6-terra
// Recover the exceptional character first, then walk backward around the
// cyclic dependency.  Each ciphertext character supplies a P-position xor
// the next plaintext character’s S-position.
#include <cmath>
#include <iostream>
#include <string>
using namespace std;

int main() {
    int x;
    while (cin >> x && x) {
        string s, p, c;
        cin >> s >> p >> c;
        int posS[256], posP[256];
        int i;
        for (i = 0; i < 256; ++i) posS[i] = posP[i] = -1;
        for (i = 0; i < (int)s.size(); ++i) {
            posS[(unsigned char)s[i]] = i;
            posP[(unsigned char)p[i]] = i;
        }
        int n = (int)c.size();
        int d = ((int)(n * sqrt((double)n)) + x) % n;
        string m(n, ' ');
        m[d] = p[posS[(unsigned char)c[d]]];
        for (i = 1; i < n; ++i) {
            int j = (d - i + n) % n;
            int next = (j + 1) % n;
            m[j] = p[posS[(unsigned char)c[j]] ^ posS[(unsigned char)m[next]]];
        }
        cout << m << '\n';
    }
    return 0;
}
