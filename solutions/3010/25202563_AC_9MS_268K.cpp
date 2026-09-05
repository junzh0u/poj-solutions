// POJ 3010 - The Genome Database of All Space Life
// Model: claude-sonnet-5
//
// The compressed genome grammar is: seq := token*; token := digits '(' seq ')'
// | digits letter | letter. Repetition counts are up to 1000 and nest up to
// ~33 deep in a 100-char string, so the true expanded length can reach
// 1000^33 and overflows any integer type almost immediately. Since queries
// only ask for an index i <= 1,000,000, every length only needs to be known
// exactly up to a cap just above that bound; beyond the cap we only need to
// know "this is already longer than any possible query", never the exact
// value. So every computed length (a leaf's count, a group's unit length
// times its repeat count, a sequence's summed length) is clamped at CAP.
// Because capping only ever rounds a value DOWN toward CAP, a capped length
// that exceeds the query index i still proves the true length exceeds i,
// and a capped length that is <= i (only possible when nothing along the
// way was actually capped) equals the true length exactly. That invariant
// lets the same capped arithmetic answer both "is index i present" and,
// during the recursive descent to find the i-th letter, "which repetition /
// which sibling token holds i" (a group's modulus is only taken against an
// exact, non-capped unit length; once a unit length is capped we know i
// falls in the first repetition and recurse directly).
//
// No statement ambiguity: the grammar and the sample both parse
// unambiguously, and the discuss board's traffic was about MLE/parsing
// tedium rather than any genuine misreading.
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
using namespace std;

typedef long long ll;
static const ll CAP = 2000010LL; // > max possible query index (1,000,000)

struct Node {
    bool leaf;
    char ch;
    ll count;
    vector<Node> seq;   // valid when !leaf
    ll unitLen;         // capped length of one repetition unit (1 if leaf)
    ll totalLen;         // capped length = count * unitLen, clamped at CAP
};

static ll seqLen(const vector<Node>& v) {
    ll s = 0;
    for (size_t k = 0; k < v.size(); ++k) s += v[k].totalLen;
    return s; // bounded by (#tokens)*CAP, always fits in ll
}

static Node parseToken(const string& s, size_t& pos) {
    Node node;
    if (isdigit((unsigned char)s[pos])) {
        ll num = 0;
        while (pos < s.size() && isdigit((unsigned char)s[pos])) {
            num = num * 10 + (s[pos] - '0');
            ++pos;
        }
        if (s[pos] == '(') {
            ++pos; // skip '('
            node.leaf = false;
            node.count = num;
            // parse subsequence
            while (pos < s.size() && s[pos] != ')') {
                node.seq.push_back(parseToken(s, pos));
            }
            ++pos; // skip ')'
            node.unitLen = seqLen(node.seq);
            if (node.unitLen >= CAP) {
                node.totalLen = CAP;
            } else {
                ll prod = node.count * node.unitLen;
                node.totalLen = (prod >= CAP) ? CAP : prod;
            }
        } else {
            node.leaf = true;
            node.count = num;
            node.ch = s[pos];
            ++pos;
            node.unitLen = 1;
            node.totalLen = node.count; // count <= 1000 < CAP
        }
    } else {
        node.leaf = true;
        node.count = 1;
        node.ch = s[pos];
        ++pos;
        node.unitLen = 1;
        node.totalLen = 1;
    }
    return node;
}

static vector<Node> parseTop(const string& s) {
    vector<Node> result;
    size_t pos = 0;
    while (pos < s.size()) {
        result.push_back(parseToken(s, pos));
    }
    return result;
}

static char locateSeq(const vector<Node>& v, ll i);

static char locateToken(const Node& t, ll i) {
    if (t.leaf) {
        return t.ch;
    }
    if (t.unitLen >= CAP) {
        // one repetition already exceeds any possible query index; i falls
        // within the first repetition
        return locateSeq(t.seq, i);
    }
    ll idx = i % t.unitLen;
    return locateSeq(t.seq, idx);
}

static char locateSeq(const vector<Node>& v, ll i) {
    for (size_t k = 0; k < v.size(); ++k) {
        if (i < v[k].totalLen) {
            return locateToken(v[k], i);
        }
        i -= v[k].totalLen;
    }
    // should not happen if caller already checked bounds
    return '0';
}

int main() {
    char buf[200];
    ll idx;
    while (scanf("%s %lld", buf, &idx) == 2) {
        string s(buf);
        if (s == "0" && idx == 0) break;
        vector<Node> top = parseTop(s);
        ll total = seqLen(top);
        if (idx >= total) {
            printf("0\n");
        } else {
            char c = locateSeq(top, idx);
            printf("%c\n", c);
        }
    }
    return 0;
}
