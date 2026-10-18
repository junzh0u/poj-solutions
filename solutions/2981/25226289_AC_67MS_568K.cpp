// POJ 2981 - Deal of Fortune
// Model: gpt-6-astra
// Normalize whole-word negation and solve the resulting CNF by CDCL:
// watched literals, first-UIP learning, activity branching and restarts.
// Only a non-first subclause loses its leading 'or' token; prefixes such
// as 'orange' and 'notable' are ordinary words. Preserve a negative form
// from the input when available; otherwise prefix the positive form.
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
struct Solver {
    int n, head;
    double inc;
    vector<vector<int> > c, watch;
    vector<int> val, level, reason, phase, trail, lim;
    vector<double> activity;
    Solver(int size): n(size), head(0), inc(1), watch(2*size),
        val(size,-1), level(size), reason(size,-1), phase(size,0), activity(size,0) {}
    int value(int p) { return val[p/2]<0 ? -1 : (val[p/2] != (p%2)); }
    bool put(int p,int r) {
        if(value(p)!=-1) return value(p)==1;
        int v=p/2; val[v]=1-p%2; phase[v]=val[v];
        level[v]=lim.size(); reason[v]=r; trail.push_back(p); return true;
    }
    int add(const vector<int>& a) {
        int id=c.size(); c.push_back(a);
        if(a.size()>1) { watch[a[0]].push_back(id); watch[a[1]].push_back(id); }
        return id;
    }
    int propagate() {
        while(head<(int)trail.size()) {
            int f=trail[head++]^1;
            vector<int>& w=watch[f];
            for(int i=0;i<(int)w.size();) {
                int id=w[i]; vector<int>& a=c[id];
                if(a[0]==f) swap(a[0],a[1]);
                if(value(a[0])==1) { ++i; continue; }
                int k=2;
                while(k<(int)a.size() && value(a[k])==0) ++k;
                if(k<(int)a.size()) {
                    swap(a[1],a[k]); watch[a[1]].push_back(id);
                    w[i]=w.back(); w.pop_back();
                } else {
                    if(!put(a[0],id)) return id;
                    ++i;
                }
            }
        }
        return -1;
    }
    void back(int target) {
        if((int)lim.size()<=target) return;
        int cut=lim[target];
        for(int i=(int)trail.size()-1;i>=cut;--i) val[trail[i]/2]=-1;
        trail.resize(cut); head=cut; lim.resize(target);
    }
    bool solve() {
        for(int i=0;i<(int)c.size();++i) {
            if(c[i].empty()) return false;
            for(int j=0;j<(int)c[i].size();++j) activity[c[i][j]/2]+=1.0/c[i].size();
            if(c[i].size()==1 && !put(c[i][0],i)) return false;
        }
        int conflicts=0, restart=100;
        for(;;) {
            int bad=propagate();
            if(bad>=0) {
                if(lim.empty()) return false;
                vector<int> learned(1,-1), seen(n,0);
                int pending=0, p=-1, idx=trail.size()-1;
                do {
                    vector<int>& a=c[bad];
                    for(int j=0;j<(int)a.size();++j) {
                        int v=a[j]/2;
                        if((p>=0 && v==p/2) || seen[v] || level[v]==0) continue;
                        seen[v]=1; activity[v]+=inc;
                        if(activity[v]>1e100) {
                            for(int k=0;k<n;++k) activity[k]*=1e-100;
                            inc*=1e-100;
                        }
                        if(level[v]==(int)lim.size()) ++pending;
                        else learned.push_back(a[j]);
                    }
                    while(!seen[trail[idx]/2]) --idx;
                    p=trail[idx--]; seen[p/2]=0; --pending;
                    bad=reason[p/2];
                } while(pending>0);
                learned[0]=p^1;
                int target=0;
                for(int j=1;j<(int)learned.size();++j)
                    if(level[learned[j]/2]>target) {
                        target=level[learned[j]/2]; swap(learned[1],learned[j]);
                    }
                back(target);
                int id=add(learned); put(learned[0],id);
                inc/=0.95; ++conflicts;
            } else {
                if(conflicts>=restart) {
                    back(0); conflicts=0; restart=restart*3/2;
                    continue;
                }
                int v=-1;
                for(int i=0;i<n;++i)
                    if(val[i]<0 && (v<0 || activity[i]>activity[v])) v=i;
                if(v<0) return true;
                lim.push_back(trail.size()); put(2*v+(1-phase[v]),-1);
            }
        }
    }
};
int main() {
    map<string,int> ids;
    vector<string> names, negatives;
    vector<vector<int> > clauses;
    vector<int> current;
    string line;
    while(getline(cin,line)) {
        if(!line.empty() && line[line.size()-1]=='\r') line.erase(line.size()-1);
        if(line=="and") { clauses.push_back(current); current.clear(); continue; }
        if(line.empty()) continue;
        if(!current.empty() && line.substr(0,3)=="or ") line.erase(0,3);
        istringstream in(line); string word, name; bool neg=false;
        while(in>>word) {
            if(word=="not") neg=true;
            else { if(!name.empty()) name+=' '; name+=word; }
        }
        int id;
        if(ids.find(name)==ids.end()) {
            id=names.size(); ids[name]=id; names.push_back(name);
            negatives.push_back("not "+name);
        } else id=ids[name];
        if(neg) negatives[id]=line;
        current.push_back(2*id+(neg?1:0));
    }
    if(!current.empty()) clauses.push_back(current);
    Solver s(names.size());
    for(int i=0;i<(int)clauses.size();++i) s.add(clauses[i]);
    if(s.solve()) for(int i=0;i<(int)names.size();++i)
        cout<<(s.val[i]?names[i]:negatives[i])<<'\n';
    return 0;
}
