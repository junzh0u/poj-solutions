#include<vector>
#include<queue>
#include<iostream>
#include<cstring>

using namespace std;

#define N 10010

vector<int> _edge[N];
int visted[N];

void BFS(int x) {
    memset(visted, -1, sizeof(visted));
    queue<int> que;
    que.push(x);
    visted[x] = 0;
    while(!que.empty()) {
        int x = que.front();
        que.pop();
        for(int i = 0; i < _edge[x].size(); ++i) {
            if(visted[_edge[x][i]] == -1) {
                visted[_edge[x][i]] = visted[x] + 1;
                que.push(_edge[x][i]);
            }
        }
    }
}

int check(int n) {
    for(int i = 2; i <= n; ++i) {
        int has = 0;
        if(visted[i] == -1) {
            return 0;
        }
        for(int j = 0; j < _edge[i].size(); ++j) {
            if(visted[_edge[i][j]] == visted[i] - 1 || visted[_edge[i][j]] == visted[i]) {
                ++ has;
            }
        }
        if(has <= 1) {
            return 0;
        }
    }
    return 1;
}

int main(){
    int n, m;
    while(cin >> n >> m) {
        int x, y;
        for(int i = 0; i < m; ++i) {
            cin >> x >> y;
            _edge[x].push_back(y), _edge[y].push_back(x);
        }
        
        BFS(1);
        cout << check(n) << endl;
    }
    return 0;
}
