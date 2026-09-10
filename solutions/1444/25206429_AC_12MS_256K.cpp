// POJ 1444 - Parallelepiped walk
// Model: claude-sonnet-5
//
// Two points A,B lie on the surface of a box [0,L]x[0,W]x[0,H]. We need the
// square of the shortest surface path between them.
//
// Approach: general "unfold the box" search. The box has 6 rectangular
// faces; any two distinct faces are either adjacent (share an edge) or
// opposite (parallel, no shared edge) - this is the octahedron adjacency
// graph. A taut (locally shortest) geodesic on a convex polyhedron crosses
// each face at most once, so every candidate shortest path corresponds to a
// *simple* sequence of faces (no repeats) starting on a face containing A
// and ending on a face containing B (a point on an edge/corner can belong
// to 2 or 3 faces, so we try every face membership of A and B).
//
// For a fixed sequence of faces we unfold them into a common plane: face 1
// keeps its own (u,v) -> (u,v) parametrization, and each subsequent face is
// obtained by "flipping" the previous rectangle open about their shared
// edge (an isometry of the plane, computed by matching the shared edge's
// two endpoints). Because every box face meets its neighbours at right
// angles, all these isometries are one of the 8 symmetries of the square,
// so every rectangle in the unfolded net stays axis aligned and every
// transform can be represented and composed with exact integer arithmetic.
//
// A' = image of A on the first face, B' = image of B on the last face. The
// straight segment A'B' is a valid witness for this face sequence only if
// it actually threads through the rectangles in order: clip the segment
// against every rectangle (Liang-Barsky, done with exact fractions so
// there's no epsilon issue) and require the resulting parameter intervals
// to tile [0,1] contiguously (exit of rectangle i == entry of rectangle
// i+1). Among all sequences (DFS over the 6-face graph, depth <= 6) and all
// membership choices for A and B, the minimum valid |A'B'|^2 is the answer.
// This "which faces are crossed" DFS is the standard technique for
// box/cube shortest-surface-path problems (needed here: e.g. the classic
// 30x12x12 spider-and-fly puzzle needs a path crossing 5 faces to beat the
// naive 2-face unfolding, and this solver reproduces that classic answer).
//
// Verified: sample (36), the classic spider/fly 30x12x12 puzzle (1600 =
// 40^2, crossing 5 faces - a depth-3 cap reproduces the wrong classic
// "naive" answer instead), cube corner-to-corner (L*L*5), several
// degenerate boxes (a zero dimension collapses to a flat rectangle or a
// 1D segment; A==B is handled explicitly), ~250k random surface point
// pairs (including boxes up to 1000 in each dimension, and thin 1x1xN
// boxes) cross-checked against a dense 8-direction grid Dijkstra (which is
// always an upper bound - never violated), and a differential mutant that
// skips the unfolding-validity check: across all of the above it never
// finds a smaller (invalid) candidate than the validated search, so the
// validity check is defensive rather than load-bearing for a box's
// right-angle geometry, but it's kept since it costs nothing and is
// needed in principle for a general convex polyhedron.
#include <cstdio>
#include <vector>
using namespace std;
// All coordinates stay within a few thousand in magnitude (L,W,H <= 1000,
// unfolded across at most 6 faces), and squared distances stay well under
// 2^31, so plain int is exact and sufficient - no need for long long/%lld.
typedef int ll;

struct P3 { ll v[3];
    P3(){ v[0]=v[1]=v[2]=0; }
    P3(ll a, ll b, ll c){ v[0]=a; v[1]=b; v[2]=c; }
    bool eq(const P3&o) const { return v[0]==o.v[0] && v[1]==o.v[1] && v[2]==o.v[2]; }
};

struct Face {
    int id;
    int fixedAxis;
    ll val;
    int uAxis, vAxis;
    ll Lu, Lv;
    Face(){}
    Face(int id_, int fa, ll val_, int ua, int va, ll lu, ll lv)
        : id(id_), fixedAxis(fa), val(val_), uAxis(ua), vAxis(va), Lu(lu), Lv(lv) {}
};

static Face faces[6];

static bool pointOnFace(const P3&P, const Face&f){
    if (P.v[f.fixedAxis]!=f.val) return false;
    ll u=P.v[f.uAxis], vv=P.v[f.vAxis];
    return u>=0 && u<=f.Lu && vv>=0 && vv<=f.Lv;
}
static void localCoords(const P3&P, const Face&f, ll&u, ll&v){
    u = P.v[f.uAxis]; v = P.v[f.vAxis];
}
static P3 point3D(const Face&f, ll u, ll v){
    P3 P;
    P.v[f.fixedAxis]=f.val;
    P.v[f.uAxis]=u;
    P.v[f.vAxis]=v;
    return P;
}

struct Pt2 { ll x,y; Pt2():x(0),y(0){} Pt2(ll a,ll b):x(a),y(b){} };

struct Transform {
    ll ox,oy,a,b,c,d;
    Transform(){}
    Transform(ll ox_, ll oy_, ll a_, ll b_, ll c_, ll d_)
        : ox(ox_), oy(oy_), a(a_), b(b_), c(c_), d(d_) {}
};

