// POJ 2217 - Secretary
// Model: gpt-5.6-terra
// Build a suffix automaton for the first line, then follow its transitions
// while scanning the second line.  The longest current match is the longest
// common substring ending at the current character.
// Lines are read with getline because spaces (and empty lines) are text.
#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>

using namespace std;

const int MAX_STATE = 20005;

struct State {
    int next[256];
    int link;
    int len;
};

State st[MAX_STATE];
int states;
int last_state;

void init_state(int v) {
    memset(st[v].next, -1, sizeof(st[v].next));
    st[v].link = -1;
    st[v].len = 0;
}

void extend(unsigned char c) {
    int cur = states++;
    init_state(cur);
    st[cur].len = st[last_state].len + 1;
    int p = last_state;
    while (p != -1 && st[p].next[c] == -1) {
        st[p].next[c] = cur;
        p = st[p].link;
    }
    if (p == -1) {
        st[cur].link = 0;
    } else {
        int q = st[p].next[c];
        if (st[p].len + 1 == st[q].len) {
            st[cur].link = q;
        } else {
            int clone = states++;
            st[clone] = st[q];
            st[clone].len = st[p].len + 1;
            while (p != -1 && st[p].next[c] == q) {
                st[p].next[c] = clone;
                p = st[p].link;
            }
            st[q].link = st[cur].link = clone;
        }
    }
    last_state = cur;
}

int longest_common_substring(const string& a, const string& b) {
    states = 1;
    last_state = 0;
    init_state(0);
    for (size_t i = 0; i < a.size(); ++i) extend((unsigned char)a[i]);

    int v = 0;
    int matched = 0;
    int answer = 0;
    for (size_t i = 0; i < b.size(); ++i) {
        unsigned char c = (unsigned char)b[i];
        while (v != 0 && st[v].next[c] == -1) {
            v = st[v].link;
            matched = st[v].len;
        }
        if (st[v].next[c] != -1) {
            v = st[v].next[c];
            ++matched;
        } else {
            matched = 0;
        }
        if (matched > answer) answer = matched;
    }
    return answer;
}

int main() {
    int tests;
    if (!(cin >> tests)) return 0;
    string a, b;
    getline(cin, a);
    while (tests--) {
        getline(cin, a);
        getline(cin, b);
        if (!a.empty() && a[a.size() - 1] == '\r') a.erase(a.size() - 1);
        if (!b.empty() && b[b.size() - 1] == '\r') b.erase(b.size() - 1);
        cout << "Nejdelsi spolecny retezec ma delku "
             << longest_common_substring(a, b) << ".\n";
    }
    return 0;
}
