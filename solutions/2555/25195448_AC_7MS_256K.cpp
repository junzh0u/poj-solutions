// POJ 2555 - Drink, on Ice
// Model: claude-sonnet-5
// Approach: energy conservation. Measure every gram's thermal energy
// relative to -30C using cw=4.19 (water), ci=2.09 (ice), em=335 (latent
// heat of melting). Energy of 1g of water at temperature tw (tw>=0) is
// ci*30 + em + cw*tw (heat ice from -30 to 0, melt it, heat water to tw).
// Energy of 1g of ice at temperature ti (ti<=0) is ci*(ti+30).
// Sum over both masses to get total energy E, and let M = mw+mi.
// baseline = M*ci*30 is the energy of the whole mass sitting as ice at 0C.
// excess = E - baseline determines the final state:
//   excess < 0            -> all ice, t = excess/(M*ci)            (t<0)
//   0 <= excess < M*em    -> mixture at 0C, melted = excess/em,
//                             ice = M-melted, water = melted
//   excess >= M*em        -> all water, t = (excess-M*em)/(M*cw)   (t>0)
// This is continuous at both boundaries so which branch is taken there
// does not matter. Ambiguity checked against all 4 samples by hand before
// coding; all matched exactly.
// Discuss-board pitfall (msg 164830): computing t as a ratio whose
// denominator can be algebraically rewritten with a flipped sign produces
// -0.0 instead of 0.0 in the exact-zero case, and printf renders that as
// "-0.0", which the judge rejects even though the value is right. Every
// printed quantity is rounded through round1() below, which forces a
// zero result to positive zero, before being formatted.
#include <cstdio>
#include <cmath>

static const double CW = 4.19;
static const double CI = 2.09;
static const double EM = 335.0;

double round1(double x) {
    double r;
    if (x >= 0) r = std::floor(x * 10.0 + 0.5) / 10.0;
    else        r = -std::floor(-x * 10.0 + 0.5) / 10.0;
    if (r == 0.0) r = 0.0; // normalize -0.0 to +0.0
    return r;
}

int main() {
    double mw, mi, tw, ti;
    while (std::scanf("%lf %lf %lf %lf", &mw, &mi, &tw, &ti) == 4) {
        if (mw == 0 && mi == 0 && tw == 0 && ti == 0) break;

        double M = mw + mi;
        double E = mw * (CI * 30.0 + EM + CW * tw) + mi * (CI * (ti + 30.0));
        double baseline = M * CI * 30.0;
        double excess = E - baseline;

        double iceMass, waterMass, t;
        if (excess < 0) {
            iceMass = M;
            waterMass = 0.0;
            t = excess / (M * CI);
        } else if (excess < M * EM) {
            double melted = excess / EM;
            iceMass = M - melted;
            waterMass = melted;
            t = 0.0;
        } else {
            double excess2 = excess - M * EM;
            iceMass = 0.0;
            waterMass = M;
            t = excess2 / (M * CW);
        }

        if (iceMass < 0) iceMass = 0;
        if (waterMass < 0) waterMass = 0;

        double rIce = round1(iceMass);
        double rWater = round1(waterMass);
        double rT = round1(t);

        std::printf("%.1f g of ice and %.1f g of water at %.1f C\n", rIce, rWater, rT);
    }
    return 0;
}
