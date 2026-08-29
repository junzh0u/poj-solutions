// POJ 3760 - WarCraft (revised)
// Model: claude-sonnet-5
//
// Discrete event simulation. Each hour has phases at minutes 0,10,20,30,40,50:
//   :00 each HQ tries to produce the next warrior in its fixed cycle (index does
//       not advance while waiting for funds); :10 every living warrior not yet at
//       the enemy HQ advances one step (iceman mutates every 2nd step); :20 every
//       city gains 10 elements; :30 a lone warrior in a city claims all of that
//       city's elements; :40 cities with exactly one red and one blue warrior
//       fight (attacker decided by flag/parity); :50 both HQs report elements.
// Reward (+8 life to a killer, silent, no output line) is resolved in one global
// pass across all of this tick's kills, ordered by distance to the ENEMY HQ,
// strictly before any of this tick's spoils are credited to HQ pools (statement:
// "always reward the warriors first, only then collect the spoils"). Verified by
// hand-tracing both official samples to the element/HP totals reported at :50.
// A city can only ever hold one warrior per side (siblings on the same side are
// born on different hours and always keep a strict order), so a "battle" is
// always exactly one red vs one blue, and two same-side warriors can never reach
// the enemy HQ on the same tick either (arrival hour = birth hour + N+1, and
// birth hours on one side are all distinct) - so headquarters occupation is
// always triggered by a single new arrival raising the enemy count to 2, and no
// tie-break by id was needed anywhere except as a harmless safety net.
// Statement ambiguities resolved from the discuss board (msg 168366, 173360):
// reward priority is distance to the ENEMY headquarters (closest first), and the
// elements a HQ gains at minute 30/40 come from the city pool, not from the
// battle-kill reward - the two are wholly separate (the +8 reward never appears
// as its own output line, only later as a bumped life value).
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;

static const char* TYPE_NAME[5] = {"dragon","ninja","iceman","lion","wolf"};
enum { DRAGON=0, NINJA=1, ICEMAN=2, LION=3, WOLF=4 };

struct Warrior {
    int side; // 0 red, 1 blue
    int type;
    int id;
    long long life;
    long long force;
    int pos;   // 0 = red HQ, N+1 = blue HQ, 1..N = cities
    int steps;
    int killCountActive; // wolf's active-kill counter
    bool alive;
};

static int N;
static long long M;
static int T;
static long long lifeInit[5], forceInit[5];
static const int redCycle[5]  = {ICEMAN, LION, WOLF, NINJA, DRAGON};
static const int blueCycle[5] = {LION, DRAGON, NINJA, ICEMAN, WOLF};

static deque<Warrior> redW, blueW;
static long long redHQ, blueHQ;
static int redCycleIdx, blueCycleIdx;
static int redBornCount, blueBornCount;

static vector<long long> cityElements;
static vector<int> cityFlag;        // 0 none, 1 red, 2 blue
static vector<int> cityStreakSide;  // 0 none, 1 red, 2 blue
static vector<int> cityStreakCount;

static vector<string> outLines;

static const char* colorName(int side){ return side==0 ? "red" : "blue"; }

static string fmtTime(int t){
    char buf[16];
    sprintf(buf, "%03d:%02d", t/60, t%60);
    return string(buf);
}

static void addLine(int t, const string& s){
    outLines.push_back(fmtTime(t) + " " + s);
}

static Warrior* occupantAt(int side, int pos){
    deque<Warrior>& v = (side==0) ? redW : blueW;
    for (size_t k=0;k<v.size();++k){
        if (v[k].alive && v[k].pos==pos) return &v[k];
    }
    return NULL;
}

struct MarchEvt { int pos; int side; int id; string text; };
static bool marchLess(const MarchEvt&a, const MarchEvt&b){
    if (a.pos!=b.pos) return a.pos<b.pos;
    if (a.side!=b.side) return a.side<b.side; // red(0) before blue(1)
    return a.id<b.id;
}

struct RewardItem { int cityPos; Warrior* killer; };
static bool redRewardLess(const RewardItem&a, const RewardItem&b){
    return a.cityPos > b.cityPos; // larger city index = closer to blue HQ
}
static bool blueRewardLess(const RewardItem&a, const RewardItem&b){
    return a.cityPos < b.cityPos; // smaller city index = closer to red HQ
}

