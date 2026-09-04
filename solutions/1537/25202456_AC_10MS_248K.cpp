// POJ 1537 - Identifying Legal Pascal Real Constants
// Model: claude-sonnet-5
// Approach: read whitespace-delimited tokens (the board's msg 359374 reports
// that a single input line can hold several candidates separated by blanks,
// e.g. "1e2 4", which must be classified as two separate constants rather
// than one candidate with an embedded blank -- so tokenizing on whitespace,
// not reading whole lines, is what the judge data actually requires; the
// statement's "no embedded blanks" rule is then automatically satisfied
// since a whitespace-delimited token can never contain one). For each token,
// check the Pascal unsigned/signed real-constant grammar directly:
//   [sign] digits ( '.' digits [ ('e'|'E') [sign] digits ] | ('e'|'E') [sign] digits )
// i.e. a mandatory leading digit sequence, then either a decimal point with
// digits on both sides (optionally followed by an exponent) or an exponent
// alone -- at least one of decimal point / exponent must be present. Leading
// zeros are fine (board hint: no need to reject them, "0.1E123" is legal).
#include <cstdio>
#include <cstring>
#include <cctype>

bool isLegal(const char *s) {
    int i = 0, n = (int)strlen(s);
    if (s[i] == '+' || s[i] == '-') i++;
    if (!(i < n && isdigit((unsigned char)s[i]))) return false;
    while (i < n && isdigit((unsigned char)s[i])) i++;
    bool hasDot = false, hasExp = false;
    if (i < n && s[i] == '.') {
        hasDot = true;
        i++;
        if (!(i < n && isdigit((unsigned char)s[i]))) return false;
        while (i < n && isdigit((unsigned char)s[i])) i++;
    }
    if (i < n && (s[i] == 'e' || s[i] == 'E')) {
        hasExp = true;
        i++;
        if (i < n && (s[i] == '+' || s[i] == '-')) i++;
        if (!(i < n && isdigit((unsigned char)s[i]))) return false;
        while (i < n && isdigit((unsigned char)s[i])) i++;
    }
    return i == n && (hasDot || hasExp);
}

int main() {
    static char buf[1000006];
    while (scanf("%1000000s", buf) == 1) {
        if (strcmp(buf, "*") == 0) break;
        if (isLegal(buf)) printf("%s is legal.\n", buf);
        else printf("%s is illegal.\n", buf);
    }
    return 0;
}
