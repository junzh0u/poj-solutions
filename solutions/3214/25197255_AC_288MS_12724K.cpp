// POJ 3214 - Heap
// Model: gpt-5.6-terra
// Keep the largest subset of unchanged keys.  A postorder linear extension
// turns every heap constraint into a nondecreasing sequence constraint; each
// left-to-right subtree boundary is strict and is removed by its offset.
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

vector<int> value;
vector<int> sequence;
int offset = 0;

void visit(int node) {
    const int left = node * 2 + 1;
    const int right = node * 2 + 2;
    if (left < static_cast<int>(value.size())) visit(left);
    if (right < static_cast<int>(value.size())) {
        ++offset;
        visit(right);
    }
    sequence.push_back(value[node] - offset);
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int height;
    if (!(cin >> height)) return 0;
    int x;
    while (cin >> x) value.push_back(x);
    if (value.empty()) {
        cout << 0 << '\n';
        return 0;
    }

    visit(0);
    vector<int> tails;
    for (size_t i = 0; i < sequence.size(); ++i) {
        const int x = sequence[i];
        vector<int>::iterator it = upper_bound(tails.begin(), tails.end(), x);
        if (it == tails.end()) tails.push_back(x);
        else *it = x;
    }
    cout << value.size() - tails.size() << '\n';
    return 0;
}
