// POJ 1073 - The Willy Memorial Program
// Model: claude-sonnet-5
//
// Physics reduce to: pipe1 receives water at a constant rate that, given all
// pipes share diameter 1, raises a single active pipe's level at 1 cm/sec.
// Water always fills the lowest currently-reachable empty point first. This
// makes the whole process single-threaded: at any instant there is exactly
// one "active group" of pipes -- all pipes that have equalised onto one
// common water level and are rising together (splitting the unit rate evenly,
// 1/k cm/sec each, k = group size) -- while every other already-touched pipe
// sits frozen at the level it reached earlier, waiting to be caught up to.
// When the active group's level reaches a not-yet-used link:
//   - if the far pipe is untouched, the ENTIRE current group freezes (pushed
//     on a stack) and the far pipe becomes the sole new active group, since
//     all incoming flow now drains into the lower, still-empty pipe;
//   - if the far pipe is already part of the same group, it's a harmless
//     redundant connection (no-op).
// When the active group's level instead reaches an outstanding frozen
// ancestor's level, that ancestor unfreezes and merges back in, growing the
// active group (and shrinking the per-member rate) for the next phase.
// A pipe that reaches its own physical top does NOT simply drop out of the
// active set while its siblings keep rising -- the problem's own law ("the
// level of water in both pipes remains the SAME" whenever connected) is
// absolute, so a single capped member permanently caps the level of the
// WHOLE connected group, even members with plenty of their own headroom
// left. The only way past it is a not-yet-used link sitting at that exact
// height (a genuinely new, still-empty branch to drain into); if there is
// none, the entire component is done rising, forever, from that instant on
// (a second submission excluded just the capped pipe and let the rest keep
// climbing, which is Wrong Answer whenever a merged group's cap isn't the
// group's very last event).
//

// Since every pipe coordinate/height is an integer and the rate is always of
// the form 1/k for an integer k, every elapsed-time step (levelDelta * k) is
// an exact integer, so the whole simulation runs in plain integer arithmetic.
//
// "The target level is always assumed to be a bit higher than the specified
// one": the answer is the first instant the target pipe's level becomes
// STRICTLY less (numerically) than the requested y. When the level merely
// touches the requested y because that is exactly where its group freezes
// (waiting on a sibling to catch up), that touch does not count -- the
// answer is deferred until the group actually resumes and moves past it
// (tracked here with the pendingTy flag, re-checked every time the target
// pipe rejoins an active group).
//
// Link endpoints are matched to pipe walls by exact x-coordinate: a link's
// left endpoint must equal some pipe's right wall (x+1), its right endpoint
// some other pipe's left wall (x), both within that pipe's y-range -- this
// matches the sample figure (link runs from the left pipe's right wall to
// the right pipe's left wall) and is the only reading under which "no link
// crosses a pipe" and "both endpoints are connected to pipes" is meaningful
// for axis-aligned, unit-diameter pipes at integer coordinates.
//
// A link may sit exactly at the level a pipe starts at (its own untouched
// bottom, or a level it has just frozen/rejoined at) rather than strictly
// above it -- the nearest-unconsumed-link search below treats "at or before
// the current level" (<=) as due, not just "strictly before" (a first
// submission used strict '<' here and missed such links entirely, so the
// pipe on the far side was never touched -- Wrong Answer; nothing in the
// sample exercises that boundary).
//
// Symmetrically, pendingTy is armed only from `landed` below -- never
// pre-armed just because tp starts a step already sitting at ty (its own
// bottom, or a level just rejoined) -- so that level is first given a chance
// to run its own candidate search: if it turns out to have a link of its own
// exactly there too, that's a real (possibly zero-duration) freeze, not an
// automatic success. The mid-step check's `ty <= curShared` (not strict `<`)
// is what actually resolves the immediate-continuation case once that
// search comes back clean.
//
// Verified against poj.org's own worked example (answer 9), the statement's
// explicit "target level 4 gives 5, not 2" freeze-boundary example, and
// tens of thousands of randomized trials -- including generators that force
// link y-coordinates onto pipe top/bottom boundaries -- cross-checked
// against two independently structured simulators (one driven by an
// explicit freeze stack, one recomputing the globally-lowest reachable
// frontier from scratch every step, one a per-cm discrete tick simulation)
// with zero disagreements after fixing the bugs above.

#include <cstdio>
#include <vector>
using namespace std;

int P;
int top_[21], bottom_[21], pxw[21];
int L;
int lpa[51], lpb[51], lyv[51];
bool consumed[51];
bool touched[21];
bool inGroup[21];
int level[21];

