// POJ 4038 - Game, Set and Match
// Model: gpt-6.1-sol
// Solve deuce by summing its geometric series, then propagate probabilities
// backward through points, games and sets. Sets require six games (the
// original contest statement's four is a typo); use eleven decimal places
// as required by POJ's ordinary output checker.
#include <cstdio>

double race(int n, double p) {
    double d[8][8] = {{0}};
    d[n-1][n-1] = p*p / (p*p+(1-p)*(1-p));
    for (int a=n; a>=0; --a) for (int b=n; b>=0; --b) {
        if (a==n) d[a][b]=1;
        else if (b==n) d[a][b]=0;
        else if (a!=n-1 || b!=n-1)
            d[a][b]=p*d[a+1][b]+(1-p)*d[a][b+1];
    }
    return d[0][0];
}

double setwin(double g,double t) {
    double d[8][8]={{0}};
    for (int a=7;a>=0;--a) for(int b=7;b>=0;--b) {
        if(a==6 && b==6) d[a][b]=t;
        else if((a>=6 && a-b>=2) || a==7) d[a][b]=1;
        else if((b>=6 && b-a>=2) || b==7) d[a][b]=0;
        else d[a][b]=g*d[a+1][b]+(1-g)*d[a][b+1];
    }
    return d[0][0];
}
int main() {
    double p;
    while (std::scanf("%lf",&p)==1 && p>=0) {
        double g=race(4,p), s=setwin(g,race(7,p));
        double m=s*s*(3-2*s);
        std::printf("%.11f %.11f %.11f\n",g,s,m);
    }
    return 0;
}
