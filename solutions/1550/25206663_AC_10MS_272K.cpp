// POJ 1550 - Treadmill
// Model: claude-sonnet-5
//
// Approach: The audio stream is a sequence of integer samples (0-9), with
// value 10 marking the end of a program. A chirp is 16 samples = 8 encoded
// bits: a fixed 2-bit start marker "01", then 3 speed bits (LSB first), then
// 3 inclination bits (LSB first). A "0" bit is two samples both in {4,5}; a
// "1" bit is two samples (first in {0,1}, second in {8,9}). The statement
// guarantees the start marker "01" never occurs in background music, so a
// simple left-to-right scan that tests every position for the 4-sample start
// pattern, and on a match decodes the following 12 samples and skips all 16,
// correctly finds every chirp with no backtracking or false positives.
//
// The chirp's time is the position of its first sample (0-indexed) divided
// by 3 (3 samples/second), floored to whole seconds as the statement asks.
//
// Verified against the worked example in the statement (the 16-sample chirp
// table decodes to speed 3 / inclination 6) and against both sample
// programs. The discuss board's posted source (message 2503) and the plain
// restatement in message 359569 confirm this reading of "01" as the fixed
// start bits and the 16-samples-per-chirp framing; no input-framing surprises
// were found (multiple programs, each terminated by a lone 10, read with
// plain cin>>).

#include <iostream>
#include <vector>
using namespace std;

static bool isZeroBit(int a, int b) {
    return (a == 4 || a == 5) && (b == 4 || b == 5);
}

static bool isOneBit(int a, int b) {
    return (a == 0 || a == 1) && (b == 8 || b == 9);
}

int main() {
    int n;
    if (!(cin >> n)) return 0;
    for (int prog = 1; prog <= n; prog++) {
        vector<int> a;
        int x;
        while (cin >> x) {
            if (x == 10) break;
            a.push_back(x);
        }
        cout << "Program " << prog << "\n";
        int sz = (int)a.size();
        int i = 0;
        while (i + 15 < sz) {
            if (isZeroBit(a[i], a[i + 1]) && isOneBit(a[i + 2], a[i + 3])) {
                int bits[6];
                for (int k = 0; k < 6; k++) {
                    int p = i + 4 + 2 * k;
                    if (isZeroBit(a[p], a[p + 1])) bits[k] = 0;
                    else bits[k] = 1;
                }
                int speed = bits[0] + bits[1] * 2 + bits[2] * 4;
                int incl = bits[3] + bits[4] * 2 + bits[5] * 4;
                int t = i / 3;
                int mm = t / 60;
                int ss = t % 60;
                cout.width(2); cout.fill('0'); cout << mm;
                cout << ":";
                cout.width(2); cout.fill('0'); cout << ss;
                cout << " Speed " << speed << " Inclination " << incl << "\n";
                i += 16;
            } else {
                i++;
            }
        }
    }
    return 0;
}
