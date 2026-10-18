// POJ 2968 - Necklace
// Model: gpt-6-astra
// Represent each radius by two potentials p_i and q_i. Encode each
// sum bound in both symmetric orientations as difference constraints.
// Bellman-Ford finds feasible potentials; (p_i-q_i)/2 satisfies every
// original bound by adding its two inequalities. Nonnegative radii use
// q_i <= p_i. Nonadjacent tangency is allowed by the stated judge tolerance.
#include <cstdio>
#include <cmath>
#include <vector>
using namespace std;
struct Edge {
    int u, v;
    double w;
    Edge(int a, int b, double c): u(a), v(b), w(c) {}
};
int main() {
    int n;
    if (scanf("%d", &n) != 1) return 0;
    double x[100], y[100], d[200] = {0};
    for (int i=0; i<n; ++i) scanf("%lf%lf", &x[i], &y[i]);
    vector<Edge> edges;
    for (int i=0; i<n; ++i) {
        edges.push_back(Edge(i, i+n, 0));
        for (int j=i+1; j<n; ++j) {
            double dx=x[i]-x[j], dy=y[i]-y[j];
            double dist=sqrt(dx*dx+dy*dy);
            if (j==i+1 || (i==0 && j==n-1)) {
                edges.push_back(Edge(i, j+n, -dist));
                edges.push_back(Edge(j, i+n, -dist));
            } else {
                edges.push_back(Edge(i+n, j, dist));
                edges.push_back(Edge(j+n, i, dist));
            }
        }
    }
    for (int pass=0; pass<2*n; ++pass) {
        bool changed=false;
        for (size_t k=0; k<edges.size(); ++k) {
            const Edge &e=edges[k];
            if (d[e.v] > d[e.u]+e.w+1e-10) {
                d[e.v]=d[e.u]+e.w;
                changed=true;
            }
        }
        if (!changed) break;
    }
    for (int i=0; i<n; ++i) {
        double r=(d[i]-d[i+n])*0.5;
        if (r<0) r=0;
        printf("%.10f\n", r);
    }
    return 0;
}
