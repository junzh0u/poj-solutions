// POJ 1133 - Stars
// Model: claude-sonnet-5
//
// For each constellation, pick two points p[a], p[b] with p[a] != p[b] (the
// first two distinct points found). For every ordered pair (A, B) of distinct
// map stars, requiring "map p[a]->A, p[b]->B via a rotation+scaling" fixes a
// unique complex multiplier w = (B-A)/(p[b]-p[a]). All other constellation
// points are transformed through this affine map using exact integer/
// rational arithmetic (no floating point) and looked up in a coordinate ->
// star index map. If every point lands on a real star, this is a valid
// occurrence.
//
// Distinct (A,B) pairs can yield the very same set of stars when the
// constellation is self-symmetric under an orientation-preserving rotation -
// this is *always* true for 2-point constellations, since swapping which
// point plays role 0 vs role 1 is realized by the 180-degree rotation about
// the midpoint. The number of (A,B) pairs landing on one given occurrence
// equals the size of the orbit of the anchor pair (a,b) under the
// constellation's own rotation-automorphism group; that size, K, is computed
// once per constellation by running the identical matching procedure with
// the constellation matched against itself (target = its own point set), and
// the raw successful-pair count is then divided by K to recover the true
// occurrence count (harmless when the shape has no symmetry, where K is
// always 1). This avoids an expensive per-match dedup set, which made a
// highly symmetric small constellation (e.g. any 2-point one, up to C(n,2)
// matches) slow. Falls back to an explicit dedup-by-star-set only when the
// constellation has duplicate points or is too large for the O(s^3)
// self-check to be cheap, since successful matches are rare in that regime
// anyway (a big, generic shape is unlikely to match much or at all).
//
// Enumeration order is A from 0..n-1 (outer), B from 0..n-1 (inner), matching
// input order; ties in brightness keep the first-found occurrence under this
// order, since the judge carries no special judge and this is the natural
// enumeration a straightforward solution performs.
#include <cstdio>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

static const int SELF_CHECK_LIMIT = 200; // cap on s for the O(s^3) K computation

