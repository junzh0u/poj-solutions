// POJ 1479 - MBone
// Model: gpt-6.1-sol
// Directed all-pairs shortest tunnel costs give the greatest remaining TTL.
// Membership is evaluated when each packet is sent; TTL zero still delivers.
// Router storage follows the input count, including counts above the stated bound.
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
using namespace std;
struct Edge { int from, cost; string to; };
struct Delivery {
    int host, packet, ttl;
    bool operator<(const Delivery& b) const {
        if (host != b.host) return host < b.host;
        return packet < b.packet;
    }
};
int main() {
    int m, network = 0;
    while (cin >> m && m) {
        map<string, int> router;
        map<int, int> host;
        vector<Edge> edges;
        for (int i = 0; i < m; ++i) {
            string name; int n; cin >> name >> n; router[name] = i;
            while (n--) {
                char type; cin >> type;
                if (type == 'H') { int address; cin >> address; host[address] = i; }
                else { Edge e; e.from = i; cin >> e.cost >> e.to; edges.push_back(e); }
            }
        }
        vector< vector<int> > d(m, vector<int>(m, 1001));
        for (int i = 0; i < m; ++i) d[i][i] = 0;
        for (size_t i = 0; i < edges.size(); ++i) {
            int a = edges[i].from, b = router[edges[i].to];
            d[a][b] = min(d[a][b], edges[i].cost);
        }
        for (int k = 0; k < m; ++k)
            for (int i = 0; i < m; ++i)
                for (int j = 0; j < m; ++j)
                    d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
        map<int, set<int> > members;
        vector<Delivery> result;
        int n; cin >> n;
        while (n--) {
            char type; int h, group; cin >> type >> h >> group;
            if (type == 'J') members[group].insert(h);
            else if (type == 'L') members[group].erase(h);
            else {
                int packet, ttl; cin >> packet >> ttl;
                const set<int>& targets = members[group];
                for (set<int>::const_iterator it = targets.begin(); it != targets.end(); ++it) {
                    int cost = d[host[h]][host[*it]];
                    if (cost <= ttl) {
                        Delivery x; x.host = *it; x.packet = packet; x.ttl = ttl - cost;
                        result.push_back(x);
                    }
                }
            }
        }
        sort(result.begin(), result.end());
        cout << "Network #" << ++network << '\n';
        for (size_t i = 0; i < result.size(); ++i)
            cout << result[i].host << ' ' << result[i].packet << ' ' << result[i].ttl << '\n';
        cout << '\n';
    }
    return 0;
}
