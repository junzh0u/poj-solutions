// POJ 1612 - The Geodetic Set Problem
// Model: claude-sonnet-5
//
// For an unweighted connected graph with n<=40 vertices, precompute all-pairs
// shortest distances with BFS from every vertex (n is tiny). A vertex w lies
// on some shortest u-v path (i.e. w is in I(u,v)) iff
// dist(u,w) + dist(w,v) == dist(u,v). For each query set D, I(D) is the
// union of I(u,v) over all (ordered) pairs u,v in D (including u==v, which
// trivially just marks u itself). D is a geodetic set iff I(D) covers all n
// vertices.
//
// Input framing: the n adjacency lines and each query line list a variable
// number of integers, so they must be parsed per physical line (getline +
// istringstream) rather than with a fixed-count cin>>, since nothing states
// how many neighbours/query vertices a line holds. The board (msg 121338)
// only notes neighbours are separated by "at least one space" (whitespace
// tokenizing handles that) and (msg 353474) that the judge data is weak
// enough that stringstream parsing runs in 0ms, so no special performance
// care is needed beyond the natural O(n^3) per query bound (n<=40).
//
// The image referenced between the adjacency-list description and the
// "number of test cases" line is just the same Figure 3 example graph
// reused by the HTML (checked images/1612_1.jpg) - not a hidden constraint.

#include <cstdio>
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <iostream>
using namespace std;

int n;
bool adjm[41][41];
int distm[41][41];

int main(){
    string line;
    if(!getline(cin,line)) return 0;
    {
        istringstream iss(line);
        iss >> n;
    }
    for(int i=1;i<=n;i++){
        if(!getline(cin,line)) line="";
        istringstream iss(line);
        int x;
        while(iss>>x){
            adjm[i][x]=true;
            adjm[x][i]=true;
        }
    }

    memset(distm,-1,sizeof(distm));
    for(int s=1;s<=n;s++){
        distm[s][s]=0;
        queue<int> q;
        q.push(s);
        while(!q.empty()){
            int u=q.front(); q.pop();
            for(int v=1;v<=n;v++){
                if(adjm[u][v] && distm[s][v]==-1){
                    distm[s][v]=distm[s][u]+1;
                    q.push(v);
                }
            }
        }
    }

    int k=0;
    if(getline(cin,line)){
        istringstream iss(line);
        iss>>k;
    }

    for(int t=0;t<k;t++){
        if(!getline(cin,line)) line="";
        istringstream iss(line);
        int x;
        vector<int> D;
        while(iss>>x) D.push_back(x);

        bool covered[41];
        memset(covered,0,sizeof(covered));
        for(size_t i=0;i<D.size();i++) covered[D[i]]=true;

        for(size_t i=0;i<D.size();i++){
            for(size_t j=0;j<D.size();j++){
                int u=D[i], v=D[j];
                int duv = distm[u][v];
                for(int w=1;w<=n;w++){
                    if(distm[u][w]+distm[w][v]==duv) covered[w]=true;
                }
            }
        }

        bool ok=true;
        for(int i=1;i<=n;i++){
            if(!covered[i]){ ok=false; break; }
        }
        printf(ok? "yes\n" : "no\n");
    }

    return 0;
}
