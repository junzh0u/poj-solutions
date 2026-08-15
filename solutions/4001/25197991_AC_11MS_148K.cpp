// POJ 4001 - Xiangqi
// Model: claude-sonnet-5
//
// Board is 10x9 (rows 1..10, cols 1..9). Black has only its general; red has
// its general plus 1-6 chariots/cannons/horses, and has just delivered check
// to black's general. Determine whether it is checkmate: try every legal
// move black's general has (the four orthogonal palace moves, capturing a
// red piece if one sits there, plus the "flying general" capture of the red
// general when the file between them is clear -- included for robustness,
// though the problem's legality guarantee means the current position can
// never already have that alignment). For each candidate, remove any
// captured piece, then test whether the general's new square is attacked by
// any remaining red piece (chariot/cannon line, horse jump with leg
// blocking, or the enemy general via the flying-general rule). If some
// candidate leaves the general safe, it's not checkmate.
//
// Ambiguity: none in the statement itself; the board discussion mentions no
// real pitfalls beyond generic "recompute all attackers after simulating the
// move" bugs, which this implementation does by rebuilding the attack test
// from scratch for every candidate square.
#include <cstdio>
#include <cstring>
using namespace std;

struct Piece { char type; int r, c; };

static bool clearPath(int r1, int c1, int r2, int c2, bool occ[11][10]) {
    if (r1 == r2) {
        int lo = c1 < c2 ? c1 : c2, hi = c1 < c2 ? c2 : c1;
        for (int c = lo + 1; c < hi; c++) if (occ[r1][c]) return false;
        return true;
    } else if (c1 == c2) {
        int lo = r1 < r2 ? r1 : r2, hi = r1 < r2 ? r2 : r1;
        for (int r = lo + 1; r < hi; r++) if (occ[r][c1]) return false;
        return true;
    }
    return false;
}

static int countBetween(int r1, int c1, int r2, int c2, bool occ[11][10]) {
    int cnt = 0;
    if (r1 == r2) {
        int lo = c1 < c2 ? c1 : c2, hi = c1 < c2 ? c2 : c1;
        for (int c = lo + 1; c < hi; c++) if (occ[r1][c]) cnt++;
    } else if (c1 == c2) {
        int lo = r1 < r2 ? r1 : r2, hi = r1 < r2 ? r2 : r1;
        for (int r = lo + 1; r < hi; r++) if (occ[r][c1]) cnt++;
    }
    return cnt;
}

static bool isAttacked(int br, int bc, Piece pieces[], int cnt) {
    bool occ[11][10];
    memset(occ, 0, sizeof(occ));
    occ[br][bc] = true;
    for (int i = 0; i < cnt; i++) occ[pieces[i].r][pieces[i].c] = true;

    for (int i = 0; i < cnt; i++) {
        Piece &p = pieces[i];
        if (p.type == 'R') {
            if ((p.r == br || p.c == bc) && clearPath(p.r, p.c, br, bc, occ)) return true;
        } else if (p.type == 'C') {
            if ((p.r == br || p.c == bc) && countBetween(p.r, p.c, br, bc, occ) == 1) return true;
        } else if (p.type == 'H') {
            int dr = br - p.r, dc = bc - p.c;
            int legr = -1, legc = -1;
            if (dr == 2 && (dc == 1 || dc == -1)) { legr = p.r + 1; legc = p.c; }
            else if (dr == -2 && (dc == 1 || dc == -1)) { legr = p.r - 1; legc = p.c; }
            else if (dc == 2 && (dr == 1 || dr == -1)) { legr = p.r; legc = p.c + 1; }
            else if (dc == -2 && (dr == 1 || dr == -1)) { legr = p.r; legc = p.c - 1; }
            if (legr != -1 && !occ[legr][legc]) return true;
        } else if (p.type == 'G') {
            if (p.c == bc && clearPath(p.r, p.c, br, bc, occ)) return true;
        }
    }
    return false;
}

int main() {
    int N, bgr, bgc;
    while (scanf("%d %d %d", &N, &bgr, &bgc) == 3) {
        if (N == 0 && bgr == 0 && bgc == 0) break;
        Piece red[8];
        int redCount = 0;
        int genR = -1, genC = -1;
        for (int i = 0; i < N; i++) {
            char buf[8];
            int r, c;
            scanf("%s %d %d", buf, &r, &c);
            red[redCount].type = buf[0];
            red[redCount].r = r;
            red[redCount].c = c;
            if (buf[0] == 'G') { genR = r; genC = c; }
            redCount++;
        }

        int cr[8], cc[8], ncand = 0;
        int dr4[4] = {-1, 1, 0, 0}, dc4[4] = {0, 0, -1, 1};
        for (int k = 0; k < 4; k++) {
            int nr = bgr + dr4[k], nc = bgc + dc4[k];
            if (nr >= 1 && nr <= 3 && nc >= 4 && nc <= 6) { cr[ncand] = nr; cc[ncand] = nc; ncand++; }
        }
        {
            bool occ[11][10];
            memset(occ, 0, sizeof(occ));
            occ[bgr][bgc] = true;
            for (int i = 0; i < redCount; i++) occ[red[i].r][red[i].c] = true;
            if (genC == bgc && clearPath(bgr, bgc, genR, genC, occ)) {
                cr[ncand] = genR; cc[ncand] = genC; ncand++;
            }
        }

        bool escape = false;
        for (int m = 0; m < ncand && !escape; m++) {
            int nr = cr[m], nc = cc[m];
            Piece newPieces[8];
            int newCnt = 0;
            for (int i = 0; i < redCount; i++) {
                if (red[i].r == nr && red[i].c == nc) continue;
                newPieces[newCnt++] = red[i];
            }
            if (!isAttacked(nr, nc, newPieces, newCnt)) escape = true;
        }
        printf(escape ? "NO\n" : "YES\n");
    }
    return 0;
}