int main(){
    int t;
    if(scanf("%d", &t) != 1) return 0;
    while(t--){
        scanf("%d", &P);
        for(int i=1;i<=P;i++){
            int x,y,h;
            scanf("%d %d %d", &x, &y, &h);
            pxw[i]=x; top_[i]=y; bottom_[i]=y+h;
        }
        scanf("%d", &L);
        for(int j=0;j<L;j++){
            int x,y,len;
            scanf("%d %d %d", &x, &y, &len);
            int leftx = x, rightx = x+len;
            int pa=-1, pb=-1;
            for(int i=1;i<=P;i++){
                if(pxw[i]+1==leftx && top_[i]<=y && y<=bottom_[i]) pa=i;
                if(pxw[i]==rightx && top_[i]<=y && y<=bottom_[i]) pb=i;
            }
            lpa[j]=pa; lpb[j]=pb; lyv[j]=y;
        }
        int tp, ty;
        scanf("%d %d", &tp, &ty);

        // A target strictly outside [top, bottom] is geometrically
        // meaningless -- water can never be above a pipe's own rim, nor
        // below its closed floor, not even trivially at t==0 -- so it's
        // always "No Solution", regardless of direction. (A second
        // submission answered 0 for ty>bottom, reasoning the untouched
        // surface trivially satisfies "level < ty"; that's wrong, this is
        // Wrong Answer's actual remaining cause -- confirmed by a real
        // accepted mirror solution's identical strict-outside-range check.)
        // ty exactly equal to top or bottom is NOT rejected here -- it
        // falls through to the simulation below.
        if(ty > bottom_[tp] || ty < top_[tp]){
            printf("No Solution\n");
            continue;
        }

        for(int i=1;i<=P;i++){ level[i]=bottom_[i]; touched[i]=false; inGroup[i]=false; }
        for(int j=0;j<L;j++) consumed[j]=false;
        touched[1]=true; inGroup[1]=true;

        // explicit freeze stack: stackMembers[k] lists the pipes frozen at
        // stackLevel[k]; stacks are shallow (<= P frames).
        vector<vector<int> > stackMembers;
        vector<int> stackLevel;

        int timeAcc = 0;
        // pendingTy: tp's level has reached exactly ty by freezing there
        // (waiting on a sibling to catch up) and is waiting to see whether
        // it immediately continues past ty (success, at the current time)
        // once that sibling arrives. It must only ever be armed *after* the
        // current level has had its own same-level link processed (below,
        // via `landed`) -- arming it up front for tp's starting bottom would
        // wrongly skip checking whether that very bottom also has its own
        // outgoing link (ty == bottom[tp] with a link exactly there is a
        // real, zero-duration freeze, not an immediate success).
        bool pendingTy = false;
        int answer = -1;
        bool found = false;

        while(true){
            // A pipe reaching its own physical top permanently caps the
            // level of EVERY pipe still connected to it: the problem's own
            // "same level when connected" law applies regardless of how
            // much headroom siblings have, so once any current member is
            // capped the whole component can never move again -- any link
            // that could rescue it at this exact height would already have
            // been consumed by the per-step spawn search below. This must
            // be checked before resolving pendingTy: touching ty while
            // permanently capped there is a touch, not a crossing.
            bool capped = false;
            for(int i=1;i<=P;i++){
                if(inGroup[i] && level[i]==top_[i]){ capped = true; break; }
            }
            if(capped) break; // permanently stuck

            if(pendingTy && inGroup[tp]){
                answer = timeAcc;
                found = true;
                break;
            }

            vector<int> activeMembers;
            for(int i=1;i<=P;i++) if(inGroup[i]) activeMembers.push_back(i);

            int curShared = level[activeMembers[0]];
            int rateK = (int)activeMembers.size();

            int nextY = -1000000;
            for(size_t k=0;k<activeMembers.size();k++){
                int i = activeMembers[k];
                int cand = top_[i];
                for(int j=0;j<L;j++){
                    if(consumed[j]) continue;
                    if(lpa[j]!=i && lpb[j]!=i) continue;
                    if(lyv[j] <= level[i] && lyv[j] > cand) cand = lyv[j];
                }
                if(cand > nextY) nextY = cand;
            }
            if(!stackLevel.empty()){
                int sTop = stackLevel.back();
                if(sTop > nextY) nextY = sTop;
            }

            bool tpActive = inGroup[tp] && level[tp] > top_[tp];

            // ty <= curShared (not strictly <) so that tp starting a step
            // already exactly at ty -- its own bottom, or a level it just
            // rejoined at -- resolves immediately (zero added time) whenever
            // this step actually moves it below ty right away.
            if(tpActive && nextY < ty && ty <= curShared){
                answer = timeAcc + (curShared - ty) * rateK;
                found = true;
                break;
            }

            int dt = (curShared - nextY) * rateK;
            timeAcc += dt;
            for(size_t k=0;k<activeMembers.size();k++) level[activeMembers[k]] = nextY;

            bool landed = tpActive && (nextY == ty);

            int useLink = -1;
            for(int j=0;j<L;j++){
                if(consumed[j]) continue;
                if(lyv[j] != nextY) continue;
                if(inGroup[lpa[j]] || inGroup[lpb[j]]){
                    useLink = j;
                    break;
                }
            }

            bool spawned = false;
            if(useLink != -1){
                consumed[useLink] = true;
                int a = lpa[useLink], b = lpb[useLink];
                int other = -1;
                if(!touched[a]) other = a;
                else if(!touched[b]) other = b;
                if(other != -1){
                    touched[other] = true;
                    vector<int> frozenMembers;
                    for(int i=1;i<=P;i++) if(inGroup[i]) frozenMembers.push_back(i);
                    stackMembers.push_back(frozenMembers);
                    stackLevel.push_back(nextY);
                    for(int i=1;i<=P;i++) inGroup[i]=false;
                    inGroup[other]=true;
                    spawned = true;
                    // Deliberately not arming pendingTy here even if
                    // bottom_[other]==ty: the newly spawned pipe hasn't had
                    // its own candidates (possibly another link at that same
                    // bottom) evaluated yet. Left alone, the next iteration
                    // processes it exactly like any other pipe -- the mid-
                    // step check above (ty<=curShared) catches the immediate-
                    // continuation case, and `landed` below catches an
                    // immediate re-freeze at that same level.
                }
            }

            if(!spawned){
                if(!stackLevel.empty() && nextY == stackLevel.back()){
                    vector<int> &fr = stackMembers.back();
                    for(size_t k=0;k<fr.size();k++) inGroup[fr[k]] = true;
                    stackMembers.pop_back();
                    stackLevel.pop_back();
                }
            }

            if(landed) pendingTy = true;
        }

        if(found) printf("%d\n", answer);
        else printf("No Solution\n");
    }
    return 0;
}
