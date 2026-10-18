// POJ 2617 - Taxes
// Model: gpt-6-astra
// Invert the monotone, cent-rounded net-income function by binary search.
// Tax salary and regional payments separately, then aggregate each stream.
// Choose the smallest cent salary producing the given net amount; rounding
// can make the inverse nonunique. Tax brackets are accumulated before rounding.
// Integer-valued doubles hold cents exactly; split products into quotient and
// remainder to keep every intermediate integer below 2^53.
#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
using namespace std;
vector<double> bound;
vector<int> rate;
int regional;
double percent(double x, int s) {
    double q=floor(x/100);
    return q*s+floor(((x-q*100)*s+50)/100);
}
double tax(double x) {
    double whole=0, rem=0, last=0;
    for (unsigned i=0;i<rate.size();++i) {
        double end=(i<bound.size() && bound[i]<x)?bound[i]:x;
        double part=end-last, q=floor(part/100);
        whole+=q*rate[i]; rem+=(part-q*100)*rate[i];
        if (end==x) break;
        last=end;
    }
    return whole+floor((rem+50)/100);
}
double net(double x) {
    double a=percent(x,regional);
    return x-tax(x)+a-tax(a);
}
int main() {
    if (!(cin>>regional)) return 0;
    double n; int s;
    while (cin>>n>>s) {
        rate.push_back(s);
        if(n==0) break;
        bound.push_back(floor(n*100+0.5));
    }
    double salaries=0, allowances=0, retained=0, income;
    while (cin>>income && income>=0) {
        double target=floor(income*100+0.5), lo=0, hi=100*(target+100);
        while(lo<hi) {
            double mid=floor((lo+hi)/2);
            if(net(mid)<target) lo=mid+1; else hi=mid;
        }
        double a=percent(lo,regional);
        salaries+=lo; allowances+=a; retained+=tax(lo)+tax(a);
    }
    double result=tax(salaries)+tax(allowances)-retained;
    cout<<fixed<<setprecision(2)<<result/100<<"\n";
    return 0;
}
