// POJ 3727 - Newton's Method
// Model: gpt-6.1-sol
// Parse signed monomials, including the negative exponents confirmed by
// the discussion. Evaluate the function and derivative directly, then
// simulate Newton steps. Both stopping tests return the current index n,
// not n+1; the setter explicitly clarifies the small-step case.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
using namespace std;
struct Term { double c; int e; Term(double a,int b):c(a),e(b){} };
int main() {
    string line;
    while (getline(cin,line)) {
        if (line.find('=') == string::npos) continue;
        vector<Term> terms;
        istringstream in(line.substr(0,line.find('=')));
        string s;
        while (in >> s) {
            string::size_type p=s.find('x');
            double c; int e;
            if (p==string::npos) { c=atof(s.c_str()); e=0; }
            else {
                string a=s.substr(0,p);
                if (a.empty() || a=="+") c=1;
                else if (a=="-") c=-1;
                else c=atof(a.c_str());
                string::size_type q=s.find('^',p);
                e=(q==string::npos)?1:atoi(s.c_str()+q+1);
            }
            terms.push_back(Term(c,e));
        }
        double x;
        if (!(cin>>x)) break;
        getline(cin,line);
        int answer=-1;
        for (int n=0;n<1000;++n) {
            double f=0,d=0;
            for (size_t i=0;i<terms.size();++i) {
                double c=terms[i].c; int e=terms[i].e;
                if (c==0) continue;
                f+=c*pow(x,e);
                if (e!=0) d+=c*e*pow(x,e-1);
            }
            if (fabs(f)<1e-6) { answer=n; break; }
            if (d==0) break;
            double y=x-f/d;
            if (fabs(y-x)<1e-6) { answer=n; break; }
            x=y;
        }
        if (answer<0) cout<<"Bad x0 or bad equation!\n";
        else cout<<answer<<'\n';
    }
}
