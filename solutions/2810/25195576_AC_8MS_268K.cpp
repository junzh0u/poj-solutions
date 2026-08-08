// POJ 2810 - Take Your Vitamins
// Model: claude-opus-5
//
// Pure input-framing / output-formatting problem. Each line is "A U R V" with
// exactly one space between fields; V runs to end of line and may contain
// spaces. Read to a line with A < 0 (A == 0 is a real record -- the sample's
// Calcium row). Emit every nutrient whose percentage is at least 1%, then the
// header line, then the sub-1% names, both in input order.
//
// Statement ambiguities and how they were settled:
//  * "If it is at least 1%" -- the test is on the raw percentage, not on the
//    rounded one. The sample settles it: Fiber is 0.15/25 = 0.6%, which
//    printf("%.0f") renders as "1", yet Fiber is listed as insignificant.
//  * "P with zero digits precision" -- printf rounding, not truncation. The
//    sample cannot separate the two (70, 100, 125 are exact and 11.01 truncates
//    and rounds alike); the discuss board answers "round, do not truncate", and
//    "precision" is printf's own word.
//  * V carries a trailing space in the data and it must not be echoed --
//    the board's most-repeated pitfall. Trailing blanks (and CR) are stripped.
//  * The "Provides no significant amount of:" header is printed unconditionally,
//    including when no nutrient falls below 1%; two board posters report AC
//    doing exactly that.
//  * The 1% comparison carries a 1e-9 slack so a value that is mathematically
//    exactly 1% is not pushed below the line by float noise (e.g. 0.29/29*100
//    evaluates to 0.99999999999999989 in SSE doubles but to 1.0 under x87).

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

using namespace std;

static bool isSpace(char c) { return c == ' ' || c == '\t' || c == '\r' || c == '\n'; }

// Parses "A U R V". False if the line does not even hold A, U and R.
static bool parseLine(const string &s, double &A, string &U, double &R, string &V) {
    const char *p = s.c_str();
    while (*p && isSpace(*p)) ++p;
    char *e;
    A = strtod(p, &e);
    if (e == p) return false;
    const char *q = e;
    while (*q && isSpace(*q)) ++q;
    const char *us = q;
    while (*q && !isSpace(*q)) ++q;
    U.assign(us, q - us);
    while (*q && isSpace(*q)) ++q;
    R = strtod(q, &e);
    if (e == q) return false;
    const char *v = e;
    while (*v && isSpace(*v)) ++v;
    V.assign(v);
    while (!V.empty() && isSpace(V[V.size() - 1])) V.erase(V.size() - 1);
    return true;
}

struct Row {
    string V, U;
    double A, pct;
};

int main() {
    // Slurp the input; the dataset structure is then a plain scan, and no
    // assumption about line length or record count is baked in.
    vector<string> lines;
    {
        const int BUF = 4096;
        char buf[BUF];
        string cur;
        while (fgets(buf, BUF, stdin)) {
            cur += buf;
            if (!cur.empty() && cur[cur.size() - 1] == '\n') {
                lines.push_back(cur);
                cur.clear();
            }
        }
        if (!cur.empty()) lines.push_back(cur);
    }

    size_t i = 0;
    while (i < lines.size()) {
        // Skip blank filler before a dataset; stop if only blanks remain.
        bool any = false;
        while (i < lines.size()) {
            const char *p = lines[i].c_str();
            while (*p && isSpace(*p)) ++p;
            if (*p) { any = true; break; }
            ++i;
        }
        if (!any) break;

        vector<Row> good;
        vector<string> bad;
        for (; i < lines.size(); ++i) {
            double A, R;
            string U, V;
            if (!parseLine(lines[i], A, U, R, V)) continue;
            if (A < 0.0) { ++i; break; }

            double pct;
            if (R > 0.0) pct = A / R * 100.0;
            else pct = (A > 0.0) ? 1e18 : 0.0;

            if (pct >= 1.0 - 1e-9) {
                Row r;
                r.V = V; r.U = U; r.A = A; r.pct = pct;
                good.push_back(r);
            } else {
                bad.push_back(V);
            }
        }

        for (size_t k = 0; k < good.size(); ++k)
            printf("%s %.1f %s %.0f%%\n", good[k].V.c_str(), good[k].A,
                   good[k].U.c_str(), good[k].pct);
        printf("Provides no significant amount of:\n");
        for (size_t k = 0; k < bad.size(); ++k)
            printf("%s\n", bad[k].c_str());
    }
    return 0;
}
