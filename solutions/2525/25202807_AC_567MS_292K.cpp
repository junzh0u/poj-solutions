// POJ 2525 - Text Formalization
// Model: gpt-5.6-terra
// Scan every original input line from left to right.  At a position, the first
// rule in the two input lists that matches is substituted; each acronym can be
// chosen only once per text.  Since output is appended directly, substitutions
// are never considered again, as required by the statement.
// The rules accept the listed spelling, its all-uppercase spelling, and its
// capitalized spelling for contractions; exact spelling is required for acronyms.
// Public contest data also permits variable spacing around the arrow and rules
// embedded in arbitrary punctuation, so rules are parsed by their quote marks.

#include <cctype>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Rule {
    string left;
    string right;
    bool contraction;
};

static string upperCase(const string &s) {
    string r = s;
    for (size_t i = 0; i < r.size(); ++i)
        r[i] = static_cast<char>(toupper(static_cast<unsigned char>(r[i])));
    return r;
}

static string capitalized(const string &s) {
    string r = s;
    if (!r.empty())
        r[0] = static_cast<char>(toupper(static_cast<unsigned char>(r[0])));
    return r;
}

static Rule parseRule(const string &s, bool contraction) {
    Rule r;
    size_t a = s.find('"');
    size_t b = s.find('"', a + 1);
    size_t c = s.find('"', b + 1);
    size_t d = s.find('"', c + 1);
    r.left = s.substr(a + 1, b - a - 1);
    r.right = s.substr(c + 1, d - c - 1);
    r.contraction = contraction;
    return r;
}

static bool startsAt(const string &line, size_t pos, const string &word) {
    return pos + word.size() <= line.size() && line.compare(pos, word.size(), word) == 0;
}

static string expandLine(const string &line, const vector<Rule> &rules, vector<bool> &usedAcronym, bool &terminated) {
    string out;
    for (size_t pos = 0; pos < line.size();) {
        if (line[pos] == '#' && (pos + 1 == line.size() || line[pos + 1] != '#')) {
            out += '#';
            usedAcronym.assign(rules.size(), false);
            terminated = true;
            break;
        }
        int chosen = -1;
        int style = 0; // 0 listed, 1 uppercase, 2 capitalized
        // Input order resolves rules starting at this position.  Within a rule,
        // test its three permitted spellings in the stated order.
        for (size_t k = 0; k < rules.size(); ++k) {
            if (!rules[k].contraction && usedAcronym[k])
                continue;
            if (startsAt(line, pos, rules[k].left)) {
                chosen = static_cast<int>(k);
                style = 0;
                break;
            }
            if (rules[k].contraction && startsAt(line, pos, upperCase(rules[k].left))) {
                chosen = static_cast<int>(k);
                style = 1;
                break;
            }
            if (rules[k].contraction && startsAt(line, pos, capitalized(rules[k].left))) {
                chosen = static_cast<int>(k);
                style = 2;
                break;
            }
        }
        if (chosen < 0) {
            out += line[pos++];
            continue;
        }
        const Rule &r = rules[chosen];
        if (r.contraction) {
            if (style == 0) out += r.right;
            else if (style == 1) out += upperCase(r.right);
            else out += capitalized(r.right);
        } else {
            out += r.right + " (" + r.left + ")";
            usedAcronym[chosen] = true;
        }
        pos += r.left.size();
    }
    return out;
}

int main() {
    int c, a;
    if (!(cin >> c >> a)) return 0;
    string line;
    getline(cin, line);
    vector<Rule> rules;
    for (int i = 0; i < c; ++i) {
        getline(cin, line);
        while (line.empty()) getline(cin, line);
        rules.push_back(parseRule(line, true));
    }
    for (int i = 0; i < a; ++i) {
        getline(cin, line);
        while (line.empty()) getline(cin, line);
        rules.push_back(parseRule(line, false));
    }
    // Public data may put one blank separator after the rule lists.
    if (cin.peek() == '\n') getline(cin, line);
    vector<bool> usedAcronym(rules.size(), false);
    while (getline(cin, line)) {
        bool terminated = false;
        cout << expandLine(line, rules, usedAcronym, terminated) << '\n';
    }
    return 0;
}
