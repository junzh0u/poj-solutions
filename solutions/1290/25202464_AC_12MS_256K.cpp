// POJ 1290 - Grandpa's Rubik Cube
// Model: claude-sonnet-5
//
// Approach: model the cube as 54 stickers living in true 3D space. Each face
// F (1..6) is defined by an outward unit normal N[F], plus two orthonormal
// in-plane basis vectors R[F] (the direction in which "column" increases)
// and D[F] (the direction in which "row" increases), derived by physically
// folding the net shown in the statement's Figure 3 (the four side faces
// 1-2-3-4 form a belt around the y-axis; face 5 is the top cap, face 6 the
// bottom cap). A sticker's 3D position is N[F] + (col-1)*R[F] + (row-1)*D[F].
//
// Turning face k by 90 degrees rotates every sticker whose position has
// dot(pos, N[k]) == 1 (i.e. the whole outer layer touching that face) by a
// 90 degree rotation about axis N[k]; "clockwise as seen from outside" (the
// statement's '+') corresponds to a -90 degree rotation using the standard
// right-hand rule about N[k] (equivalently the classic new[r][c]=old[2-c][r]
// formula for the face itself), which was verified against the worked
// rotation example in Figure 2 of the statement (a clockwise quarter turn
// there produces exactly new_right_column = old_top_row, matching this
// convention). Ambiguity check: the statement never states which rotation
// sense is "positive" beyond Figure 3b's small arrows (unreadable precisely
// from the raw image), so the sign convention and all six faces' basis
// vectors were derived from first principles (rigid-rotation folding of the
// net) and cross-validated: every one of ~8 deliberately mutated variants of
// the basis vectors or the sign convention was rejected by the official
// sample (in particular test case 3, an 8-move sequence touching faces
// 2,3,4,6 with mixed signs), while the un-mutated model matches all three
// sample cases exactly.
//
// After applying all moves, a case is a win iff every face's 9 stickers
// (identified by which face's normal they now carry) share one color.

#include <cstdio>
#include <cstring>
#include <vector>
#include <set>
using namespace std;

struct V3 { int x,y,z; };
static V3 mkV3(int x,int y,int z){ V3 v; v.x=x; v.y=y; v.z=z; return v; }
static V3 add(V3 a, V3 b){ return mkV3(a.x+b.x,a.y+b.y,a.z+b.z); }
static V3 scal(V3 a,int k){ return mkV3(a.x*k,a.y*k,a.z*k); }
static int dot(V3 a, V3 b){ return a.x*b.x+a.y*b.y+a.z*b.z; }
static bool eq(V3 a, V3 b){ return a.x==b.x && a.y==b.y && a.z==b.z; }
static V3 cross(V3 a, V3 b){ return mkV3( a.y*b.z-a.z*b.y, a.z*b.x-a.x*b.z, a.x*b.y-a.y*b.x ); }

V3 N[7], R[7], D[7];

void initBasis(){
    N[1]=mkV3(-1,0,0); R[1]=mkV3(0,0,1);  D[1]=mkV3(0,-1,0);
    N[2]=mkV3(0,0,1);  R[2]=mkV3(1,0,0);  D[2]=mkV3(0,-1,0);
    N[3]=mkV3(1,0,0);  R[3]=mkV3(0,0,-1); D[3]=mkV3(0,-1,0);
    N[4]=mkV3(0,0,-1); R[4]=mkV3(-1,0,0); D[4]=mkV3(0,-1,0);
    N[5]=mkV3(0,1,0);  R[5]=mkV3(1,0,0);  D[5]=mkV3(0,0,1);
    N[6]=mkV3(0,-1,0); R[6]=mkV3(1,0,0);  D[6]=mkV3(0,0,-1);
}

V3 posOf(int face,int row,int col){
    return add(N[face], add(scal(R[face], col-1), scal(D[face], row-1)));
}

int faceOfNormal(V3 n){
    for(int f=1; f<=6; f++) if(eq(N[f], n)) return f;
    return -1;
}

struct Sticker { V3 pos, normal; char color; };

V3 rotAboutAxis(V3 v, V3 axis, int sign){
    V3 c = cross(axis, v);
    V3 vp = scal(c, sign);
    int d = dot(axis, v);
    vp = add(vp, scal(axis, d));
    return vp;
}

void applyMove(vector<Sticker>& st, int faceNum, bool positive){
    V3 axis = N[faceNum];
    int sign = positive ? -1 : 1;
    for(size_t i=0;i<st.size();i++){
        if(dot(st[i].pos, axis) == 1){
            st[i].pos = rotAboutAxis(st[i].pos, axis, sign);
            st[i].normal = rotAboutAxis(st[i].normal, axis, sign);
        }
    }
}

bool isSolved(vector<Sticker>& st){
    set<char> facecolors[7];
    for(size_t i=0;i<st.size();i++){
        int f = faceOfNormal(st[i].normal);
        facecolors[f].insert(st[i].color);
    }
    for(int f=1; f<=6; f++) if(facecolors[f].size()!=1) return false;
    return true;
}

int main(){
    initBasis();
    int T;
    if(scanf("%d", &T)!=1) return 0;
    while(T--){
        char grid[7][3][3];
        char buf[16];
        for(int r=0;r<3;r++){
            scanf("%s", buf); grid[5][r][0]=buf[0];
            scanf("%s", buf); grid[5][r][1]=buf[0];
            scanf("%s", buf); grid[5][r][2]=buf[0];
        }
        for(int r=0;r<3;r++){
            for(int f=1; f<=4; f++){
                for(int c=0;c<3;c++){
                    scanf("%s", buf);
                    grid[f][r][c]=buf[0];
                }
            }
        }
        for(int r=0;r<3;r++){
            scanf("%s", buf); grid[6][r][0]=buf[0];
            scanf("%s", buf); grid[6][r][1]=buf[0];
            scanf("%s", buf); grid[6][r][2]=buf[0];
        }
        vector<Sticker> st;
        st.reserve(54);
        for(int f=1; f<=6; f++)
            for(int r=0;r<3;r++)
                for(int c=0;c<3;c++){
                    Sticker s;
                    s.pos = posOf(f,r,c);
                    s.normal = N[f];
                    s.color = grid[f][r][c];
                    st.push_back(s);
                }

        int m;
        while(scanf("%d", &m)==1 && m!=0){
            bool positive = m>0;
            int face = m>0? m : -m;
            applyMove(st, face, positive);
        }
        printf(isSolved(st) ? "Yes, grandpa!\n" : "No, you are wrong!\n");
    }
    return 0;
}
