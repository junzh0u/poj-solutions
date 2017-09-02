#include<cstdio>
#include <cmath>
#include<iostream>

using namespace std;

int ans[10][10]={
    {0},
{3},
{3,7},
{3,7,13},
{3,7,13,37},
{3,7,13,19,61},
{3,7,13,19,31,43},
{3,7,13,19,37,163,181},
{3,7,13,19,37,97,139,163},
{3,7,13,19,31,43,61,97,151}
};

int main() {
    int T, n;
    cin >> T;
    while(T--) {
        cin >> n;
        long long sum = 1;
        for(int i = 0; i < n; ++i) {
            sum *= ans[n][i];
        }
        for(int i = 0; i < n; ++i) {
            cout << ans[n][i] << endl;
        }
        long long a = ((long long)sqrt(4*sum - 3) - 1) / 2;
        cout << a << endl;
    }
    return 0;
}
