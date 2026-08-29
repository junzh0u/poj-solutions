// POJ 3739 - Special Squares
// Model: claude-sonnet-5
//
// Approach: A special square is a choice of two x-lines (xL<xR) and two
// y-lines (yL<yR) with xR-xL == yR-yL == d > 0 such that the closed box
// [xL,xR] x [yL,yR] contains at least one point.
//
// Key fact: for a fixed lower-left corner (xL,yL), the box [xL,xL+d] x
// [yL,yL+d] grows monotonically (in the containment sense) as d grows, so
// there is a threshold D(xL,yL) = min over points p with px>=xL,py>=yL of
// max(px-xL, py-yL) such that the box contains a point iff d >= D(xL,yL).
// D can be computed for the whole coordinate grid in O(V^2) via a DP:
//   D(x,y) = min( D(x+1,y+1)+1,               // point strictly beyond both
//                 minPy[x](>=y) - y,           // point in same column x
//                 minPx[y](>=x) - x )          // point in same row y
//
// The answer is then
//   sum_{xL,yL} freqX[xL]*freqY[yL] * sum_{d>=D(xL,yL)} freqX[xL+d]*freqY[yL+d]
// The inner sum only depends on the diagonal c = yL-xL, so for each diagonal
// we build one suffix-sum array over k=xL+d of freqX[k]*freqY[k+c], and use
// it for every xL on that diagonal.  Overall O(V^2).
//
// Statement ambiguity: the discuss board (POJ 3739) reports the stated
// coordinate bound 0<=coord<=1000 is violated by the actual judge data --
// multiple posters needed arrays sized to 1500-2000 to avoid RE/WA ("開到
// 1500才過", "coordinate maximum 1000 RE, 2000 AC"). This solution reads
// all input first and sizes every array dynamically from the true maximum
// coordinate seen, so it is correct regardless of the real bound.
//
// Lines with an identical coordinate are treated as distinct combinatorial
// choices (standard "choose 2 of n given lines" reading), matching the
// sample: with lines at {0,2,4,6} on both axes and points (1,1),(3,3),(6,6)
// this yields 8, as required.

#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

int main(){
    int n1,n2,n3;
    if(scanf("%d %d %d",&n1,&n2,&n3)!=3) return 0;
    vector<int> ys(n1), xs(n2), px(n3), py(n3);
    int maxCoord = 0;
    for(int i=0;i<n1;i++){ scanf("%d",&ys[i]); maxCoord=max(maxCoord,ys[i]); }
    for(int i=0;i<n2;i++){ scanf("%d",&xs[i]); maxCoord=max(maxCoord,xs[i]); }
    for(int i=0;i<n3;i++){ scanf("%d %d",&px[i],&py[i]); maxCoord=max(maxCoord,max(px[i],py[i])); }

    int N = maxCoord + 1; // coordinates 0..maxCoord

    vector<long long> freqX(N,0), freqY(N,0);
    for(int i=0;i<n2;i++) freqX[xs[i]]++;
    for(int i=0;i<n1;i++) freqY[ys[i]]++;

    // Bit-packed point grid to keep memory small even if the true
    // coordinate bound turns out to be well past what the statement claims.
    vector<bool> hasPoint((long long)N*N, false);
    for(int i=0;i<n3;i++) hasPoint[(long long)px[i]*N+py[i]] = true;

    const int INF = 30000; // safely above any possible coordinate difference
    // D[x][y] for x,y in [0,N], where index N is the sentinel (=INF, never
    // written) representing "out of range". Stored as short to halve memory.
    vector<short> D((long long)(N+1)*(N+1), (short)INF);
    // helper to index D
    long long Nd = N+1;

    vector<int> rowMin(N, INF);
    for(int x=N-1;x>=0;x--){
        int colBest = INF;
        for(int y=N-1;y>=0;y--){
            bool hp = hasPoint[(long long)x*N+y];
            if(hp) colBest = min(colBest, y);
            int rowCandidate = rowMin[y];
            if(hp) rowCandidate = min(rowCandidate, x);
            int colTerm = (colBest<INF) ? (colBest-y) : INF;
            int rowTerm = (rowCandidate<INF) ? (rowCandidate-x) : INF;
            int dxy = D[(long long)(x+1)*Nd + (y+1)];
            int diagTerm = (dxy<INF) ? (dxy+1) : INF;
            int best = min(colTerm, min(rowTerm, diagTerm));
            D[(long long)x*Nd + y] = (short)best;
            if(hp) rowMin[y] = min(rowMin[y], x);
        }
    }

    vector<long long> Suf(N+2, 0);
    long long answer = 0;
    for(int c = -(N-1); c <= (N-1); c++){
        Suf[N] = 0;
        for(int k=N-1;k>=0;k--){
            int j = k+c;
            long long z = 0;
            if(j>=0 && j<N) z = freqX[k]*freqY[j];
            Suf[k] = Suf[k+1] + z;
        }
        for(int xL=0; xL<N; xL++){
            int yL = xL + c;
            if(yL<0 || yL>=N) continue;
            if(freqX[xL]==0 || freqY[yL]==0) continue;
            int d0 = D[(long long)xL*Nd + yL];
            if(d0 < 1) d0 = 1; // d=0 is not a square (degenerate, zero side length)
            long long contribution = 0;
            if(d0<INF){
                long long k0 = (long long)xL + d0;
                if(k0 < N) contribution = Suf[(int)k0];
            }
            answer += freqX[xL]*freqY[yL]*contribution;
        }
    }

    printf("%lld\n", answer);
    return 0;
}
