// POJ 3510 - A Tale from the Dark Side of the Moon
// Model: claude-opus-5
//
// Single left-to-right streaming pass over the RAW input bytes.  For each byte
// we look at the three raw bytes that preceded it:
//   "EOF"  -> stop immediately, emitting everything produced so far
//   "dd"   -> retract the emitted 'd', emit 'p'
//   "ei"   -> retract the emitted 'e', emit "ie", unless the raw byte before
//             the 'e' is 'c'
//   "pink" -> retract the emitted "pin", emit "floyd"
//   otherwise emit the byte if it is a lowercase letter or ' ', '\t', '\n'
//
// Two readings the statement leaves open, both settled before submitting:
//
// 1) Adjacency is measured on the RAW input, not on the filtered text.  The
//    sample decides this: "d123d" stays "dd" while "123dd" becomes "p", so the
//    discarded bytes still break a pair.  Filtering first would collapse both.
//
// 2) Nothing is rescanned.  A rule never fires on characters it just produced,
//    so "eiie" -> "ieie" (not "iiee"), "eiii" -> "ieii" (not "iiie"), and
//    "ddink" -> "pink" rather than "floyd".  The official sample cannot tell
//    this apart; the discuss board can, and reports it as a repeated WA.
//
// "EOF" may appear anywhere, including mid-line, and the text before it is
// still output ("ddEOF" -> "p").  The "vv" -> "m" rule is retracted by the
// statement and the sample confirms it ("vv is ok" survives unchanged).
//
// Output is held back four characters so a rule can retract up to three; the
// rest streams out, so memory is constant regardless of document length.

#include <stdio.h>

static const int IN = 1 << 16;
static const int OUT = 1 << 16;

static char inbuf[IN];
static char obuf[OUT];
static int on = 0;

static void flushc(char c) {
    obuf[on++] = c;
    if (on == OUT) { fwrite(obuf, 1, on, stdout); on = 0; }
}

/* the last <=4 emitted characters, still retractable */
static char tail[4];
static int tn = 0;

static void emit(char c) {
    if (tn == 4) {
        flushc(tail[0]);
        tail[0] = tail[1]; tail[1] = tail[2]; tail[2] = tail[3];
        tn = 3;
    }
    tail[tn++] = c;
}

static void rb(int k) { tn = (tn >= k) ? tn - k : 0; }

int main() {
    int p1 = -1, p2 = -1, p3 = -1; /* preceding raw bytes, p1 the most recent */
    int done = 0;
    size_t len;
    while (!done && (len = fread(inbuf, 1, IN, stdin)) > 0) {
        for (size_t idx = 0; idx < len; ++idx) {
            int a = (unsigned char)inbuf[idx];
            if (a == 'F' && p1 == 'O' && p2 == 'E') { done = 1; break; }
            if (a == 'd' && p1 == 'd') {
                rb(1); emit('p');
            } else if (a == 'i' && p1 == 'e' && p2 != 'c') {
                rb(1); emit('i'); emit('e');
            } else if (a == 'k' && p1 == 'n' && p2 == 'i' && p3 == 'p') {
                rb(3); emit('f'); emit('l'); emit('o'); emit('y'); emit('d');
            } else if ((a >= 'a' && a <= 'z') || a == ' ' || a == '\n' || a == '\t') {
                emit((char)a);
            }
            p3 = p2; p2 = p1; p1 = a;
        }
    }
    for (int i = 0; i < tn; ++i) flushc(tail[i]);
    if (on) fwrite(obuf, 1, on, stdout);
    return 0;
}
