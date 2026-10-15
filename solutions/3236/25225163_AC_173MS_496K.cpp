// POJ 3236 - Michelle's Evaluation
// Model: gpt-6.1-sol
// A shortcut is an induced path, with length counted in vertices.
// Thus speedy graphs are P4-free cographs. Recursively split each
// induced graph into components, or into complement components.
// If neither split exists on at least two vertices, an induced P4 exists.
// Packed adjacency rows accelerate component searches in dense graphs.
#include <cstdio>
#include <vector>
using namespace std;
const int MAXN=1200, WORDS=38;
unsigned int adj[MAXN][WORDS];
int n, words;
int bitIndex(unsigned int x) {
    int r=0;
    if (!(x&65535U)) {r+=16; x>>=16;}
    if (!(x&255U)) {r+=8; x>>=8;}
    if (!(x&15U)) {r+=4; x>>=4;}
    if (!(x&3U)) {r+=2; x>>=2;}
    if (!(x&1U)) ++r;
    return r;
}
vector< vector<int> > components(const vector<int>& vertices, bool complement) {
    unsigned int unseen[WORDS]={0};
    for (unsigned int i=0;i<vertices.size();++i)
        unseen[vertices[i]/32] |= 1U<<(vertices[i]%32);
    vector< vector<int> > result;
    for (unsigned int i=0;i<vertices.size();++i) {
        int start=vertices[i];
        if (!(unseen[start/32] & (1U<<(start%32)))) continue;
        result.push_back(vector<int>());
        vector<int>& q=result.back();
        q.push_back(start);
        unseen[start/32] &= ~(1U<<(start%32));
        for (unsigned int head=0;head<q.size();++head) {
            int v=q[head];
            for (int w=0;w<words;++w) {
                unsigned int row=complement ? ~adj[v][w] : adj[v][w];
                unsigned int take=row & unseen[w];
                unseen[w] &= ~take;
                while (take) {
                    unsigned int low=take & (0U-take);
                    q.push_back(w*32+bitIndex(low));
                    take-=low;
                }
            }
        }
    }
    return result;
}
bool speedy(const vector<int>& vertices) {
    if (vertices.size()<4) return true;
    vector< vector<int> > parts=components(vertices,false);
    if (parts.size()==1) parts=components(vertices,true);
    if (parts.size()==1) return false;
    for (unsigned int i=0;i<parts.size();++i)
        if (!speedy(parts[i])) return false;
    return true;
}
int main() {
    int m;
    if (scanf("%d%d",&n,&m)!=2) return 0;
    words=(n+31)/32;
    for (int i=0;i<m;++i) {
        int a,b; scanf("%d%d",&a,&b); --a; --b;
        if (a==b) continue;
        adj[a][b/32] |= 1U<<(b%32);
        adj[b][a/32] |= 1U<<(a%32);
    }
    vector<int> vertices;
    for (int i=0;i<n;++i) vertices.push_back(i);
    puts(speedy(vertices)?"Yes":"No");
    return 0;
}
