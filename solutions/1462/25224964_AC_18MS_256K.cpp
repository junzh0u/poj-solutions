// POJ 1462 - Random Walk
// Model: gpt-6.1-sol
// Solve each procedure's expected remaining times by Gaussian elimination.
// Procedure calls form a DAG; their expectations become command costs.
// END costs zero. Coincident jump and fallthrough transitions are added.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <algorithm>
using namespace std;
struct Command { int type, target; double p; string name; Command():type(0),target(0),p(0){} };
struct Procedure { vector<Command> c; double answer; bool done; Procedure():answer(0),done(false){} };
vector<Procedure> procs;
map<string,int> ids;
double solve(int id) {
    if(procs[id].done) return procs[id].answer;
    int n=procs[id].c.size();
    vector<vector<double> > a(n,vector<double>(n+1,0));
    for(int i=0;i<n;++i) {
        Command c=procs[id].c[i]; a[i][i]=1; a[i][n]=1;
        if(c.type==1) {
            if(i+1<n) a[i][i+1]-=1-c.p;
            if(c.target<n) a[i][c.target]-=c.p;
        } else {
            if(i+1<n) a[i][i+1]-=1;
            if(c.type==2 && c.p>0) a[i][n]+=c.p*solve(ids[c.name]);
        }
    }
    for(int k=0;k<n;++k) {
        int pivot=k;
        for(int i=k+1;i<n;++i) if(fabs(a[i][k])>fabs(a[pivot][k])) pivot=i;
        swap(a[k],a[pivot]);
        for(int i=k+1;i<n;++i) if(a[i][k]!=0) {
            double f=a[i][k]/a[k][k]; a[i][k]=0;
            for(int j=k+1;j<=n;++j) a[i][j]-=f*a[k][j];
        }
    }
    vector<double> x(n,0);
    for(int i=n-1;i>=0;--i) { double b=a[i][n]; for(int j=i+1;j<n;++j) b-=a[i][j]*x[j]; x[i]=b/a[i][i]; }
    procs[id].done=true; procs[id].answer=n?x[0]:0;
    return procs[id].answer;
}
int main() {
    string line; int current=-1;
    while(getline(cin,line)) {
        istringstream ss(line); string word; ss>>word;
        if(word=="PROG_END") break;
        if(word=="PROC") {string name; ss>>name; current=procs.size(); ids[name]=current; procs.push_back(Procedure());}
        else if(word=="NOP;") procs[current].c.push_back(Command());
        else if(word=="IF") {
            string s; for(size_t i=0;i<line.size();++i) if(line[i]!=' ' && line[i]!='\t' && line[i]!='\r') s+=line[i];
            char *end; double threshold=strtod(s.c_str()+4,&end);
            Command c; c.p=s[3]=='<'?threshold:1-threshold;
            c.p=max(0.0,min(1.0,c.p));
            string rest(end); if(rest.substr(0,4)=="GOTO") {c.type=1;c.target=atoi(rest.c_str()+4)-1;}
            else {c.type=2;c.name=rest.substr(4); if(!c.name.empty() && c.name[c.name.size()-1]==';') c.name.erase(c.name.size()-1);}
            procs[current].c.push_back(c);
        }
    }
    cout<<fixed<<setprecision(3);
    while(cin>>line && line!="REQUEST_END") cout<<solve(ids[line])<<'\n';
}
