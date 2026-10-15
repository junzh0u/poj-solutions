// POJ 1497 - Molecules
// Model: gpt-6.1-sol
// Enumerate chain assignments and rectangle dimensions. Internal ordered
// character pairs record all possible intersections on each vertical chain.
// Distances are boundary-to-boundary; vacant area is (width-1)*(height-1).
// Both intersections exclude the first and last elements, preserving tails.
#include <iostream>
#include <string>
#include <algorithm>
#include <cstring>
using namespace std;
int main() {
    string s[4];
    while (cin >> s[0] && s[0][0] != 'Q') {
        cin >> s[1] >> s[2] >> s[3];
        int n = s[0].size();
        bool has[4][12][16][16];
        memset(has, 0, sizeof(has));
        for (int c=0;c<4;++c)
            for (int i=1;i<n-1;++i)
                for (int j=i+2;j<n-1;++j)
                    has[c][j-i][s[c][i]-'A'][s[c][j]-'A']=true;
        int p[4]={0,1,2,3}, best=0;
        do {
            for (int w=n-3;w>=2;--w)
                for (int h=n-3;h>=2;--h) {
                    int area=(w-1)*(h-1);
                    if (area<=best) continue;
                    for (int i=1;i+w<n-1;++i)
                        for (int j=1;j+w<n-1;++j)
                            if (has[p[2]][h][s[p[0]][i]-'A'][s[p[1]][j]-'A'] &&
                                has[p[3]][h][s[p[0]][i+w]-'A'][s[p[1]][j+w]-'A'])
                                best=area;
                }
        } while (next_permutation(p,p+4));
        cout << best << '\n';
    }
}
