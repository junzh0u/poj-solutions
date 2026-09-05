// POJ 1076 - Bowl
// Model: claude-sonnet-5
// Approach: each number in a bowler's line is the number of pins left
// STANDING after that throw (confirmed against the sample: Chuck's
// alternating "1 0" throws score as spares "9/", Solly's run of zeros are
// all strikes). Pins reset to a fresh rack of 10 at the start of every
// frame, and also immediately after any ball that clears the rack via a
// strike or (in frame 10) a completed spare/strike bonus; otherwise the
// rack simply carries whatever was left standing into the next ball. Each
// ball's display mark depends on whether ITS rack was a fresh reset (X for
// a one-ball clear of a fresh rack, / for clearing a rack that still had
// pins left over from an earlier, non-clearing ball in the same rack, - for
// zero pins, else the digit) -- this is a property of the game rule that
// produced the rack, not just of the numeric pin count (frame 10's "X-/"
// example: the second ball misses everything, leaving all 10 up again, and
// the third ball clearing those 10 is still a '/' because that rack was
// never actually reset by rule).
// A bowler's line may end mid-game (game still in progress): parsing simply
// stops once the throw list runs dry, so the final frame shown may carry
// fewer marks than a finished frame would. Scores are derived from a flat
// list of every known per-ball pin count using the usual strike/spare bonus
// lookahead; the cumulative-score line stops at the first frame whose bonus
// ball(s) are not yet present in the input, matching the sample exactly
// (e.g. Lothar shows three "-/" frames but only two cumulative scores).
#include <cstdio>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

// remaining pins after the ball, reset = whether this ball's rack was a
// fresh 10 by game rule (frame start, or right after a strike/spare bonus).
char markChar(int knocked, int remaining, bool reset) {
    if (remaining == 0) return reset ? 'X' : '/';
    if (knocked == 0) return '-';
    return char('0' + knocked);
}

int main() {
    string name;
    bool first = true;
    while (cin >> name) {
        vector<int> throws;
        int x;
        while (cin >> x && x != -1) throws.push_back(x);

        vector< vector<int> > frameBalls;
        vector<string> frameMarks;
        vector<bool> frameComplete;
        size_t idx = 0, N = throws.size();

        for (int f = 1; f <= 10; f++) {
            if (idx >= N) break;
            vector<int> balls;
            string marks;

            int r1 = throws[idx++];
            int k1 = 10 - r1;
            balls.push_back(k1);
            marks += markChar(k1, r1, true);

            bool complete = false;

            if (f <= 9) {
                if (k1 == 10) {
                    complete = true; // strike, frame done with 1 ball
                } else {
                    if (idx >= N) {
                        frameBalls.push_back(balls); frameMarks.push_back(marks); frameComplete.push_back(false);
                        break;
                    }
                    int r2 = throws[idx++];
                    int k2 = r1 - r2; // no reset
                    balls.push_back(k2);
                    marks += markChar(k2, r2, false);
                    complete = true;
                }
            } else {
                // frame 10
                if (idx >= N) {
                    frameBalls.push_back(balls); frameMarks.push_back(marks); frameComplete.push_back(false);
                    break;
                }
                int r2 = throws[idx++];
                int k2;
                bool ball2Reset = (k1 == 10);
                if (ball2Reset) k2 = 10 - r2; else k2 = r1 - r2;
                balls.push_back(k2);
                marks += markChar(k2, r2, ball2Reset);

                bool needThird = (k1 == 10) || (k1 + k2 == 10);
                if (needThird) {
                    if (idx >= N) {
                        frameBalls.push_back(balls); frameMarks.push_back(marks); frameComplete.push_back(false);
                        break;
                    }
                    int r3 = throws[idx++];
                    int k3;
                    bool ball3Reset;
                    if (k1 == 10 && k2 == 10) { ball3Reset = true; k3 = 10 - r3; }
                    else if (k1 == 10) { ball3Reset = false; k3 = r2 - r3; }
                    else { ball3Reset = true; k3 = 10 - r3; } // spare bonus, fresh rack
                    balls.push_back(k3);
                    marks += markChar(k3, r3, ball3Reset);
                }
                complete = true;
            }

            frameBalls.push_back(balls);
            frameMarks.push_back(marks);
            frameComplete.push_back(complete);
        }

        // flatten balls, record start offset of each frame
        vector<int> allBalls;
        vector<int> frameStart(frameBalls.size());
        for (size_t i = 0; i < frameBalls.size(); i++) {
            frameStart[i] = (int)allBalls.size();
            for (size_t j = 0; j < frameBalls[i].size(); j++) allBalls.push_back(frameBalls[i][j]);
        }

        vector<int> cumScore;
        int running = 0;
        for (size_t i = 0; i < frameBalls.size(); i++) {
            if (!frameComplete[i]) break;
            int len = (int)frameBalls[i].size();
            int score = 0;
            bool known = true;

            if (i < 9) {
                if (len == 1) { // strike
                    int p1 = frameStart[i] + 1, p2 = frameStart[i] + 2;
                    if (p2 < (int)allBalls.size()) score = 10 + allBalls[p1] + allBalls[p2];
                    else known = false;
                } else {
                    int sum = frameBalls[i][0] + frameBalls[i][1];
                    if (sum == 10) {
                        int p1 = frameStart[i] + 2;
                        if (p1 < (int)allBalls.size()) score = 10 + allBalls[p1];
                        else known = false;
                    } else {
                        score = sum;
                    }
                }
            } else {
                for (size_t j = 0; j < frameBalls[i].size(); j++) score += frameBalls[i][j];
            }

            if (!known) break;
            running += score;
            cumScore.push_back(running);
        }

        if (!first) cout << "\n";
        first = false;

        // marking line: name left-justified in 13, each frame field
        // left-justified in 3 + a separator space, except the very last
        // field shown, which is printed raw (no padding) + separator space.
        char buf[32];
        snprintf(buf, sizeof(buf), "%-13s", name.c_str());
        string line = buf;
        for (size_t i = 0; i < frameMarks.size(); i++) {
            if (i + 1 < frameMarks.size()) {
                char fbuf[8];
                snprintf(fbuf, sizeof(fbuf), "%-3s", frameMarks[i].c_str());
                line += fbuf;
                line += ' ';
            } else {
                line += frameMarks[i];
                line += ' ';
            }
        }
        cout << line << "\n";

        // score line: blank name field, each known cumulative score
        // right-justified in 3 + a separator space.
        string sline(13, ' ');
        for (size_t i = 0; i < cumScore.size(); i++) {
            char fbuf[8];
            snprintf(fbuf, sizeof(fbuf), "%3d", cumScore[i]);
            sline += fbuf;
            sline += ' ';
        }
        cout << sline << "\n";
    }
    return 0;
}