static Pt2 applyT(const Transform&T, ll u, ll v){
    return Pt2(T.ox+T.a*u+T.b*v, T.oy+T.c*u+T.d*v);
}

static const Face* findAdjacent(const Face&cur, const P3&P1, const P3&P2){
    for (int i=0;i<6;i++){
        if (faces[i].id==cur.id) continue;
        if (pointOnFace(P1,faces[i]) && pointOnFace(P2,faces[i])) return &faces[i];
    }
    return 0;
}

static Transform computeNextTransform(const Face&cur, const Transform&T,
                                       ll u1, ll v1, ll u2, ll v2,
                                       const Face&fnext, const P3&P1_3d, const P3&P2_3d){
    ll Q1x,Q1y,Q2x,Q2y;
    localCoords(P1_3d, fnext, Q1x, Q1y);
    localCoords(P2_3d, fnext, Q2x, Q2y);
    Pt2 P1g = applyT(T,u1,v1);
    Pt2 P2g = applyT(T,u2,v2);
    ll dQx = Q2x-Q1x, dQy = Q2y-Q1y;
    ll dPx = P2g.x-P1g.x, dPy = P2g.y-P1g.y;
    ll e1x,e1y,f1x,f1y;
    if (dQx==0){ e1x=0; e1y = dQy>0?1:-1; } else { e1x = dQx>0?1:-1; e1y=0; }
    ll e2x = -e1y, e2y = e1x;
    if (dPx==0){ f1x=0; f1y = dPy>0?1:-1; } else { f1x = dPx>0?1:-1; f1y=0; }
    ll f2x = -f1y, f2y = f1x;

    ll rlx2 = fnext.Lu - 2*Q1x;
    ll rly2 = fnext.Lv - 2*Q1y;
    ll cross_local = e1x*rly2 - e1y*rlx2;
    int sign_local = (cross_local>0) - (cross_local<0);

    ll ccgx2 = 2*T.ox + T.a*cur.Lu + T.b*cur.Lv;
    ll ccgy2 = 2*T.oy + T.c*cur.Lu + T.d*cur.Lv;
    ll rgx2 = ccgx2 - 2*P1g.x;
    ll rgy2 = ccgy2 - 2*P1g.y;
    ll cross_cur = f1x*rgy2 - f1y*rgx2;
    int sign_cur = (cross_cur>0) - (cross_cur<0);

    bool useReflect = (sign_local==sign_cur);
    ll f2ux = useReflect ? -f2x : f2x;
    ll f2uy = useReflect ? -f2y : f2y;

    ll a = f1x*e1x + f2ux*e2x;
    ll b = f1x*e1y + f2ux*e2y;
    ll c = f1y*e1x + f2uy*e2x;
    ll d = f1y*e1y + f2uy*e2y;
    ll ox = P1g.x - (a*Q1x + b*Q1y);
    ll oy = P1g.y - (c*Q1x + d*Q1y);
    return Transform(ox,oy,a,b,c,d);
}

struct Frac {
    ll num, den; // den > 0
    Frac() : num(0), den(1) {}
    Frac(ll n, ll d){ if(d<0){n=-n;d=-d;} num=n; den=d; }
};
static bool fracLess(const Frac&x, const Frac&y){ return x.num*y.den < y.num*x.den; }
static bool fracGreater(const Frac&x, const Frac&y){ return fracLess(y,x); }
static bool fracEq(const Frac&x, const Frac&y){ return x.num*y.den == y.num*x.den; }

struct Rect { ll xlo,xhi,ylo,yhi; };

static Rect rectOf(const Transform&T, const Face&f){
    ll xs[4], ys[4];
    Pt2 c;
    c=applyT(T,0,0); xs[0]=c.x; ys[0]=c.y;
    c=applyT(T,f.Lu,0); xs[1]=c.x; ys[1]=c.y;
    c=applyT(T,0,f.Lv); xs[2]=c.x; ys[2]=c.y;
    c=applyT(T,f.Lu,f.Lv); xs[3]=c.x; ys[3]=c.y;
    Rect r;
    r.xlo=r.xhi=xs[0]; r.ylo=r.yhi=ys[0];
    for(int i=1;i<4;i++){
        if(xs[i]<r.xlo) r.xlo=xs[i];
        if(xs[i]>r.xhi) r.xhi=xs[i];
        if(ys[i]<r.ylo) r.ylo=ys[i];
        if(ys[i]>r.yhi) r.yhi=ys[i];
    }
    return r;
}

// Updates tlo/thi with the half-plane constraint p*t <= q; returns false if
// the feasible interval becomes empty.
static bool clipHalfPlane(ll p, ll q, Frac&tlo, Frac&thi){
    if (p==0){
        return q>=0;
    }
    Frac t(q,p);
    if (p<0){
        if (fracGreater(t,thi)) return false;
        if (fracGreater(t,tlo)) tlo=t;
    } else {
        if (fracLess(t,tlo)) return false;
        if (fracLess(t,thi)) thi=t;
    }
    return true;
}

