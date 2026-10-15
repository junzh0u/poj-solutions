// POJ 2320 - SHEET
// Model: gpt-6.1-sol
// Remove cells with no remaining dependencies, propagating to their users.
// What remains reaches a cycle. All syntactic references count, including
// those multiplied by zero or cancelled algebraically, as clarified.
#include <iostream>
#include <string>
#include <vector>
#include <queue>
using namespace std;
int cell(const string &s, int p) {
    int r=(s[p+1]-'0')*10+s[p+2]-'0';
    int c=(s[p+4]-'0')*10+s[p+5]-'0';
    return (r-1)*20+c-1;
}
int main() {
    vector<int> users[400];
    int degree[400]={0};
    vector<int> order;
    vector<string> names;
    string s;
    while (cin >> s) {
        int u=cell(s,0);
        order.push_back(u);
        names.push_back(s.substr(0,6));
        for (int p=7;p<(int)s.size();++p) {
            if (s[p]=='R') {
                int v=cell(s,p);
                users[v].push_back(u);
                ++degree[u];
                p+=5;
            }
        }
    }
    queue<int> q;
    for (int i=0;i<(int)order.size();++i)
        if (!degree[order[i]]) q.push(order[i]);
    while (!q.empty()) {
        int v=q.front(); q.pop();
        for (int j=0;j<(int)users[v].size();++j) {
            int u=users[v][j];
            if (--degree[u]==0) q.push(u);
        }
    }
    for (int i=0;i<(int)order.size();++i)
        cout << names[i] << (degree[order[i]] ? " circular" : " ok") << '\n';
    return 0;
}
