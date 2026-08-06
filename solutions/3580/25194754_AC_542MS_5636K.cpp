// POJ 3580 - SuperMemo
// Model: gpt-5.6-terra
// Approach: an implicit randomized treap stores sequence order, subtree minimum,
// and lazy range-add and reversal tags. Splitting by position isolates every
// operation's interval; REVOLVE is a split at its normalized right rotation.
// Ambiguity handled: the unbounded REVOLVE count may be negative, so it is read
// as long long and normalized modulo the current interval length.
#include <cstdio>

const int MAXN = 200005;
const long long INF = (1LL << 60);

struct Node {
    int left;
    int right;
    int size;
    unsigned int priority;
    long long value;
    long long minimum;
    long long add;
    bool reverse;
};

Node tree[MAXN];
int nodes = 0;
unsigned int random_state = 2463534242U;

unsigned int next_random() {
    random_state ^= random_state << 13;
    random_state ^= random_state >> 17;
    random_state ^= random_state << 5;
    return random_state;
}

int size_of(int root) {
    return root == 0 ? 0 : tree[root].size;
}

long long minimum_of(int root) {
    return root == 0 ? INF : tree[root].minimum;
}

void apply_add(int root, long long delta) {
    if (root == 0) return;
    tree[root].value += delta;
    tree[root].minimum += delta;
    tree[root].add += delta;
}

void apply_reverse(int root) {
    if (root == 0) return;
    int temporary = tree[root].left;
    tree[root].left = tree[root].right;
    tree[root].right = temporary;
    tree[root].reverse = !tree[root].reverse;
}

void push(int root) {
    if (root == 0) return;
    if (tree[root].add != 0) {
        apply_add(tree[root].left, tree[root].add);
        apply_add(tree[root].right, tree[root].add);
        tree[root].add = 0;
    }
    if (tree[root].reverse) {
        apply_reverse(tree[root].left);
        apply_reverse(tree[root].right);
        tree[root].reverse = false;
    }
}

void pull(int root) {
    tree[root].size = size_of(tree[root].left) + size_of(tree[root].right) + 1;
    tree[root].minimum = tree[root].value;
    long long left_minimum = minimum_of(tree[root].left);
    long long right_minimum = minimum_of(tree[root].right);
    if (left_minimum < tree[root].minimum) tree[root].minimum = left_minimum;
    if (right_minimum < tree[root].minimum) tree[root].minimum = right_minimum;
}

int make_node(long long value) {
    ++nodes;
    tree[nodes].left = 0;
    tree[nodes].right = 0;
    tree[nodes].size = 1;
    tree[nodes].priority = next_random();
    tree[nodes].value = value;
    tree[nodes].minimum = value;
    tree[nodes].add = 0;
    tree[nodes].reverse = false;
    return nodes;
}

void split(int root, int left_size, int &left_tree, int &right_tree) {
    if (root == 0) {
        left_tree = 0;
        right_tree = 0;
        return;
    }
    push(root);
    if (size_of(tree[root].left) >= left_size) {
        split(tree[root].left, left_size, left_tree, tree[root].left);
        right_tree = root;
        pull(right_tree);
    } else {
        split(tree[root].right, left_size - size_of(tree[root].left) - 1,
              tree[root].right, right_tree);
        left_tree = root;
        pull(left_tree);
    }
}

int merge(int left_tree, int right_tree) {
    if (left_tree == 0) return right_tree;
    if (right_tree == 0) return left_tree;
    if (tree[left_tree].priority > tree[right_tree].priority) {
        push(left_tree);
        tree[left_tree].right = merge(tree[left_tree].right, right_tree);
        pull(left_tree);
        return left_tree;
    }
    push(right_tree);
    tree[right_tree].left = merge(left_tree, tree[right_tree].left);
    pull(right_tree);
    return right_tree;
}

int main() {
    int n;
    if (std::scanf("%d", &n) != 1) return 0;
    int root = 0;
    int i;
    for (i = 0; i < n; ++i) {
        long long value;
        std::scanf("%lld", &value);
        root = merge(root, make_node(value));
    }

    int operations;
    std::scanf("%d", &operations);
    char command[16];
    for (i = 0; i < operations; ++i) {
        std::scanf("%15s", command);
        if (command[0] == 'A') {
            int x, y;
            long long delta;
            std::scanf("%d%d%lld", &x, &y, &delta);
            int first, middle, last;
            split(root, x - 1, first, middle);
            split(middle, y - x + 1, middle, last);
            apply_add(middle, delta);
            root = merge(first, merge(middle, last));
        } else if (command[0] == 'R' && command[3] == 'E') {
            int x, y;
            std::scanf("%d%d", &x, &y);
            int first, middle, last;
            split(root, x - 1, first, middle);
            split(middle, y - x + 1, middle, last);
            apply_reverse(middle);
            root = merge(first, merge(middle, last));
        } else if (command[0] == 'R') {
            int x, y;
            long long turns;
            std::scanf("%d%d%lld", &x, &y, &turns);
            int length = y - x + 1;
            turns %= length;
            if (turns < 0) turns += length;
            if (turns != 0) {
                int first, middle, last, left_part, right_part;
                split(root, x - 1, first, middle);
                split(middle, length, middle, last);
                split(middle, length - (int)turns, left_part, right_part);
                root = merge(first, merge(right_part, merge(left_part, last)));
            }
        } else if (command[0] == 'I') {
            int x;
            long long value;
            std::scanf("%d%lld", &x, &value);
            int first, last;
            split(root, x, first, last);
            root = merge(first, merge(make_node(value), last));
        } else if (command[0] == 'D') {
            int x;
            std::scanf("%d", &x);
            int first, removed, last;
            split(root, x - 1, first, removed);
            split(removed, 1, removed, last);
            root = merge(first, last);
        } else {
            int x, y;
            std::scanf("%d%d", &x, &y);
            int first, middle, last;
            split(root, x - 1, first, middle);
            split(middle, y - x + 1, middle, last);
            std::printf("%lld\n", tree[middle].minimum);
            root = merge(first, merge(middle, last));
        }
    }
    return 0;
}