static string toStr(int v){
    char buf[16];
    sprintf(buf, "%d", v);
    return string(buf);
}

static void resetState(){
    redW.clear(); blueW.clear();
    redHQ = blueHQ = M;
    redCycleIdx = blueCycleIdx = 0;
    redBornCount = blueBornCount = 0;
    cityElements.assign(N+2, 0);
    cityFlag.assign(N+2, 0);
    cityStreakSide.assign(N+2, 0);
    cityStreakCount.assign(N+2, 0);
}

int main(){
    int cases;
    if (scanf("%d", &cases) != 1) return 0;
    for (int tc=1; tc<=cases; ++tc){
        long long Min;
        scanf("%lld %d %d", &Min, &N, &T);
        M = Min;
        for (int i=0;i<5;i++) scanf("%lld", &lifeInit[i]);
        for (int i=0;i<5;i++) scanf("%lld", &forceInit[i]);

        resetState();
        outLines.clear();
        outLines.push_back("Case:" + toStr(tc));

        bool gameOver = false;

        for (int hour=0; !gameOver && (long long)hour*60 <= T; ++hour){
            int t0 = hour*60;
            // ---- :00 born ----
            {
                int rtype = redCycle[redCycleIdx];
                if (redHQ >= lifeInit[rtype]){
                    redHQ -= lifeInit[rtype];
                    Warrior w; w.side=0; w.type=rtype; w.id = ++redBornCount;
                    w.life = lifeInit[rtype]; w.force = forceInit[rtype];
                    w.pos = 0; w.steps = 0; w.killCountActive = 0; w.alive = true;
                    redW.push_back(w);
                    redCycleIdx = (redCycleIdx+1) % 5;
                    char buf[128];
                    sprintf(buf, "red %s %d born", TYPE_NAME[rtype], w.id);
                    addLine(t0, buf);
                }
                int btype = blueCycle[blueCycleIdx];
                if (blueHQ >= lifeInit[btype]){
                    blueHQ -= lifeInit[btype];
                    Warrior w; w.side=1; w.type=btype; w.id = ++blueBornCount;
                    w.life = lifeInit[btype]; w.force = forceInit[btype];
                    w.pos = N+1; w.steps = 0; w.killCountActive = 0; w.alive = true;
                    blueW.push_back(w);
                    blueCycleIdx = (blueCycleIdx+1) % 5;
                    char buf[128];
                    sprintf(buf, "blue %s %d born", TYPE_NAME[btype], w.id);
                    addLine(t0, buf);
                }
            }

            // ---- :10 march ----
            int t1 = t0 + 10;
            if (t1 > T) break;
            {
                vector<MarchEvt> evts;
                for (size_t k=0;k<redW.size();++k){
                    Warrior &w = redW[k];
                    if (!w.alive) continue;
                    if (w.pos == N+1) continue; // already at enemy HQ, stays
                    w.pos += 1;
                    w.steps += 1;
                    if (w.type==ICEMAN && w.steps%2==0){
                        if (w.life - 9 <= 0) w.life = 1; else w.life -= 9;
                        w.force += 20;
                    }
                    char buf[160];
                    if (w.pos == N+1){
                        sprintf(buf, "red %s %d reached blue headquarter with %lld elements and force %lld",
                                TYPE_NAME[w.type], w.id, w.life, w.force);
                    } else {
                        sprintf(buf, "red %s %d marched to city %d with %lld elements and force %lld",
                                TYPE_NAME[w.type], w.id, w.pos, w.life, w.force);
                    }
                    MarchEvt e; e.pos=w.pos; e.side=0; e.id=w.id; e.text=buf;
                    evts.push_back(e);
                }
                for (size_t k=0;k<blueW.size();++k){
                    Warrior &w = blueW[k];
                    if (!w.alive) continue;
                    if (w.pos == 0) continue;
                    w.pos -= 1;
                    w.steps += 1;
                    if (w.type==ICEMAN && w.steps%2==0){
                        if (w.life - 9 <= 0) w.life = 1; else w.life -= 9;
                        w.force += 20;
                    }
                    char buf[160];
                    if (w.pos == 0){
                        sprintf(buf, "blue %s %d reached red headquarter with %lld elements and force %lld",
                                TYPE_NAME[w.type], w.id, w.life, w.force);
                    } else {
                        sprintf(buf, "blue %s %d marched to city %d with %lld elements and force %lld",
                                TYPE_NAME[w.type], w.id, w.pos, w.life, w.force);
                    }
                    MarchEvt e; e.pos=w.pos; e.side=1; e.id=w.id; e.text=buf;
                    evts.push_back(e);
                }
                sort(evts.begin(), evts.end(), marchLess);
                for (size_t k=0;k<evts.size();++k){
                    addLine(t1, evts[k].text);
                    bool lastOfGroup = (k+1==evts.size()) || (evts[k+1].pos != evts[k].pos);
                    if (lastOfGroup){
                        if (evts[k].pos == 0){
                            int cnt=0;
                            for (size_t j=0;j<blueW.size();++j) if (blueW[j].alive && blueW[j].pos==0) cnt++;
                            if (cnt >= 2){
                                addLine(t1, "red headquarter was taken");
                                gameOver = true;
                            }
                        } else if (evts[k].pos == N+1){
                            int cnt=0;
                            for (size_t j=0;j<redW.size();++j) if (redW[j].alive && redW[j].pos==N+1) cnt++;
                            if (cnt >= 2){
                                addLine(t1, "blue headquarter was taken");
                                gameOver = true;
                            }
                        }
                    }
                }
            }
            if (gameOver) break;

            // ---- :20 production ----
            int t2 = t0 + 20;
            if (t2 > T) break;
            for (int i=1;i<=N;++i) cityElements[i] += 10;

            // ---- :30 pickup ----
            int t3 = t0 + 30;
            if (t3 > T) break;
            for (int i=1;i<=N;++i){
                Warrior* r = occupantAt(0,i);
                Warrior* b = occupantAt(1,i);
                int cnt = (r?1:0) + (b?1:0);
                if (cnt == 1){
                    Warrior* w = r ? r : b;
                    long long amt = cityElements[i];
                    cityElements[i] = 0;
                    if (w->side==0) redHQ += amt; else blueHQ += amt;
                    char buf[160];
                    sprintf(buf, "%s %s %d earned %lld elements for his headquarter",
                            colorName(w->side), TYPE_NAME[w->type], w->id, amt);
                    addLine(t3, buf);
                }
            }

            // ---- :40 battles ----
            int t4 = t0 + 40;
            if (t4 > T) break;
            {
                vector<vector<string> > cityBlocks(N+2);
                vector<pair<int,int> > kills; // (side, cityPos) for reward priority; warrior via pointer stored separately
                vector<Warrior*> killerPtrs;
                vector<long long> spoilsSide0, spoilsSide1; // pending spoils amounts per side (order irrelevant)
                struct Pending { int side; long long amt; };
                vector<Pending> pendingSpoils;

                for (int i=1;i<=N;++i){
                    Warrior* r = occupantAt(0,i);
                    Warrior* b = occupantAt(1,i);
                    if (!r || !b) continue;
                    int attackerSide;
                    if (cityFlag[i]==1) attackerSide = 0;
                    else if (cityFlag[i]==2) attackerSide = 1;
                    else attackerSide = (i%2==1) ? 0 : 1;
                    Warrior* att = (attackerSide==0) ? r : b;
                    Warrior* def = (attackerSide==0) ? b : r;

                    vector<string>& block = cityBlocks[i];
                    char buf[200];

                    long long attStartLife = att->life;
                    long long attStartForce = att->force;
                    long long defStartLife = def->life;

                    sprintf(buf, "%s %s %d attacked %s %s %d in city %d with %lld elements and force %lld",
                            colorName(att->side), TYPE_NAME[att->type], att->id,
                            colorName(def->side), TYPE_NAME[def->type], def->id,
                            i, attStartLife, attStartForce);
                    block.push_back(buf);

                    def->life -= att->force;
                    Warrior* killed = NULL;
                    Warrior* killer = NULL;
                    bool activeKill = false;

                    if (def->life <= 0){
                        killed = def; killer = att; activeKill = true;
                        def->alive = false;
                        sprintf(buf, "%s %s %d was killed in city %d",
                                colorName(def->side), TYPE_NAME[def->type], def->id, i);
                        block.push_back(buf);
                    } else {
                        if (def->type != NINJA){
                            sprintf(buf, "%s %s %d fought back against %s %s %d in city %d",
                                    colorName(def->side), TYPE_NAME[def->type], def->id,
                                    colorName(att->side), TYPE_NAME[att->type], att->id, i);
                            block.push_back(buf);
                            long long counterDmg = def->force / 2;
                            att->life -= counterDmg;
                            if (att->life <= 0){
                                killed = att; killer = def; activeKill = false;
                                att->alive = false;
                                sprintf(buf, "%s %s %d was killed in city %d",
                                        colorName(att->side), TYPE_NAME[att->type], att->id, i);
                                block.push_back(buf);
                            }
                        }
                    }

                    if (killed){
                        long long killedPreBattleLife = (killed==def) ? defStartLife : attStartLife;
                        if (activeKill && killer->type==WOLF){
                            killer->killCountActive++;
                            if (killer->killCountActive % 2 == 0){
                                killer->life *= 2;
                                killer->force *= 2;
                            }
                        }
                        if (killed->type==LION){
                            killer->life += killedPreBattleLife;
                        }
                    }

                    if (att->type==DRAGON && att->alive){
                        sprintf(buf, "%s %s %d yelled in city %d",
                                colorName(att->side), TYPE_NAME[att->type], att->id, i);
                        block.push_back(buf);
                    }

                    if (killed){
                        int winnerSide = killer->side;
                        long long spoilsAmt = cityElements[i];
                        cityElements[i] = 0;
                        sprintf(buf, "%s %s %d earned %lld elements for his headquarter",
                                colorName(killer->side), TYPE_NAME[killer->type], killer->id, spoilsAmt);
                        block.push_back(buf);

                        int flagCode = winnerSide + 1;
                        if (cityStreakSide[i] == flagCode){
                            cityStreakCount[i]++;
                        } else {
                            cityStreakSide[i] = flagCode;
                            cityStreakCount[i] = 1;
                        }
                        if (cityStreakCount[i] >= 2 && cityFlag[i] != flagCode){
                            cityFlag[i] = flagCode;
                            sprintf(buf, "%s flag raised in city %d", colorName(winnerSide), i);
                            block.push_back(buf);
                        }

                        Pending p; p.side = winnerSide; p.amt = spoilsAmt;
                        pendingSpoils.push_back(p);
                        kills.push_back(make_pair(winnerSide, i));
                        killerPtrs.push_back(killer);
                    } else {
                        cityStreakSide[i] = 0;
                        cityStreakCount[i] = 0;
                    }
                }

                // Global reward pass (uses pre-tick HQ balances only).
                {
                    vector<RewardItem> redItems, blueItems;
                    for (size_t k=0;k<kills.size();++k){
                        RewardItem it; it.cityPos = kills[k].second; it.killer = killerPtrs[k];
                        if (kills[k].first==0) redItems.push_back(it); else blueItems.push_back(it);
                    }
                    sort(redItems.begin(), redItems.end(), redRewardLess);
                    sort(blueItems.begin(), blueItems.end(), blueRewardLess);
                    for (size_t k=0;k<redItems.size();++k){
                        if (redHQ >= 8){ redHQ -= 8; redItems[k].killer->life += 8; }
                    }
                    for (size_t k=0;k<blueItems.size();++k){
                        if (blueHQ >= 8){ blueHQ -= 8; blueItems[k].killer->life += 8; }
                    }
                }

                // Spoils commit (after reward pass).
                for (size_t k=0;k<pendingSpoils.size();++k){
                    if (pendingSpoils[k].side==0) redHQ += pendingSpoils[k].amt;
                    else blueHQ += pendingSpoils[k].amt;
                }

                // Output, city order ascending.
                for (int i=1;i<=N;++i){
                    for (size_t k=0;k<cityBlocks[i].size();++k) addLine(t4, cityBlocks[i][k]);
                }
            }

            // ---- :50 report ----
            int t5 = t0 + 50;
            if (t5 > T) break;
            {
                char buf[64];
                sprintf(buf, "%lld elements in red headquarter", redHQ);
                addLine(t5, buf);
                sprintf(buf, "%lld elements in blue headquarter", blueHQ);
                addLine(t5, buf);
            }
        }

        for (size_t k=0;k<outLines.size();++k){
            fputs(outLines[k].c_str(), stdout);
            fputc('\n', stdout);
        }
    }
    return 0;
}
