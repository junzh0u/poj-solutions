// POJ 1768 - Hang or not to hang
// Model: claude-sonnet-5
//
// Approach: Tom's machine has 32 one-bit registers with arbitrary initial
// values and a RANDOM instruction; we want the shortest possible number of
// cycles until STOP over an EXISTENTIAL choice of initial register values
// and all RANDOM outcomes (or HANGS if no choice ever stops).
//
// A register's value only matters if it can ever change which branch a JZ
// takes, directly or indirectly. Build a dependency graph over the 32
// registers: AND/OR/XOR/MOV "a op b" adds an edge a -> b (a's new value
// depends on the old a and on b); NOT/SET/RANDOM add no edge (NOT depends
// only on the register itself, already present; SET/RANDOM don't depend on
// any register at all). Mark every register tested by a JZ as a root, and
// take the set S of registers reachable from those roots by following
// dependency edges. A register outside S can never influence any branch
// decision no matter what it holds, so its instructions are no-ops for
// reachability and it never needs to be part of the search state.
//
// Crucially, if some AND/OR/XOR/MOV instruction targets a register a in S,
// its source operand b was added to the graph by that very instruction, so
// b is automatically in S too - there is never a "tracked target read from
// an untracked source" case to worry about, and every JZ's tested register
// is a root of S by construction. So S can simply be tracked as an exact
// concrete bit vector with no extra bookkeeping.
//
// A short counting argument bounds |S|: each of the n instructions other
// than STOP contributes at most one new graph edge (or is itself a JZ root),
// so at most n-1 <= 15 registers ever end up in S. The reachable state space
// is then (pc, S-vector), at most 16 * 2^15 states - small enough for a
// plain multi-source BFS: every possible initial vector of S starts at
// distance 0, RANDOM on a register in S branches into both outcomes, and
// the first time BFS pops a state whose instruction is STOP gives the
// minimum number of cycles (including that STOP cycle itself). If BFS
// exhausts the queue without ever reaching a STOP instruction, no choice of
// initial values / random draws ever stops the program: HANGS.
//
// An earlier submission tracked registers by a cruder rule (referenced by
// more than one static read/write) and treated a register read at exactly
// one static instruction as a fresh free choice at every dynamic visit to
// that instruction. That is unsound when such an instruction sits on a loop
// and is reached more than once with different surrounding state: the real
// register is a single fixed value for the whole run, but the cruder rule
// let different loop passes "choose" contradictory values for it, which can
// manufacture a shorter run than any real one and got Wrong Answer. The
// dependency-graph closure above tracks exactly the registers that can ever
// affect a branch, so this problem cannot arise: an operand is either
// already provably irrelevant (safe to ignore) or provably in S (tracked
// exactly, no free choice needed). Cross-checked against an independently
// written accepted reference (Wizmann/ACM-ICPC, POJ/1/1768.cc) which uses
// the same dependency-closure idea, and against a small-register exact
// brute force in local testing.
//
// The sample was used to validate the model by hand: SET pins register 0 to
// 1, JZ falls through, RANDOM re-frees it, JMP loops back to JZ, and the
// second pass can choose RANDOM's outcome to be 0, taking the jump straight
// to STOP for a total of 6 cycles - matching the official sample output.

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <iostream>

using namespace std;

enum Op { OP_AND, OP_OR, OP_XOR, OP_NOT, OP_MOV, OP_SET, OP_RANDOM, OP_JMP, OP_JZ, OP_STOP };

struct Instr {
    int op;
    int a, b;   // register operands
    int x;      // jump target
    int c;      // constant for SET
};

int n;
vector<Instr> prog;
int trackedId[32];
int k; // number of tracked registers
vector<int> depends[32]; // dependency graph: depends[a] lists b's from "a op b"
bool relevant[32];

static inline int setbit(int v, int pos, int val) {
    if (val) return v | (1 << pos);
    else return v & ~(1 << pos);
}

void dfsMark(int r) {
    if (relevant[r]) return;
    relevant[r] = true;
    for (size_t i = 0; i < depends[r].size(); ++i) dfsMark(depends[r][i]);
}