int main(){
    int n;
    int mapNum = 0;
    while (scanf("%d", &n) == 1 && n != 0) {
        ++mapNum;
        vector<long long> sx(n), sy(n), sb(n);
        map<pair<long long,long long>, int> coordIdx;
        for (int i = 0; i < n; ++i) {
            long long x,y,b;
            scanf("%lld %lld %lld", &x, &y, &b);
            sx[i]=x; sy[i]=y; sb[i]=b;
            coordIdx[make_pair(x,y)] = i;
        }
        int m;
        scanf("%d", &m);
        printf("Map #%d\n", mapNum);
        for (int c = 0; c < m; ++c) {
            int s;
            char nameBuf[64];
            scanf("%d %s", &s, nameBuf);
            vector<long long> qx(s), qy(s);
            for (int i = 0; i < s; ++i) {
                scanf("%lld %lld", &qx[i], &qy[i]);
            }
            printf("\n%s occurs ", nameBuf);

            int a = 0, b = -1;
            for (int i = 1; i < s; ++i) {
                if (qx[i]!=qx[0] || qy[i]!=qy[0]) { b=i; break; }
            }

            long long count = 0;
            long long bestBrightness = -1;
            vector<pair<long long,long long> > bestPts;

            if (s == 1) {
                count = n;
                long long bb=-1, bx=0, by=0;
                for (int i=0;i<n;++i) if (sb[i]>bb){bb=sb[i]; bx=sx[i]; by=sy[i];}
                if (n>0){ bestBrightness=bb; bestPts.push_back(make_pair(bx,by)); }
            } else if (b == -1) {
                count = 0;
            } else {
                long long vx = qx[b]-qx[a], vy = qy[b]-qy[a];
                long long D = vx*vx+vy*vy;

                // Check constellation points are pairwise distinct, and
                // decide whether the cheap K-based counting applies.
                map<pair<long long,long long>, int> qCoordIdx;
                bool qDistinct = true;
                for (int i = 0; i < s; ++i) {
                    pair<long long,long long> key(qx[i], qy[i]);
                    if (qCoordIdx.find(key) != qCoordIdx.end()) { qDistinct = false; break; }
                    qCoordIdx[key] = i;
                }
                bool useK = qDistinct && s <= SELF_CHECK_LIMIT;
                long long K = 1;
                if (useK) {
                    K = 0;
                    for (int a2 = 0; a2 < s; ++a2) {
                        for (int b2 = 0; b2 < s; ++b2) {
                            if (a2 == b2) continue;
                            long long vx2 = qx[b2]-qx[a2], vy2 = qy[b2]-qy[a2];
                            long long Wr = vx2*vx+vy2*vy; // (q[b2]-q[a2]) * conj(v)
                            long long Wi = vy2*vx-vx2*vy;
                            bool ok = true;
                            vector<int> used(s, 0);
                            used[a2] = 1; used[b2] = 1;
                            for (int k = 0; k < s && ok; ++k) {
                                if (k == a || k == b) continue;
                                long long ddx = qx[k]-qx[a], ddy = qy[k]-qy[a];
                                long long numX = Wr*ddx - Wi*ddy;
                                long long numY = Wr*ddy + Wi*ddx;
                                if (numX % D != 0 || numY % D != 0) { ok=false; break; }
                                long long ix = qx[a2] + numX/D;
                                long long iy = qy[a2] + numY/D;
                                map<pair<long long,long long>,int>::iterator it = qCoordIdx.find(make_pair(ix,iy));
                                if (it == qCoordIdx.end()) { ok=false; break; }
                                if (used[it->second]) { ok=false; break; }
                                used[it->second] = 1;
                            }
                            if (ok) ++K;
                        }
                    }
                    if (K < 1) K = 1;
                }

                set<vector<int> > seen; // only used when !useK
                vector<int> assign(s);
                long long rawCount = 0;
                for (int A=0;A<n;++A){
                    for (int B=0;B<n;++B){
                        if (A==B) continue;
                        long long dx = sx[B]-sx[A], dy = sy[B]-sy[A];
                        long long Wr = dx*vx+dy*vy;
                        long long Wi = dy*vx-dx*vy;
                        bool ok = true;
                        assign[a]=A; assign[b]=B;
                        for (int k=0;k<s && ok;++k){
                            if (k==a || k==b) continue;
                            long long ddx = qx[k]-qx[a], ddy = qy[k]-qy[a];
                            long long numX = Wr*ddx - Wi*ddy;
                            long long numY = Wr*ddy + Wi*ddx;
                            if (numX % D != 0 || numY % D != 0) { ok=false; break; }
                            long long ix = sx[A] + numX/D;
                            long long iy = sy[A] + numY/D;
                            map<pair<long long,long long>,int>::iterator it = coordIdx.find(make_pair(ix,iy));
                            if (it == coordIdx.end()) { ok=false; break; }
                            assign[k] = it->second;
                        }
                        if (!ok) continue;

                        if (useK) {
                            ++rawCount;
                            long long bright=0;
                            for (int i=0;i<s;++i) bright += sb[assign[i]];
                            if (bright > bestBrightness) {
                                bestBrightness = bright;
                                bestPts.clear();
                                for (int i=0;i<s;++i) bestPts.push_back(make_pair(sx[assign[i]], sy[assign[i]]));
                            }
                        } else {
                            vector<int> key = assign;
                            sort(key.begin(), key.end());
                            bool distinct=true;
                            for (int i=1;i<s;++i) if (key[i]==key[i-1]) { distinct=false; break; }
                            if (!distinct) continue;
                            if (seen.insert(key).second) {
                                ++count;
                                long long bright=0;
                                for (int i=0;i<s;++i) bright += sb[assign[i]];
                                if (bright > bestBrightness) {
                                    bestBrightness = bright;
                                    bestPts.clear();
                                    for (int i=0;i<s;++i) bestPts.push_back(make_pair(sx[assign[i]], sy[assign[i]]));
                                }
                            }
                        }
                    }
                }
                if (useK) count = rawCount / K;
            }

            printf("%lld time(s) in the map.\n", count);
            if (count > 0) {
                sort(bestPts.begin(), bestPts.end());
                printf("Brightest occurrence:");
                for (size_t i=0;i<bestPts.size();++i) {
                    printf(" (%lld,%lld)", bestPts[i].first, bestPts[i].second);
                }
                printf("\n");
            }
        }
        printf("-----\n");
    }
    return 0;
}
