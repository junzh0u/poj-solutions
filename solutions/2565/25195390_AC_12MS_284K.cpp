// POJ 2565 - Average is not Fast Enough!
// Model: claude-opus-5
//
// Sum each team's section times in whole seconds, divide by the relay distance
// d, round to the nearest second and print as m:ss (or "-" if any runner was
// disqualified).
//
// Exactness: d is read as a decimal string and kept as the exact rational
// DN / 10^k, so the average is T * 10^k / DN with T the integer second total.
// Rounding half up is then the integer expression (2*T*10^k + DN) / (2*DN),
// which removes floating point from the one place a tie could flip a digit.
// (The sample's own last team lands on 359.6 -> 360 -> "6:00", i.e. the carry
// out of the seconds field into the minutes field must happen after rounding.)
//
// Input framing: the discuss board's accepted sources re-read a fresh "n d"
// header whenever a line carries no ':' character, so the input may hold more
// than the single data set the statement describes; this reads to EOF and does
// the same. Every team line contains ':' (a disqualified runner is "-:--:--"),
// so the test is unambiguous. Section times are taken as all the tokens after
// the team number rather than exactly n of them.

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

int main() {
    std::ios::sync_with_stdio(false);
    std::string line;
    long long dnum = 1;   // d == dnum / dden, exactly
    long long dden = 1;

    while (std::getline(std::cin, line)) {
        while (!line.empty() &&
               (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
            line.erase(line.size() - 1);

        bool blank = true;
        for (std::string::size_type i = 0; i < line.size(); ++i)
            if (!isspace(static_cast<unsigned char>(line[i]))) blank = false;
        if (blank) continue;

        if (line.find(':') == std::string::npos) {
            // header line: "n d"
            std::istringstream hs(line);
            std::string ntok, dtok;
            if (!(hs >> ntok >> dtok)) continue;
            dnum = 0;
            dden = 1;
            bool frac = false;
            for (std::string::size_type i = 0; i < dtok.size(); ++i) {
                char c = dtok[i];
                if (c == '.') {
                    frac = true;
                } else if (c >= '0' && c <= '9') {
                    dnum = dnum * 10 + (c - '0');
                    if (frac) dden *= 10;
                }
            }
            if (dnum <= 0) { dnum = 1; dden = 1; }
            continue;
        }

        std::istringstream ts(line);
        int team = 0;
        ts >> team;
        std::string tok;
        long long total = 0;
        bool dq = false;
        while (ts >> tok) {
            if (tok.find('-') != std::string::npos) { dq = true; continue; }
            long long h = 0, m = 0, s = 0;
            std::string::size_type p = 0;
            long long *fields[3];
            fields[0] = &h; fields[1] = &m; fields[2] = &s;
            int fi = 0;
            for (; p < tok.size() && fi < 3; ++p) {
                if (tok[p] == ':') { ++fi; continue; }
                if (tok[p] >= '0' && tok[p] <= '9')
                    *fields[fi] = *fields[fi] * 10 + (tok[p] - '0');
            }
            total += h * 3600 + m * 60 + s;
        }

        std::cout.width(3);
        std::cout << team << ": ";
        if (dq) {
            std::cout << "-\n";
        } else {
            // round(total * dden / dnum) with halves going up
            long long sec = (2 * total * dden + dnum) / (2 * dnum);
            long long mm = sec / 60;
            long long ss = sec % 60;
            std::cout << mm << ":";
            if (ss < 10) std::cout << "0";
            std::cout << ss << " min/km\n";
        }
    }
    return 0;
}
