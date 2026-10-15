// POJ 3284 - Eventually periodic sequence
// Model: gpt-6.1-sol
// Compile the RPN expression into a polynomial modulo N, then evaluate
// by Horner's rule. First-visit indices give the eventual cycle length.
// Reduction after each operation is valid since only + and * precede %.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;
typedef vector<int> Poly;
int main() {
    int mod, x;
    string line, token;
    while (cin >> mod >> x && mod) {
        getline(cin, line);
        istringstream in(line);
        vector<Poly> st;
        while (in >> token) {
            if (token == "%") {
                st.pop_back(); // The final divisor is N.
            } else if (token == "+" || token == "*") {
                Poly b = st.back(); st.pop_back();
                Poly a = st.back(); st.pop_back();
                Poly c(token == "+" ? max(a.size(), b.size()) : a.size()+b.size()-1, 0);
                if (token == "+") {
                    for (size_t i=0; i<a.size(); ++i) c[i]=a[i];
                    for (size_t i=0; i<b.size(); ++i) c[i]=(c[i]+b[i])%mod;
                } else {
                    for (size_t i=0; i<a.size(); ++i)
                        for (size_t j=0; j<b.size(); ++j)
                            c[i+j]=(c[i+j]+(long long)a[i]*b[j])%mod;
                }
                while (c.size()>1 && c.back()==0) c.pop_back();
                st.push_back(c);
            } else if (token == "x") {
                Poly p(2,0); p[1]=1%mod; st.push_back(p);
            } else {
                int value=0;
                if (token != "N")
                    for (size_t i=0; i<token.size(); ++i) value=((long long)value*10+token[i]-'0')%mod;
                st.push_back(Poly(1,value));
            }
        }
        const Poly &p=st.back();
        vector<int> seen(mod+1,0);
        int step=1;
        while (!seen[x]) {
            seen[x]=step++;
            int y=0;
            for (int i=(int)p.size()-1; i>=0; --i) y=((long long)y*x+p[i])%mod;
            x=y;
        }
        cout << step-seen[x] << '\n';
    }
    return 0;
}