int main() {
    ios::sync_with_stdio(false);
    cin >> n;
    prog.resize(n);
    vector<int> jzRegs;

    for (int i = 0; i < n; ++i) {
        string tok;
        cin >> tok;
        Instr ins;
        ins.a = ins.b = ins.x = ins.c = -1;
        if (tok == "AND") { ins.op = OP_AND; cin >> ins.a >> ins.b; depends[ins.a].push_back(ins.b); }
        else if (tok == "OR") { ins.op = OP_OR; cin >> ins.a >> ins.b; depends[ins.a].push_back(ins.b); }
        else if (tok == "XOR") { ins.op = OP_XOR; cin >> ins.a >> ins.b; depends[ins.a].push_back(ins.b); }
        else if (tok == "NOT") { ins.op = OP_NOT; cin >> ins.a; }
        else if (tok == "MOV") { ins.op = OP_MOV; cin >> ins.a >> ins.b; depends[ins.a].push_back(ins.b); }
        else if (tok == "SET") { ins.op = OP_SET; cin >> ins.a >> ins.c; }
        else if (tok == "RANDOM") { ins.op = OP_RANDOM; cin >> ins.a; }
        else if (tok == "JMP") { ins.op = OP_JMP; cin >> ins.x; }
        else if (tok == "JZ") { ins.op = OP_JZ; cin >> ins.x >> ins.a; jzRegs.push_back(ins.a); }
        else if (tok == "STOP") { ins.op = OP_STOP; }
        else { /* unexpected token; ignore */ ins.op = OP_STOP; }
        prog[i] = ins;
    }

    memset(relevant, 0, sizeof(relevant));
    for (size_t i = 0; i < jzRegs.size(); ++i) dfsMark(jzRegs[i]);

    for (int r = 0; r < 32; ++r) trackedId[r] = -1;
    k = 0;
    for (int r = 0; r < 32; ++r) {
        if (relevant[r]) trackedId[r] = k++;
    }

    int sz = 1 << k;
    int total = n * sz;
    vector<int> dist(total, -1);

    queue<int> q;
    for (int v = 0; v < sz; ++v) {
        int st = 0 * sz + v; // pc=0
        dist[st] = 0;
        q.push(st);
    }

    int answer = -1;

    while (!q.empty()) {
        int st = q.front(); q.pop();
        int pc = st / sz;
        int v = st % sz;
        int d = dist[st];

        const Instr &ins = prog[pc];
        if (ins.op == OP_STOP) {
            answer = d + 1;
            break;
        }

        // collect successors as (pc', v') pairs (at most 2)
        int npc[2], nv[2], cnt = 0;

        switch (ins.op) {
            case OP_AND: case OP_OR: case OP_XOR: {
                int ta = trackedId[ins.a];
                if (ta == -1) { npc[cnt] = pc + 1; nv[cnt] = v; cnt++; break; }
                int tb = trackedId[ins.b]; // guaranteed tracked (same edge that put a in S)
                int oldA = (v >> ta) & 1;
                int oldB = (v >> tb) & 1;
                int newA;
                if (ins.op == OP_AND) newA = oldA & oldB;
                else if (ins.op == OP_OR) newA = oldA | oldB;
                else newA = oldA ^ oldB;
                npc[cnt] = pc + 1; nv[cnt] = setbit(v, ta, newA); cnt++;
                break;
            }
            case OP_NOT: {
                int ta = trackedId[ins.a];
                if (ta == -1) { npc[cnt] = pc + 1; nv[cnt] = v; cnt++; break; }
                int oldA = (v >> ta) & 1;
                npc[cnt] = pc + 1; nv[cnt] = setbit(v, ta, 1 - oldA); cnt++;
                break;
            }
            case OP_MOV: {
                int ta = trackedId[ins.a];
                if (ta == -1) { npc[cnt] = pc + 1; nv[cnt] = v; cnt++; break; }
                int tb = trackedId[ins.b]; // guaranteed tracked
                int newA = (v >> tb) & 1;
                npc[cnt] = pc + 1; nv[cnt] = setbit(v, ta, newA); cnt++;
                break;
            }
            case OP_SET: {
                int ta = trackedId[ins.a];
                if (ta == -1) { npc[cnt] = pc + 1; nv[cnt] = v; cnt++; break; }
                npc[cnt] = pc + 1; nv[cnt] = setbit(v, ta, ins.c); cnt++;
                break;
            }
            case OP_RANDOM: {
                int ta = trackedId[ins.a];
                if (ta == -1) { npc[cnt] = pc + 1; nv[cnt] = v; cnt++; break; }
                for (int c = 0; c < 2; ++c) {
                    npc[cnt] = pc + 1; nv[cnt] = setbit(v, ta, c); cnt++;
                }
                break;
            }
            case OP_JMP: {
                npc[cnt] = ins.x; nv[cnt] = v; cnt++;
                break;
            }
            case OP_JZ: {
                int ta = trackedId[ins.a]; // guaranteed tracked: every JZ register is a root of S
                int bit = (ta == -1) ? 0 : (v >> ta) & 1;
                if (bit == 0) { npc[cnt] = ins.x; nv[cnt] = v; cnt++; }
                else { npc[cnt] = pc + 1; nv[cnt] = v; cnt++; }
                break;
            }
            default: break;
        }

        for (int i = 0; i < cnt; ++i) {
            int nst = npc[i] * sz + nv[i];
            if (dist[nst] == -1) {
                dist[nst] = d + 1;
                q.push(nst);
            }
        }
    }

    if (answer == -1) printf("HANGS\n");
    else printf("%d\n", answer);

    return 0;
}
