// POJ 2667 - Barber's problem
// Model: gpt-6.1-sol
// Arrival times repeat modulo the sum of the intervals. The three jobs
// performed by Tom must occupy disjoint translates of those residues.
// A shift is forbidden exactly when it is a difference of two arrivals.
// Enumerate the dry and pay shifts relative to cut. Any residue choice
// can preserve Wash and Change by adding whole periods to the waits.
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;
int main() {
    string line;
    while (getline(cin, line)) {
        vector<int> gaps;
        for (size_t i=0; i<line.size();) {
            if (isdigit(static_cast<unsigned char>(line[i]))) {
                int v=0;
                while (i<line.size() && isdigit(static_cast<unsigned char>(line[i])))
                    v=v*10+line[i++]-'0';
                gaps.push_back(v);
            } else ++i;
        }
        if (gaps.empty()) continue;
        if (gaps[0]==0) break;
        vector<int> a;
        int period=0;
        for (size_t i=0; i<gaps.size(); ++i) {
            a.push_back(period);
            period+=gaps[i];
        }
        vector<int> bad(period, 0);
        for (size_t i=0; i<a.size(); ++i)
            for (size_t j=0; j<a.size(); ++j)
                bad[(a[i]-a[j]+period)%period]=1;
        bool yes=false;
        for (int x=1; x<period && !yes; ++x) if (!bad[x])
            for (int y=x+1; y<period; ++y)
                if (!bad[y] && !bad[y-x]) { yes=true; break; }
        cout << (yes ? "Yes" : "No") << '\n';
    }
    return 0;
}
