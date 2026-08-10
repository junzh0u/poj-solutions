// POJ 2758 - Checking the Text
// Model: gpt-5.6-terra
// Build one suffix array and LCP RMQ for the immutable original text. In the
// current text, skip maximal original-character runs with that LCP oracle and
// compare inserted boundary characters directly. Query indices continue to
// name original characters, while inserted characters participate in matching.

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const vector<int> *comparison_rank;
int comparison_gap;
int comparison_length;

struct SuffixLess {
    bool operator()(int left, int right) const {
        if ((*comparison_rank)[left] != (*comparison_rank)[right]) {
            return (*comparison_rank)[left] < (*comparison_rank)[right];
        }
        const int left_second = left + comparison_gap < comparison_length
                                    ? (*comparison_rank)[left + comparison_gap]
                                    : -1;
        const int right_second = right + comparison_gap < comparison_length
                                     ? (*comparison_rank)[right + comparison_gap]
                                     : -1;
        return left_second < right_second;
    }
};

class OriginalLcp {
public:
    explicit OriginalLcp(const string &text) : text_(text) {
        build_suffix_array();
        build_lcp();
        build_rmq();
    }

    int query(int first, int second) const {
        if (first == second) {
            return static_cast<int>(text_.size()) - first;
        }
        int left = suffix_rank_[first];
        int right = suffix_rank_[second];
        if (left > right) {
            swap(left, right);
        }
        ++left;
        const int length = right - left + 1;
        const int level = logarithm_[length];
        return min(rmq_[level][left],
                   rmq_[level][right - (1 << level) + 1]);
    }

private:
    string text_;
    vector<int> suffix_array_;
    vector<int> suffix_rank_;
    vector<int> lcp_;
    vector<int> logarithm_;
    vector< vector<int> > rmq_;

    void build_suffix_array() {
        const int n = static_cast<int>(text_.size());
        suffix_array_.resize(n);
        vector<int> rank(n);
        vector<int> next_rank(n);
        for (int i = 0; i < n; ++i) {
            suffix_array_[i] = i;
            rank[i] = static_cast<unsigned char>(text_[i]);
        }

        for (int gap = 1; ; gap *= 2) {
            comparison_rank = &rank;
            comparison_gap = gap;
            comparison_length = n;
            sort(suffix_array_.begin(), suffix_array_.end(), SuffixLess());

            next_rank[suffix_array_[0]] = 0;
            SuffixLess less;
            for (int i = 1; i < n; ++i) {
                next_rank[suffix_array_[i]] =
                    next_rank[suffix_array_[i - 1]] +
                    (less(suffix_array_[i - 1], suffix_array_[i]) ? 1 : 0);
            }
            rank.swap(next_rank);
            if (rank[suffix_array_[n - 1]] == n - 1) {
                break;
            }
        }

        suffix_rank_.resize(n);
        for (int i = 0; i < n; ++i) {
            suffix_rank_[suffix_array_[i]] = i;
        }
    }

    void build_lcp() {
        const int n = static_cast<int>(text_.size());
        lcp_.assign(n, 0);
        int common = 0;
        for (int position = 0; position < n; ++position) {
            const int rank = suffix_rank_[position];
            if (rank == 0) {
                common = 0;
                continue;
            }
            const int previous = suffix_array_[rank - 1];
            while (position + common < n && previous + common < n &&
                   text_[position + common] == text_[previous + common]) {
                ++common;
            }
            lcp_[rank] = common;
            if (common > 0) {
                --common;
            }
        }
    }

    void build_rmq() {
        const int n = static_cast<int>(text_.size());
        logarithm_.assign(n + 1, 0);
        for (int i = 2; i <= n; ++i) {
            logarithm_[i] = logarithm_[i / 2] + 1;
        }
        const int levels = logarithm_[n] + 1;
        rmq_.assign(levels, vector<int>(n, 0));
        rmq_[0] = lcp_;
        for (int level = 1; level < levels; ++level) {
            const int span = 1 << level;
            const int half = span >> 1;
            for (int i = 0; i + span <= n; ++i) {
                rmq_[level][i] = min(rmq_[level - 1][i],
                                     rmq_[level - 1][i + half]);
            }
        }
    }
};

struct Character {
    Character(char value, int original_index)
        : value(value), original_index(original_index) {}

    char value;
    int original_index;
};

void rebuild_positions(const vector<Character> &text,
                       vector<int> &original_position,
                       vector<int> &next_inserted) {
    const int length = static_cast<int>(text.size());
    next_inserted.assign(length + 1, length);
    int next = length;
    for (int position = length - 1; position >= 0; --position) {
        if (text[position].original_index < 0) {
            next = position;
        } else {
            original_position[text[position].original_index] = position;
        }
        next_inserted[position] = next;
    }
}

int dynamic_lcp(int first_original, int second_original,
                const vector<Character> &text,
                const vector<int> &original_position,
                const vector<int> &next_inserted,
                const OriginalLcp &original_lcp) {
    int first = original_position[first_original];
    int second = original_position[second_original];
    const int length = static_cast<int>(text.size());
    int matched = 0;

    while (first < length && second < length) {
        if (text[first].original_index >= 0 &&
            text[second].original_index >= 0) {
            const int first_run = next_inserted[first] - first;
            const int second_run = next_inserted[second] - second;
            const int run = min(first_run, second_run);
            const int static_match =
                original_lcp.query(text[first].original_index,
                                   text[second].original_index);
            const int skipped = min(run, static_match);
            matched += skipped;
            first += skipped;
            second += skipped;
            if (skipped < run) {
                break;
            }
        } else {
            if (text[first].value != text[second].value) {
                break;
            }
            ++matched;
            ++first;
            ++second;
        }
    }
    return matched;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(0);

    string initial;
    if (!(cin >> initial)) {
        return 0;
    }
    const OriginalLcp original_lcp(initial);

    vector<Character> text;
    text.reserve(initial.size() + 200);
    for (int i = 0; i < static_cast<int>(initial.size()); ++i) {
        text.push_back(Character(initial[i], i));
    }
    vector<int> original_position(initial.size());
    vector<int> next_inserted;
    rebuild_positions(text, original_position, next_inserted);

    int command_count;
    cin >> command_count;
    for (int command_index = 0; command_index < command_count; ++command_index) {
        char command;
        cin >> command;
        if (command == 'I') {
            char value;
            int position;
            cin >> value >> position;
            int insertion = position - 1;
            if (insertion > static_cast<int>(text.size())) {
                insertion = static_cast<int>(text.size());
            }
            text.insert(text.begin() + insertion, Character(value, -1));
            rebuild_positions(text, original_position, next_inserted);
        } else {
            int first;
            int second;
            cin >> first >> second;
            cout << dynamic_lcp(first - 1, second - 1, text,
                                original_position, next_inserted,
                                original_lcp) << '\n';
        }
    }
    return 0;
}
