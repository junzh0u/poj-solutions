// POJ 2494 - Acid Text
// Model: claude-sonnet-5
//
// Approach: overlay a set of named pictures onto a canvas according to a
// deliberately hostile CSS-like mini language. Each entry gives an absolute
// or relative (chained, acyclic) position, a source picture and a layer;
// entries are stacked in ascending layer order, ties broken by input order
// (stable sort), and non-'.' pixels overwrite the canvas while '.' pixels
// stay transparent. The canvas always starts at (row 0, col 0) -- even if
// nothing is drawn there -- and extends only as far as the highest drawn
// bounding box (transparent border pixels still count toward that box),
// per the discuss-board clarification of "top-left corner must contain
// (0,0)" / "as small as possible" (msgs 22496-22501, 36735-36741): the
// output is never cropped on the top/left, only trimmed on bottom/right,
// and the resulting picture is guaranteed to fit in 1000x1000 even though
// individual coordinates (and intermediate chained relative sums) can be
// as large as ~1e6 in absolute value.
//
// The real difficulty is the CSS tokenizer: whitespace (spaces/tabs, plus
// newlines) may be present or absent at every position where the sample
// grammar shows a space, including directly before '{' (board msg 345969,
// point 11) and around ':', 'px', '=', ';'. So the parser never trusts
// line structure: it pulls the next non-whitespace character on demand
// (skipping any run of space/tab/\r/\n) and matches the grammar's fixed
// literal keywords ("pos-x:", "px;pos-y:", "px;position:", "absolute;" /
// "relative=", "file:", "layer:") character by character, exactly as the
// grammar's own text is spelled (no letters ever get whitespace-split,
// only the specified space positions can vary in count, including zero).
// Integers are read the same way, stopping at the first non-digit without
// consuming it. Filenames/ids are read up to the next unescaped ';'.
// Everything is read from one flat in-memory buffer (all of stdin, via
// fread) with a manual cursor rather than through iostream/scanf per
// character -- a synthetic worst case (T scenarios each with 100 100x100
// pictures and 500 entries) showed plain cin>>string token reads alone
// costing over a second per ~20MB of input, which is not much slack
// against a 2000ms limit on a judge slower than this machine.
//
// Relative positions are resolved by memoized (explicit-stack, so no
// recursion-depth concern) resolution rather than by assuming definitions
// precede their uses in input order -- the statement only guarantees no
// cycles, not forward-reference-free input -- so this is correct
// regardless of ordering (the official sample happens to only use
// backward references, but nothing guarantees that in general).
//
// Ambiguity note: one old board post ends on an unresolved, unreplied
// remark about identifiers/filenames possibly needing case-insensitive
// comparison; no other message or the statement corroborates this, so
// matching is kept case-sensitive (as literally specified: "alphanumeric
// characters and dots only", nothing about case folding).

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// ---------- flat input buffer ----------
static char *buf;
static size_t bufLen;
static size_t pos;

static void loadInput() {
    size_t cap = 1 << 20;
    buf = (char *)malloc(cap);
    bufLen = 0;
    size_t got;
    while ((got = fread(buf + bufLen, 1, cap - bufLen, stdin)) > 0) {
        bufLen += got;
        if (bufLen == cap) {
            cap *= 2;
            buf = (char *)realloc(buf, cap);
        }
    }
    pos = 0;
}