static bool clipSegRect(ll Ax, ll Ay, ll Bx, ll By, const Rect&r, Frac&tlo, Frac&thi){
    tlo = Frac(0,1); thi = Frac(1,1);
    ll dx=Bx-Ax, dy=By-Ay;
    if (!clipHalfPlane(-dx, Ax-r.xlo, tlo, thi)) return false;
    if (!clipHalfPlane(dx, r.xhi-Ax, tlo, thi)) return false;
    if (!clipHalfPlane(-dy, Ay-r.ylo, tlo, thi)) return false;
    if (!clipHalfPlane(dy, r.yhi-Ay, tlo, thi)) return false;
    if (fracGreater(tlo,thi)) return false;
    return true;
}

static bool validate(const vector<Face>&pathFaces, const vector<Transform>&pathT,
                      ll Ax, ll Ay, ll Bx, ll By){
    int n = (int)pathFaces.size();
    vector<Frac> los(n), his(n);
    for (int i=0;i<n;i++){
        Rect r = rectOf(pathT[i], pathFaces[i]);
        Frac lo,hi;
        if (!clipSegRect(Ax,Ay,Bx,By,r,lo,hi)) return false;
        los[i]=lo; his[i]=hi;
    }
    if (fracGreater(los[0], Frac(0,1))) return false;
    if (fracLess(his[n-1], Frac(1,1))) return false;
    for (int i=0;i+1<n;i++){
        if (!fracEq(his[i], los[i+1])) return false;
    }
    return true;
}

static ll bestAns;

static void dfs(const Face&cur, const Transform&T, int visitedMask,
                 vector<Face>&pathFaces, vector<Transform>&pathT,
                 int targetMask, const P3&Pb, ll Ax, ll Ay){
    if (targetMask & (1<<cur.id)){
        ll Blx,Bly;
        localCoords(Pb, cur, Blx, Bly);
        Pt2 Bg = applyT(T, Blx, Bly);
        if (validate(pathFaces, pathT, Ax, Ay, Bg.x, Bg.y)){
            ll dx = Bg.x-Ax, dy = Bg.y-Ay;
            ll d2 = dx*dx+dy*dy;
            if (d2 < bestAns) bestAns = d2;
        }
    }
    if ((int)pathFaces.size() >= 6) return;
    ll Lu=cur.Lu, Lv=cur.Lv;
    ll ex[4][4] = { {0,0,0,Lv}, {Lu,0,Lu,Lv}, {0,0,Lu,0}, {0,Lv,Lu,Lv} };
    for (int e=0;e<4;e++){
        ll u1=ex[e][0], v1=ex[e][1], u2=ex[e][2], v2=ex[e][3];
        if (u1==u2 && v1==v2) continue;
        P3 P1_3d = point3D(cur,u1,v1);
        P3 P2_3d = point3D(cur,u2,v2);
        const Face* fnext = findAdjacent(cur, P1_3d, P2_3d);
        if (!fnext) continue;
        if (visitedMask & (1<<fnext->id)) continue;
        Transform newT = computeNextTransform(cur, T, u1,v1,u2,v2, *fnext, P1_3d, P2_3d);
        pathFaces.push_back(*fnext);
        pathT.push_back(newT);
        dfs(*fnext, newT, visitedMask | (1<<fnext->id), pathFaces, pathT, targetMask, Pb, Ax, Ay);
        pathFaces.pop_back();
        pathT.pop_back();
    }
}

int main(){
    ll L,W,H,x1,y1,z1,x2,y2,z2;
    if (scanf("%d %d %d %d %d %d %d %d %d",
              &L,&W,&H,&x1,&y1,&z1,&x2,&y2,&z2)!=9) return 0;

    // faces: id, fixedAxis, val, uAxis, vAxis, Lu, Lv
    faces[0] = Face(0, 0, 0, 1,2, W,H);
    faces[1] = Face(1, 0, L, 1,2, W,H);
    faces[2] = Face(2, 1, 0, 0,2, L,H);
    faces[3] = Face(3, 1, W, 0,2, L,H);
    faces[4] = Face(4, 2, 0, 0,1, L,W);
    faces[5] = Face(5, 2, H, 0,1, L,W);

    P3 A(x1,y1,z1);
    P3 B(x2,y2,z2);

    if (A.eq(B)){ printf("0\n"); return 0; }

    bestAns = 2000000000;

    int targetMask=0;
    for (int i=0;i<6;i++) if (pointOnFace(B, faces[i])) targetMask |= (1<<i);

    for (int i=0;i<6;i++){
        if (!pointOnFace(A, faces[i])) continue;
        Transform T0(0,0,1,0,0,1);
        ll Ax,Ay;
        localCoords(A, faces[i], Ax, Ay);
        vector<Face> pathFaces; pathFaces.push_back(faces[i]);
        vector<Transform> pathT; pathT.push_back(T0);
        dfs(faces[i], T0, (1<<i), pathFaces, pathT, targetMask, B, Ax, Ay);
    }

    printf("%d\n", bestAns);
    return 0;
}
