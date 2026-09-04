// POJ 3746 - Teacher YYF
// Model: claude-sonnet-5
//
// Grammar (per the corrected production table posted on the problem's
// discuss board, message 127085, with the erratum "prep.art.adj.n." for
// prepositional phrases removed -- confirmed by board reply 126732
// "You can ignore all the punctuations." and by the official sample):
//
//   S (subject) / O (object) := pron | art n | art adj n
//   I (intransitive VP)      := v | adv v | vi | adv vi
//   T (transitive VP)        := v | adv v | vt | adv vt
//   P (prepositional phrase) := prep pron | prep art n   (NOT prep art adj n)
//
//   sentence := [P] S [P] I [P]                (at most one P present)
//             | [P] S [P] T [P] O [P]          (at most one P present)
//
// Ambiguity resolved from the board (message 350xxx thread, message
// 127085/127205): a prepositional phrase's noun DOES require an article
// (bare "prep noun" is invalid; "prep art adj noun" is also invalid --
// that was fanhqme's original table, which lqp18_31 posted a correction
// for after getting Accepted). Sample sentence 6, "At the airport, he see
// a happy baby.", parses as P(at the airport) S(he) T(see) O(a happy
// baby) and is YES, which only works under this corrected grammar.
//
// Tokenization pitfalls found on the board:
//  - message 126732: "You can ignore all the punctuations" -- so commas
//    and periods are stripped entirely before matching a token against
//    the dictionary (case-insensitive).
//  - message 358786: a single sentence's words can be split across more
//    than one physical input line, so sentences must NOT be read with
//    one getline() per sentence. Instead words are read token-by-token
//    (whitespace/newline-agnostic) and a sentence ends at the first token
//    that contains a '.' character.
//
// Implementation: since every dictionary word has exactly one fixed tag,
// a sentence's tag sequence is fully determined once tokenized, so the
// grammar check is a small deterministic backtracking match trying each
// of the (at most 4/5) legal slots for the single optional P.

#include <cstdio>
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cctype>
using namespace std;

enum Tag { T_N, T_PRON, T_ADJ, T_ADV, T_PREP, T_ART, T_VT, T_VI, T_V, T_UNKNOWN };

map<string, Tag> dict;

string cleanWord(const string &s) {
    string r;
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (isalpha((unsigned char)c)) r += (char)tolower((unsigned char)c);
    }
    return r;
}

Tag tagOf(const string &word) {
    map<string, Tag>::iterator it = dict.find(word);
    if (it == dict.end()) return T_UNKNOWN;
    return it->second;
}

// deterministic matchers: return end index (exclusive) on success, -1 on failure
int matchNP(const vector<Tag> &tag, int pos) {
    int L = (int)tag.size();
    if (pos >= L) return -1;
    if (tag[pos] == T_PRON) return pos + 1;
    if (tag[pos] == T_ART) {
        if (pos + 1 < L && tag[pos + 1] == T_N) return pos + 2;
        if (pos + 1 < L && tag[pos + 1] == T_ADJ && pos + 2 < L && tag[pos + 2] == T_N) return pos + 3;
    }
    return -1;
}

int matchPP(const vector<Tag> &tag, int pos) {
    int L = (int)tag.size();
    if (pos >= L || tag[pos] != T_PREP) return -1;
    if (pos + 1 < L && tag[pos + 1] == T_PRON) return pos + 2;
    if (pos + 1 < L && tag[pos + 1] == T_ART && pos + 2 < L && tag[pos + 2] == T_N) return pos + 3;
    return -1;
}

int matchVP(const vector<Tag> &tag, int pos, bool trans) {
    int L = (int)tag.size();
    if (pos >= L) return -1;
    Tag specific = trans ? T_VT : T_VI;
    if (tag[pos] == specific || tag[pos] == T_V) return pos + 1;
    if (tag[pos] == T_ADV && pos + 1 < L && (tag[pos + 1] == specific || tag[pos + 1] == T_V)) return pos + 2;
    return -1;
}

bool tryStruct1(const vector<Tag> &tag) {
    int L = (int)tag.size();
    int cfgs[4][3] = { {0,0,0}, {1,0,0}, {0,1,0}, {0,0,1} };
    for (int c = 0; c < 4; ++c) {
        int pos = 0;
        bool ok = true;
        if (cfgs[c][0]) { int r = matchPP(tag, pos); if (r < 0) { ok = false; } else pos = r; }
        if (ok) { int r = matchNP(tag, pos); if (r < 0) ok = false; else pos = r; }
        if (ok && cfgs[c][1]) { int r = matchPP(tag, pos); if (r < 0) ok = false; else pos = r; }
        if (ok) { int r = matchVP(tag, pos, false); if (r < 0) ok = false; else pos = r; }
        if (ok && cfgs[c][2]) { int r = matchPP(tag, pos); if (r < 0) ok = false; else pos = r; }
        if (ok && pos == L) return true;
    }
    return false;
}

bool tryStruct2(const vector<Tag> &tag) {
    int L = (int)tag.size();
    int cfgs[5][4] = { {0,0,0,0}, {1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1} };
    for (int c = 0; c < 5; ++c) {
        int pos = 0;
        bool ok = true;
        if (cfgs[c][0]) { int r = matchPP(tag, pos); if (r < 0) ok = false; else pos = r; }
        if (ok) { int r = matchNP(tag, pos); if (r < 0) ok = false; else pos = r; }
        if (ok && cfgs[c][1]) { int r = matchPP(tag, pos); if (r < 0) ok = false; else pos = r; }
        if (ok) { int r = matchVP(tag, pos, true); if (r < 0) ok = false; else pos = r; }
        if (ok && cfgs[c][2]) { int r = matchPP(tag, pos); if (r < 0) ok = false; else pos = r; }
        if (ok) { int r = matchNP(tag, pos); if (r < 0) ok = false; else pos = r; }
        if (ok && cfgs[c][3]) { int r = matchPP(tag, pos); if (r < 0) ok = false; else pos = r; }
        if (ok && pos == L) return true;
    }
    return false;
}

int main() {
    ios::sync_with_stdio(false);
    int n, m;
    if (!(cin >> n >> m)) return 0;
    for (int i = 0; i < n; ++i) {
        string w, f;
        cin >> w >> f;
        w = cleanWord(w);
        Tag t;
        if (f == "n.") t = T_N;
        else if (f == "pron.") t = T_PRON;
        else if (f == "adj.") t = T_ADJ;
        else if (f == "adv.") t = T_ADV;
        else if (f == "prep.") t = T_PREP;
        else if (f == "art.") t = T_ART;
        else if (f == "vt.") t = T_VT;
        else if (f == "vi.") t = T_VI;
        else if (f == "v.") t = T_V;
        else t = T_UNKNOWN;
        dict[w] = t;
    }

    string tok;
    vector<string> outputs;
    outputs.reserve(m);
    for (int s = 0; s < m; ++s) {
        vector<Tag> tags;
        bool unknown = false;
        while (true) {
            if (!(cin >> tok)) break; // shouldn't happen with valid input
            bool isEnd = (tok.find('.') != string::npos);
            string cw = cleanWord(tok);
            if (!cw.empty()) {
                Tag t = tagOf(cw);
                if (t == T_UNKNOWN) unknown = true;
                tags.push_back(t);
            }
            if (isEnd) break;
        }
        bool ok = false;
        if (!unknown) {
            ok = tryStruct1(tags) || tryStruct2(tags);
        }
        outputs.push_back(ok ? "YES" : "NO");
    }
    for (size_t i = 0; i < outputs.size(); ++i) {
        cout << outputs[i] << "\n";
    }
    return 0;
}
