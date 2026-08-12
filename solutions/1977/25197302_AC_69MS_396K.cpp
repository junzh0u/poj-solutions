// POJ 1977 - Odd Loving Bakers
// Model: gpt-5.6-terra
// Each celebration is a linear transformation over GF(2): next winners are
// (I + A) times the current winners, where A[i][j] records j liking i.
// Binary exponentiation obtains the winners of celebration t.
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

typedef vector< vector<int> > Matrix;

Matrix multiply(const Matrix& a, const Matrix& b) {
    int n = (int)a.size();
    Matrix c(n, vector<int>(n, 0));
    int i, j, k;
    for (i = 0; i < n; ++i)
        for (k = 0; k < n; ++k)
            if (a[i][k])
                for (j = 0; j < n; ++j)
                    c[i][j] ^= b[k][j];
    return c;
}

vector<int> multiplyVector(const Matrix& a, const vector<int>& v) {
    int n = (int)a.size();
    vector<int> r(n, 0);
    int i, j;
    for (i = 0; i < n; ++i)
        for (j = 0; j < n; ++j)
            if (a[i][j] && v[j]) r[i] ^= 1;
    return r;
}

int main() {
    ios::sync_with_stdio(false);
    int cases;
    cin >> cases;
    while (cases--) {
        int n;
        long t;
        cin >> n >> t;
        vector<string> name(n);
        vector<int> initial(n), count(n);
        vector< vector<string> > favorite(n);
        map<string, int> id;
        int i, j;
        for (i = 0; i < n; ++i) {
            cin >> name[i] >> initial[i] >> count[i];
            id[name[i]] = i;
            favorite[i].resize(count[i]);
            for (j = 0; j < count[i]; ++j) cin >> favorite[i][j];
        }
        Matrix power(n, vector<int>(n, 0));
        for (i = 0; i < n; ++i) power[i][i] = 1;
        for (j = 0; j < n; ++j)
            for (i = 0; i < count[j]; ++i)
                power[id[favorite[j][i]]][j] ^= 1;
        vector<int> winners(n);
        for (i = 0; i < n; ++i) winners[i] = initial[i] & 1;
        --t;
        while (t) {
            if (t & 1) winners = multiplyVector(power, winners);
            power = multiply(power, power);
            t >>= 1;
        }
        int answer = 0;
        for (i = 0; i < n; ++i) answer += winners[i];
        cout << answer << '\n';
    }
    return 0;
}
