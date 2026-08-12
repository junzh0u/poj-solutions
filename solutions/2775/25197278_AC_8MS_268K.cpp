// POJ 2775 - The Number of the Same BST
// Model: gpt-5.6-terra
// Build the BST induced by the given insertion vector.  For a node, any
// interleaving of valid left- and right-subtree insertion vectors is valid,
// because their keys belong to disjoint sides of the node's key range.

#include <iostream>
using namespace std;

const int MOD = 9901;

struct Node {
    int key;
    int left;
    int right;
};

Node tree[100];
int nodes;
int choose_[100][100];

void insert_key(int p, int key) {
    if (key <= tree[p].key) {
        if (tree[p].left == -1) {
            tree[p].left = nodes;
            tree[nodes].key = key;
            tree[nodes].left = tree[nodes].right = -1;
            ++nodes;
        } else {
            insert_key(tree[p].left, key);
        }
    } else {
        if (tree[p].right == -1) {
            tree[p].right = nodes;
            tree[nodes].key = key;
            tree[nodes].left = tree[nodes].right = -1;
            ++nodes;
        } else {
            insert_key(tree[p].right, key);
        }
    }
}

int count_orders(int p, int &size) {
    int left_size, right_size;
    int left_ways = 1, right_ways = 1;
    if (p == -1) {
        size = 0;
        return 1;
    }
    left_ways = count_orders(tree[p].left, left_size);
    right_ways = count_orders(tree[p].right, right_size);
    size = left_size + right_size + 1;
    return ((left_ways * right_ways) % MOD * choose_[left_size + right_size][left_size]) % MOD;
}

int main() {
    int n, i, key, size;
    choose_[0][0] = 1;
    for (i = 1; i < 100; ++i) {
        choose_[i][0] = choose_[i][i] = 1;
        for (int j = 1; j < i; ++j)
            choose_[i][j] = (choose_[i - 1][j - 1] + choose_[i - 1][j]) % MOD;
    }
    while (cin >> n && n) {
        cin >> key;
        nodes = 1;
        tree[0].key = key;
        tree[0].left = tree[0].right = -1;
        for (i = 1; i < n; ++i) {
            cin >> key;
            insert_key(0, key);
        }
        cout << count_orders(0, size) << '\n';
    }
    return 0;
}
