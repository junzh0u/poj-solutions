// Problem 2380 - Sales Report
// Model: qwen3.6:latest

#include <cstdio>
#include <map>
#include <set>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    int N;
    scanf("%d", &N);

    // Aggregate sales: (item_id, spoint_id) -> total quantity
    map<pair<int,int>, int> m;
    set<int> items, spoints;

    for (int i = 0; i < N; i++) {
        int q, s, v;
        scanf("%d %d %d", &q, &s, &v);
        auto key = make_pair(q, s);
        m[key] += v;
        items.insert(q);
        spoints.insert(s);
    }

    // Group by salespoint (row): sorted vector of (item_id, total)
    map<int, vector<pair<int,int>>> sp_data;
    for (auto it = m.begin(); it != m.end(); ++it) {
        sp_data[it->first.second].push_back({it->first.first, it->second});
    }
    for (auto it = sp_data.begin(); it != sp_data.end(); ++it) {
        sort(it->second.begin(), it->second.end());
    }

    // Collect into vectors for iteration
    vector<int> sp_list(spoints.begin(), spoints.end());
    vector<int> item_list(items.begin(), items.end());

    // Print header row: -1 followed by sorted item ids
    printf("-1");
    for (size_t i = 0; i < item_list.size(); ++i) {
        printf(" %d", item_list[i]);
    }
    printf("\n");

    // Print each salespoint row using two-pointer walk per row
    for (size_t si = 0; si < sp_list.size(); ++si) {
        int sp = sp_list[si];
        int j = 0;
        const vector<pair<int,int>>& data = sp_data[sp];

        printf("%d", sp);
        for (size_t ii = 0; ii < item_list.size(); ++ii) {
            while (j < (int)data.size() && data[j].first < item_list[ii]) j++;
            if (j < (int)data.size() && data[j].first == item_list[ii]) {
                printf(" %d", data[j++].second);
            } else {
                printf(" 0");
            }
        }
        printf("\n");
    }

    return 0;
}
