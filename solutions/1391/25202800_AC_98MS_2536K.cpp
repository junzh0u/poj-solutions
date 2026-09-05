// POJ 1391 - Erdos Numbers
// Model: gpt-5.6-terra
// Treat each paper as a node in a bipartite author-paper graph.  Breadth-first
// search from Erdos gives the minimum number of papers in every coauthor chain.
// Paper lines are split into comma-delimited last/first-name pairs; the author
// portion is trimmed so the judge's known optional space before ':' is harmless.
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <queue>
#include <sstream>
#include <cctype>

using namespace std;

static string trim(const string &s) {
    string::size_type a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) ++a;
    while (b > a && isspace((unsigned char)s[b - 1])) --b;
    return s.substr(a, b - a);
}

static vector<string> authors(const string &line) {
    string::size_type colon = line.find(':');
    string part = trim(line.substr(0, colon));
    vector<string> pieces;
    string cur;
    stringstream ss(part);
    while (getline(ss, cur, ',')) pieces.push_back(trim(cur));
    vector<string> result;
    for (size_t i = 0; i + 1 < pieces.size(); i += 2)
        result.push_back(pieces[i] + ", " + pieces[i + 1]);
    return result;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    int p, n, cs = 0;
    while (cin >> p >> n && (p || n)) {
        string line;
        getline(cin, line);
        map<string, int> id;
        vector< vector<int> > graph;
        for (int paper = 0; paper < p; ++paper) {
            getline(cin, line);
            vector<string> list = authors(line);
            int paperNode = (int)graph.size();
            graph.push_back(vector<int>());
            for (size_t i = 0; i < list.size(); ++i) {
                map<string, int>::iterator it = id.find(list[i]);
                int v;
                if (it == id.end()) {
                    v = (int)graph.size();
                    id[list[i]] = v;
                    graph.push_back(vector<int>());
                } else v = it->second;
                graph[paperNode].push_back(v);
                graph[v].push_back(paperNode);
            }
        }
        vector<string> queries(n);
        for (int i = 0; i < n; ++i) getline(cin, queries[i]);
        vector<int> dist(graph.size(), -1);
        map<string, int>::iterator root = id.find("Erdos, P.");
        if (root != id.end()) {
            queue<int> q;
            dist[root->second] = 0;
            q.push(root->second);
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (size_t i = 0; i < graph[u].size(); ++i) {
                    int v = graph[u][i];
                    if (dist[v] == -1) {
                        dist[v] = dist[u] + 1;
                        q.push(v);
                    }
                }
            }
        }
        cout << "Database #" << ++cs << "\n";
        for (int i = 0; i < n; ++i) {
            map<string, int>::iterator it = id.find(queries[i]);
            cout << queries[i] << ": ";
            if (it == id.end() || dist[it->second] == -1) cout << "infinity\n";
            else cout << dist[it->second] / 2 << "\n";
        }
        cout << "\n";
    }
    return 0;
}
