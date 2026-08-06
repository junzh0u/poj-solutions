// POJ 2985 - The k-th Largest Group
// Model: gpt-5.6-terra
// Maintain disjoint-set component sizes and a Fenwick tree of their frequencies.
// A kth largest group is the (group_count-k+1)th smallest size in that tree.
// Merging two cats already in one component must leave both structures unchanged.
#include <cstdio>

class FastInput {
    static const int BUFFER_SIZE = 1 << 16;
    char buffer[BUFFER_SIZE];
    int position;
    int length;

public:
    FastInput() : position(0), length(0) {}

    int nextInt() {
        int c;
        int value = 0;
        do {
            c = nextChar();
        } while (c <= ' ');
        do {
            value = value * 10 + c - '0';
            c = nextChar();
        } while (c > ' ');
        return value;
    }

private:
    int nextChar() {
        if (position == length) {
            length = (int)std::fread(buffer, 1, BUFFER_SIZE, stdin);
            position = 0;
            if (length == 0) return EOF;
        }
        return buffer[position++];
    }
};

class Fenwick {
    int n;
    int *tree;

public:
    Fenwick(int size) : n(size), tree(new int[size + 1]) {
        int i;
        for (i = 0; i <= n; ++i) tree[i] = 0;
    }

    ~Fenwick() { delete[] tree; }

    void add(int index, int value) {
        while (index <= n) {
            tree[index] += value;
            index += index & -index;
        }
    }

    int kth(int rank) const {
        int index = 0;
        int step = 1;
        while ((step << 1) <= n) step <<= 1;
        while (step != 0) {
            int candidate = index + step;
            if (candidate <= n && tree[candidate] < rank) {
                index = candidate;
                rank -= tree[candidate];
            }
            step >>= 1;
        }
        return index + 1;
    }
};

static int findRoot(int node, int parent[]) {
    int root = node;
    while (parent[root] != root) root = parent[root];
    while (node != root) {
        int next = parent[node];
        parent[node] = root;
        node = next;
    }
    return root;
}

int main() {
    FastInput input;
    int n = input.nextInt();
    int m = input.nextInt();
    int *parent = new int[n + 1];
    int *componentSize = new int[n + 1];
    Fenwick frequencies(n);
    int groups = n;
    int operation;
    int i;

    for (i = 1; i <= n; ++i) {
        parent[i] = i;
        componentSize[i] = 1;
    }
    frequencies.add(1, n);

    for (operation = 0; operation < m; ++operation) {
        int kind = input.nextInt();
        if (kind == 0) {
            int first = findRoot(input.nextInt(), parent);
            int second = findRoot(input.nextInt(), parent);
            if (first != second) {
                int combined;
                if (componentSize[first] < componentSize[second]) {
                    int swap = first;
                    first = second;
                    second = swap;
                }
                frequencies.add(componentSize[first], -1);
                frequencies.add(componentSize[second], -1);
                combined = componentSize[first] + componentSize[second];
                parent[second] = first;
                componentSize[first] = combined;
                frequencies.add(combined, 1);
                --groups;
            }
        } else {
            int k = input.nextInt();
            std::printf("%d\n", frequencies.kth(groups - k + 1));
        }
    }

    delete[] componentSize;
    delete[] parent;
    return 0;
}
