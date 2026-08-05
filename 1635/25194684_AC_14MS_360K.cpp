// POJ 1635 - Subway tree systems
// Model: gpt-5.6-terra
// Canonicalize each rooted unordered tree by recursively sorting its children's
// canonical encodings. The DFS tour's 0/1 nesting already supplies the root.
// Tours are guaranteed correct, so parsing only has to match balanced pairs.
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

typedef map<vector<int>, int> CanonicalIds;

static int canonicalNode(const string &tour, int &pos, CanonicalIds &ids) {
    ++pos; // The '0' entering this node.
    vector<int> children;
    while (pos < static_cast<int>(tour.size()) && tour[pos] == '0') {
        children.push_back(canonicalNode(tour, pos, ids));
    }
    ++pos; // The matching '1'.
    sort(children.begin(), children.end());
    CanonicalIds::iterator found = ids.find(children);
    if (found != ids.end())
        return found->second;
    int id = static_cast<int>(ids.size()) + 1;
    ids[children] = id;
    return id;
}

static int canonical(const string &tour, CanonicalIds &ids) {
    int pos = 0;
    vector<int> children;
    while (pos < static_cast<int>(tour.size())) {
        children.push_back(canonicalNode(tour, pos, ids));
    }
    sort(children.begin(), children.end());
    CanonicalIds::iterator found = ids.find(children);
    if (found != ids.end())
        return found->second;
    int id = static_cast<int>(ids.size()) + 1;
    ids[children] = id;
    return id;
}

int main() {
    ios::sync_with_stdio(false);
    int cases;
    cin >> cases;
    string ignored;
    getline(cin, ignored);
    while (cases--) {
        string first, second;
        getline(cin, first);
        getline(cin, second);
        if (!first.empty() && first[first.size() - 1] == '\r')
            first.erase(first.size() - 1);
        if (!second.empty() && second[second.size() - 1] == '\r')
            second.erase(second.size() - 1);
        CanonicalIds ids;
        cout << (canonical(first, ids) == canonical(second, ids) ? "same" : "different")
             << '\n';
    }
    return 0;
}
