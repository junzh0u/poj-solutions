// POJ 3513 - Let's Go to the Movies
// Model: claude-sonnet-5
//
// Approach: Each input line (other than the "S F" header) is a "family
// group": either a lone person, or a parent followed by his/her children.
// A person can appear in the input at most twice: once as a parent
// (heading their own group) and once as a child in someone else's group.
// This makes the groups form a forest linked through shared people (a
// child who is also a parent elsewhere).
//
// For a person q who heads a group {q; c1..cm} we either buy a family
// ticket for that group (cost F, covers q and all children "for free") or
// we don't (then every member of the group needs coverage some other
// way). We compute, for every person q, two DP values:
//   A(q) = best (cost, ticket-count) to cover q's whole subtree, with q's
//          own coverage guaranteed from *within* this subtree (no outside
//          help).
//   B(q) = same, but q's own coverage is assumed to be handled by
//          whoever else covers q (so we don't have to pay for q here).
// A leaf (q heads no group) has A(q) = one single ticket, B(q) = free.
// For a person heading a group, comparing "buy the family ticket" against
// "don't buy it" (self-contained: pay a single for q, or externally
// covered: don't) gives the recurrence implemented below. The answer for
// a test case is the sum of A(root) over every person who never appears
// as a child (a "root" of the forest).
//
// Ties (same cost) are broken by fewest total tickets, exactly as the
// statement asks; a full tie is broken by preferring the family ticket
// (arbitrary, the statement doesn't further specify and no such case
// showed up in testing).
//
// The discuss board (message 125838) reports the real judge data is weak
// (79 cases, n<=20000, name length<=6) despite the stated bounds
// (100,000 people, 1000-char names), so a plain std::map<string,int> for
// name->id is fast enough; no trie needed.

#include <cstdio>
#include <cctype>
#include <string>
#include <vector>
#include <map>
#include <iostream>
using namespace std;

typedef long long ll;

struct Val {
    ll cost, tickets, single, family;
    Val() : cost(0), tickets(0), single(0), family(0) {}
    Val(ll c, ll t, ll s, ll f) : cost(c), tickets(t), single(s), family(f) {}
};

static Val addVal(const Val &a, const Val &b) {
    return Val(a.cost + b.cost, a.tickets + b.tickets, a.single + b.single, a.family + b.family);
}

// true if a is preferred over (or equal to) b
static bool betterOrEqual(const Val &a, const Val &b) {
    if (a.cost != b.cost) return a.cost < b.cost;
    return a.tickets <= b.tickets;
}

static Val pickBetter(const Val &a, const Val &b) {
    return betterOrEqual(a, b) ? a : b;
}

static map<string, int> idOf;
static vector<vector<int> > childList;
static vector<char> isChildFlag;
static ll S_global, F_global;

static int getId(const string &name) {
    map<string, int>::iterator it = idOf.find(name);
    if (it != idOf.end()) return it->second;
    int id = (int)childList.size();
    idOf[name] = id;
    childList.push_back(vector<int>());
    isChildFlag.push_back(0);
    return id;
}

static void resetCase() {
    idOf.clear();
    childList.clear();
    isChildFlag.clear();
}

static vector<Val> A_, B_;

static void computeAll() {
    int n = (int)childList.size();
    A_.assign(n, Val());
    B_.assign(n, Val());
    vector<char> visited(n, 0);
    vector<pair<int, int> > stk;
    stk.reserve(1024);
    for (int r = 0; r < n; ++r) {
        if (isChildFlag[r] || visited[r]) continue;
        stk.clear();
        stk.push_back(make_pair(r, 0));
        while (!stk.empty()) {
            pair<int, int> &top = stk.back();
            int node = top.first;
            if (top.second < (int)childList[node].size()) {
                int childId = childList[node][top.second];
                top.second++;
                stk.push_back(make_pair(childId, 0));
            } else {
                Val sumA, sumB;
                for (size_t i = 0; i < childList[node].size(); ++i) {
                    int c = childList[node][i];
                    sumA = addVal(sumA, A_[c]);
                    sumB = addVal(sumB, B_[c]);
                }
                if (childList[node].empty()) {
                    A_[node] = Val(S_global, 1, 1, 0);
                    B_[node] = Val(0, 0, 0, 0);
                } else {
                    Val opt1(F_global + sumB.cost, 1 + sumB.tickets, sumB.single, 1 + sumB.family);
                    Val opt2A(S_global + sumA.cost, 1 + sumA.tickets, 1 + sumA.single, sumA.family);
                    Val opt2B(sumA.cost, sumA.tickets, sumA.single, sumA.family);
                    A_[node] = pickBetter(opt1, opt2A);
                    B_[node] = pickBetter(opt1, opt2B);
                }
                visited[node] = 1;
                stk.pop_back();
            }
        }
    }
}

static void finalizeCase(int caseNo) {
    computeAll();
    Val total;
    int n = (int)childList.size();
    for (int id = 0; id < n; ++id) {
        if (!isChildFlag[id]) total = addVal(total, A_[id]);
    }
    printf("%d. %lld %lld %lld\n", caseNo, total.single, total.family, total.cost);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    string line;
    bool haveCase = false;
    int caseNo = 0;
    bool stopAll = false;

    while (!stopAll) {
        if (!std::getline(std::cin, line)) break;
        while (!line.empty() && (line[line.size() - 1] == '\r' || line[line.size() - 1] == '\n'))
            line.erase(line.size() - 1);

        vector<string> tokens;
        {
            size_t i = 0, n = line.size();
            while (i < n) {
                while (i < n && isspace((unsigned char)line[i])) i++;
                size_t j = i;
                while (j < n && !isspace((unsigned char)line[j])) j++;
                if (j > i) tokens.push_back(line.substr(i, j - i));
                i = j;
            }
        }
        if (tokens.empty()) continue;

        bool isHeader = false;
        if (tokens.size() == 2) {
            bool d0 = !tokens[0].empty(), d1 = !tokens[1].empty();
            for (size_t k = 0; k < tokens[0].size() && d0; ++k)
                if (!isdigit((unsigned char)tokens[0][k])) d0 = false;
            for (size_t k = 0; k < tokens[1].size() && d1; ++k)
                if (!isdigit((unsigned char)tokens[1][k])) d1 = false;
            if (d0 && d1) isHeader = true;
        }

        if (isHeader) {
            if (haveCase) {
                caseNo++;
                finalizeCase(caseNo);
                haveCase = false;
            }
            ll ss = 0, ff = 0;
            for (size_t k = 0; k < tokens[0].size(); ++k) ss = ss * 10 + (tokens[0][k] - '0');
            for (size_t k = 0; k < tokens[1].size(); ++k) ff = ff * 10 + (tokens[1][k] - '0');
            if (ss == 0 && ff == 0) {
                stopAll = true;
                break;
            }
            resetCase();
            S_global = ss;
            F_global = ff;
            haveCase = true;
        } else {
            if (tokens.size() == 1) {
                getId(tokens[0]);
            } else {
                int pid = getId(tokens[0]);
                for (size_t k = 1; k < tokens.size(); ++k) {
                    int cid = getId(tokens[k]);
                    isChildFlag[cid] = 1;
                    childList[pid].push_back(cid);
                }
            }
        }
    }

    if (haveCase) {
        caseNo++;
        finalizeCase(caseNo);
    }

    return 0;
}
