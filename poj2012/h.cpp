#include <iostream>
#include <algorithm>

using namespace std;

#define N 10
int smark[N][101], sum[101], a[3][101];
bool f[101][N][510];

int t[10][10] = {
    {1, 1, 2, 3, 4, 0, 6, 0, 8, 0},
    {0, 1, 0, 3, 0, 0, 0, 0, 8, 0},
    {0, 0, 2, 3, 0, 0, 6, 0, 0, 0},
    {0, 1, 0, 3, 0, 0, 0, 0, 0, 9},
    {0, 0, 0, 0, 4, 0, 6, 0, 8, 0},
    {0, 1, 0, 0, 4, 5, 0, 0, 0, 0},
    {0, 0, 0, 0, 4, 0, 6, 0, 0, 9},
    {0, 1, 0, 3, 4, 5, 6, 7, 0, 9},
    {0, 1, 0, 0, 4, 0, 0, 0, 8, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 9}
};

int main() {
    int n;
    while(cin >> n) {
        for(int i = 0; i < 3; ++i) {
            for(int j = 1; j <= n; ++j) {
                cin >> a[i][j];
            }
        }
        for(int i = 1; i <= n; ++i) {
            sum[i] = sum[i - 1] + a[0][i] + a[1][i] + a[2][i];
            for(int k = 0; k < 8; ++k) {
                smark[k][i] = 0;
                for(int j = 0; j < 3; ++j) {
                    if(k & (1<<j)) {
                        smark[k][i] += a[j][i];
                    }
                }
            }
            smark[8][i] = smark[5][i];
            smark[9][i] = smark[2][i];
        }
        memset(f, false, sizeof(f));
        f[0][0][0] = true;
        f[0][7][0] = true;
        for(int i = 0; i < n; ++i) {
            for(int m1 = 0; m1 < N; ++m1) {
                for(int s = 0; s <= sum[n] / 2; ++s) {
                    if(f[i][m1][s]) {
                        for(int m2 = 0; m2 < N; ++m2) {
                            if(t[m1][m2]) {
                                f[i + 1][m2][s + smark[i + 1][m2]] = true;
                            }
                        }
                    }
                }
            }
        }
        int ans = sum[n];
        for(int i = 1; i <= n; ++i) {
            for(int j = 0; j < N; ++j) {
                for(int s = 0; s <= sum[n] / 2; ++s) {
                    if(f[i][j][s]) {
                        if(j != 8) ans = min(ans, abs(sum[n] - s * 2));
                        if(j != 9) ans = min(ans, abs(sum[n] - (s + sum[n] - sum[i]) * 2));
                    }
                }
            }
        }
        
        cout << ans << endl;
    }
    return 0;
}

