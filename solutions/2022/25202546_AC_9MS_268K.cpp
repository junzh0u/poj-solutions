// POJ 2022 - Syntax Included
// Model: claude-sonnet-5
// Recursive-descent parser over the literal grammar in the statement.
// TEXT -> STRING | STRING TEXT | TAG | TAG TEXT means TEXT is a sequence of
// (possibly-empty) plain-character runs and TAGs, terminated by the specific
// closing tag the caller expects. A stray '>' that isn't part of a tag, or a
// '<' that doesn't start a recognized tag / the expected closer, is a syntax
// error. Tag names and HREF are case sensitive per the statement; "http://"
// and ".com" are matched literally as given in the URL production. The
// sample admits no ambiguity between readings, so no separating case was
// needed; verified with a random valid-string generator (grammar-driven)
// plus targeted mutations of each production.
#include <cstdio>
#include <string>
using namespace std;

static string s;
static int n;

static bool startsWith(int pos, const string &lit) {
    if (pos + (int)lit.size() > n) return false;
    return s.compare(pos, lit.size(), lit) == 0;
}
static bool consumeLit(int &pos, const string &lit) {
    if (!startsWith(pos, lit)) return false;
    pos += (int)lit.size();
    return true;
}

static bool parseURL(int &pos) {
    int gt = -1;
    for (int i = pos; i < n; ++i) {
        if (s[i] == '>') { gt = i; break; }
        if (s[i] == '<') return false;
    }
    if (gt == -1) return false;
    string sub = s.substr(pos, gt - pos);
    const string pre = "http://", suf = ".com";
    if (sub.size() < pre.size() + suf.size()) return false;
    if (sub.compare(0, pre.size(), pre) != 0) return false;
    if (sub.compare(sub.size() - suf.size(), suf.size(), suf) != 0) return false;
    pos = gt;
    return true;
}

static bool parseText(int &pos, const string &closeTag);

static bool parseBold(int &pos) {
    if (!consumeLit(pos, "<B>")) return false;
    if (!parseText(pos, "</B>")) return false;
    return consumeLit(pos, "</B>");
}
static bool parseItalic(int &pos) {
    if (!consumeLit(pos, "<I>")) return false;
    if (!parseText(pos, "</I>")) return false;
    return consumeLit(pos, "</I>");
}
static bool parseLink(int &pos) {
    if (!consumeLit(pos, "<A HREF=")) return false;
    if (!parseURL(pos)) return false;
    if (!consumeLit(pos, ">")) return false;
    if (!parseText(pos, "</A>")) return false;
    return consumeLit(pos, "</A>");
}

static bool parseText(int &pos, const string &closeTag) {
    for (;;) {
        if (pos >= n) return false;
        if (s[pos] == '<') {
            if (startsWith(pos, closeTag)) return true;
            if (startsWith(pos, "<B>")) { if (!parseBold(pos)) return false; continue; }
            if (startsWith(pos, "<I>")) { if (!parseItalic(pos)) return false; continue; }
            if (startsWith(pos, "<A HREF=")) { if (!parseLink(pos)) return false; continue; }
            return false;
        } else {
            while (pos < n && s[pos] != '<') {
                if (s[pos] == '>') return false;
                pos++;
            }
        }
    }
}

static bool parseBody(int &pos) {
    if (!consumeLit(pos, "<BODY>")) return false;
    if (!parseText(pos, "</BODY>")) return false;
    return consumeLit(pos, "</BODY>");
}

static bool parseHTML() {
    int pos = 0;
    if (!consumeLit(pos, "<HTML>")) return false;
    if (!parseBody(pos)) return false;
    if (!consumeLit(pos, "</HTML>")) return false;
    return pos == n;
}

int main() {
    int t;
    if (scanf("%d", &t) != 1) return 0;
    char nl[8];
    fgets(nl, sizeof(nl), stdin);
    char buf[4096];
    for (int i = 0; i < t; ++i) {
        if (!fgets(buf, sizeof(buf), stdin)) buf[0] = '\0';
        s = buf;
        while (!s.empty() && (s[s.size() - 1] == '\n' || s[s.size() - 1] == '\r')) s.erase(s.size() - 1);
        n = (int)s.size();
        printf(parseHTML() ? "Syntax Included\n" : "No Syntax Included\n");
    }
    return 0;
}
