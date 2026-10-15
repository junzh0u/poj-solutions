// POJ 1582 - Which Way Do I Go?
// Model: gpt-6.1-sol
// Dijkstra on cities, expanding every possible continuous ride on a road.
// Prefix sums give ride distance/time; each ride costs one for turns.
// Equal-cost direct rides retain the earliest input road, per the board hint.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <utility>
#include <algorithm>
using namespace std;
struct Road { string name; vector<int> city; vector<double> dist, time; };
int main() {
    int n; if (!(cin >> n)) return 0;
    vector<string> names(n); map<string,int> id;
    for (int i=0;i<n;++i) { cin >> names[i]; id[names[i]]=i; }
    int r; cin >> r; string line; getline(cin,line);
    vector<Road> roads(r);
    vector<vector<pair<int,int> > > at(n);
    for (int i=0;i<r;++i) {
        do { if (!getline(cin,line)) return 0; } while(line.empty());
        istringstream ss(line); string city; ss >> roads[i].name >> city;
        roads[i].city.push_back(id[city]); roads[i].dist.push_back(0); roads[i].time.push_back(0);
        double d,t;
        while(ss >> d >> t >> city) {
            roads[i].city.push_back(id[city]);
            roads[i].dist.push_back(roads[i].dist.back()+d);
            roads[i].time.push_back(roads[i].time.back()+t);
        }
        for(int j=0;j<(int)roads[i].city.size();++j)
            at[roads[i].city[j]].push_back(make_pair(i,j));
    }
    string kind,from,to;
    while(cin >> kind >> from >> to) {
        int s=id[from], target=id[to];
        vector<double> best(n,1e100); vector<int> prev(n,-1), via(n,-1), used(n,0);
        best[s]=0;
        for(int step=0;step<n;++step) {
            int u=-1;
            for(int i=0;i<n;++i) if(!used[i] && (u<0 || best[i]<best[u])) u=i;
            if(u<0 || best[u]==1e100 || u==target) break;
            used[u]=1;
            for(int z=0;z<(int)at[u].size();++z) {
                int ri=at[u][z].first, p=at[u][z].second; Road &road=roads[ri];
                for(int j=0;j<(int)road.city.size();++j) {
                    int v=road.city[j]; if(v==u || used[v]) continue;
                    double w=1;
                    if(kind=="distance") w=road.dist[j]-road.dist[p];
                    if(kind=="time") w=road.time[j]-road.time[p];
                    if(w<0) w=-w;
                    if(best[u]+w < best[v]-1e-9) {
                        best[v]=best[u]+w; prev[v]=u; via[v]=ri;
                    }
                }
            }
        }
        cout << "from " << from << '\n';
        vector<int> path;
        for(int v=target;v!=s && v>=0;v=prev[v]) path.push_back(v);
        reverse(path.begin(),path.end());
        for(int j=0;j<(int)path.size();++j)
            if(j+1==(int)path.size() || via[path[j]]!=via[path[j+1]])
                cout << roads[via[path[j]]].name << " to " << names[path[j]] << '\n';
    }
    return 0;
}