static inline bool isWs(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

static inline int getNonWs() {
    while (pos < bufLen) {
        char c = buf[pos];
        if (isWs(c)) { pos++; continue; }
        pos++;
        return (unsigned char)c;
    }
    return -1;
}

// Consume exactly len non-whitespace characters (grammar guarantees they
// match the expected literal; we trust it rather than re-verify byte by
// byte, matching the proven-accepted approach this design is based on).
static inline void expectLiteral(const char *lit) {
    size_t len = strlen(lit);
    for (size_t i = 0; i < len; i++) getNonWs();
}

static string readUntil(char stopChar) {
    string s;
    for (;;) {
        int c = getNonWs();
        if (c < 0 || c == stopChar) break;
        s += (char)c;
    }
    return s;
}

static long readInt() {
    int c = getNonWs();
    bool neg = false;
    if (c == '-') { neg = true; c = (pos < bufLen) ? (unsigned char)buf[pos++] : -1; }
    long val = 0;
    while (c >= '0' && c <= '9') {
        val = val * 10 + (c - '0');
        c = (pos < bufLen) ? (unsigned char)buf[pos++] : -1;
    }
    if (c >= 0) pos--; // push back the first non-digit character
    return neg ? -val : val;
}

// Reads one whitespace-delimited token (skips leading whitespace first),
// used for the picture header/name/rows section, which is plain
// space/newline separated (not the punctuation-heavy CSS grammar).
static string readToken() {
    string s;
    int c = getNonWs();
    if (c < 0) return s;
    s += (char)c;
    while (pos < bufLen && !isWs(buf[pos])) {
        s += buf[pos];
        pos++;
    }
    return s;
}

static long readTokenInt() {
    int c = getNonWs();
    bool neg = false;
    if (c == '-') { neg = true; c = (pos < bufLen) ? (unsigned char)buf[pos++] : -1; }
    long val = 0;
    while (c >= '0' && c <= '9') {
        val = val * 10 + (c - '0');
        c = (pos < bufLen) ? (unsigned char)buf[pos++] : -1;
    }
    if (c >= 0) pos--;
    return neg ? -val : val;
}

// ---------- problem data ----------

struct Picture {
    int h, w;
    vector<string> rows;
};

struct Entry {
    bool isAbsolute;
    long offRow, offCol;   // pos-y, pos-x as literally read
    string baseId;         // valid only if !isAbsolute
    string fileName;
    long layer;
    bool resolved;
    long row, col;
    Entry() : resolved(false), row(0), col(0) {}
};

static const int MAXDIM = 1005;
static char canvas[MAXDIM][MAXDIM];

struct LayerCmp {
    const vector<Entry> *e;
    LayerCmp(const vector<Entry> *e_) : e(e_) {}
    bool operator()(int a, int b) const { return (*e)[a].layer < (*e)[b].layer; }
};

int main() {
    loadInput();

    long T = readTokenInt();

    // Reuse one output buffer across scenarios to avoid per-char stdio
    // call overhead on large outputs.
    string outBuf;
    outBuf.reserve(1 << 20);

    for (long tc = 1; tc <= T; tc++) {
        int n = (int)readTokenInt();
        map<string, int> pictureIndex;
        vector<Picture> pictures(n);
        for (int i = 0; i < n; i++) {
            string name = readToken();
            int h = (int)readTokenInt();
            int w = (int)readTokenInt();
            pictureIndex[name] = i;
            pictures[i].h = h;
            pictures[i].w = w;
            pictures[i].rows.resize(h);
            for (int r = 0; r < h; r++) {
                pictures[i].rows[r] = readToken();
            }
        }

        int m = (int)readTokenInt();

        vector<Entry> entries(m);
        map<string, int> entryIndex;

        for (int i = 0; i < m; i++) {
            getNonWs(); // '#'
            string id = readUntil('{');
            entryIndex[id] = i;

            expectLiteral("pos-x:");
            long px = readInt();
            expectLiteral("px;pos-y:");
            long py = readInt();
            expectLiteral("px;position:");

            int c = getNonWs();
            if (c == 'a') {
                expectLiteral("bsolute;");
                entries[i].isAbsolute = true;
            } else {
                // c == 'r' (relative)
                expectLiteral("elative=");
                entries[i].isAbsolute = false;
                entries[i].baseId = readUntil(';');
            }
            entries[i].offRow = py;
            entries[i].offCol = px;

            expectLiteral("file:");
            entries[i].fileName = readUntil(';');

            expectLiteral("layer:");
            entries[i].layer = readInt();
            expectLiteral(";}");
        }

        // Resolve positions via memoized resolution using an explicit
        // stack (handles any reference order; statement guarantees no
        // cycles), no recursion-depth concern for up to 500 entries.
        for (int i = 0; i < m; i++) {
            if (entries[i].resolved) continue;
            vector<int> stack;
            int cur = i;
            while (true) {
                if (entries[cur].resolved) break;
                if (entries[cur].isAbsolute) {
                    entries[cur].row = entries[cur].offRow;
                    entries[cur].col = entries[cur].offCol;
                    entries[cur].resolved = true;
                    break;
                }
                int base = entryIndex[entries[cur].baseId];
                if (entries[base].resolved) {
                    entries[cur].row = entries[base].row + entries[cur].offRow;
                    entries[cur].col = entries[base].col + entries[cur].offCol;
                    entries[cur].resolved = true;
                    break;
                }
                stack.push_back(cur);
                cur = base;
            }
            for (int k = (int)stack.size() - 1; k >= 0; k--) {
                int e = stack[k];
                int base = entryIndex[entries[e].baseId];
                entries[e].row = entries[base].row + entries[e].offRow;
                entries[e].col = entries[base].col + entries[e].offCol;
                entries[e].resolved = true;
            }
        }

        // stable sort by layer ascending
        vector<int> order(m);
        for (int i = 0; i < m; i++) order[i] = i;
        stable_sort(order.begin(), order.end(), LayerCmp(&entries));

        long maxRow = 0, maxCol = 0;
        for (int k = 0; k < m; k++) {
            const Entry &e = entries[order[k]];
            int pi = pictureIndex[e.fileName];
            const Picture &pic = pictures[pi];
            if (e.row + pic.h - 1 > maxRow) maxRow = e.row + pic.h - 1;
            if (e.col + pic.w - 1 > maxCol) maxCol = e.col + pic.w - 1;
        }
        // Defensive clamp: the statement guarantees the result fits in
        // 1000x1000, but never index outside the allocated canvas.
        if (maxRow >= MAXDIM) maxRow = MAXDIM - 1;
        if (maxCol >= MAXDIM) maxCol = MAXDIM - 1;

        for (long r = 0; r <= maxRow; r++) memset(canvas[r], ' ', maxCol + 1);

        for (int k = 0; k < m; k++) {
            const Entry &e = entries[order[k]];
            int pi = pictureIndex[e.fileName];
            const Picture &pic = pictures[pi];
            for (int r = 0; r < pic.h; r++) {
                const string &rowStr = pic.rows[r];
                long rr = e.row + r;
                if (rr < 0 || rr > maxRow) continue;
                int rowLen = (int)rowStr.size();
                int w = pic.w < rowLen ? pic.w : rowLen;
                for (int c2 = 0; c2 < w; c2++) {
                    char ch = rowStr[c2];
                    if (ch != '.') {
                        long cc = e.col + c2;
                        if (cc >= 0 && cc <= maxCol) canvas[rr][cc] = ch;
                    }
                }
            }
        }

        outBuf += "Scenario #";
        char numbuf[16];
        sprintf(numbuf, "%ld", tc);
        outBuf += numbuf;
        outBuf += ":\n";
        for (long r = 0; r <= maxRow; r++) {
            outBuf.append(canvas[r], maxCol + 1);
            outBuf += '\n';
        }
        outBuf += '\n';

        if (outBuf.size() > (1 << 22)) {
            fwrite(outBuf.data(), 1, outBuf.size(), stdout);
            outBuf.clear();
        }
    }

    fwrite(outBuf.data(), 1, outBuf.size(), stdout);
    return 0;
}
