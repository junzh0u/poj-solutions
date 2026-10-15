// POJ 3330 - Dr. Podboq or: How We Became Asymmetric
// Model: gpt-6.1-sol
// Intern unordered subtree shapes and collect their distinct descendant sets.
// Similarity is intersection/union, compared exactly by cross multiplication.
// Equal similarities are resolved recursively by stronger then weaker child;
// output puts the stronger child outward according to the node's position.
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <utility>
#include <algorithm>
using namespace std;
struct Node { int l,r,id,num,den; set<int> shapes; };
Node a[130]; int cnt,pos;
string s;
map<pair<int,int>,int> ids;
int compareNodes(int x,int y) {
    int d=a[x].num*a[y].den-a[y].num*a[x].den;
    if(d) return d<0 ? -1:1;
    if(a[x].l<0) return 0;
    d=compareNodes(a[x].l,a[y].l);
    return d ? d:compareNodes(a[x].r,a[y].r);
}
int parse() {
    while(s[pos]==' ') ++pos;
    int x=cnt++;
    a[x].shapes.clear();
    if(s[pos]=='x') {
        ++pos; a[x].l=a[x].r=-1; a[x].id=0;
        a[x].num=0; a[x].den=1;
    } else {
        ++pos; int l=parse(),r=parse(); ++pos;
        a[x].l=l; a[x].r=r;
        int common=0;
        set<int>::iterator it;
        a[x].shapes=a[l].shapes;
        for(it=a[r].shapes.begin();it!=a[r].shapes.end();++it) {
            if(a[l].shapes.count(*it)) ++common;
            a[x].shapes.insert(*it);
        }
        a[x].num=common; a[x].den=(int)a[x].shapes.size();
        pair<int,int> key(min(a[l].id,a[r].id),max(a[l].id,a[r].id));
        if(!ids.count(key)) { int next=(int)ids.size()+1; ids[key]=next; }
        a[x].id=ids[key];
        if(compareNodes(l,r)>0) swap(a[x].l,a[x].r);
    }
    a[x].shapes.insert(a[x].id);
    return x;
}
void output(int x,bool left) {
    if(a[x].l<0) { cout << 'x'; return; }
    int l=a[x].l,r=a[x].r;
    if(!left) swap(l,r);
    cout << '('; output(l,true); cout << ' '; output(r,false); cout << ')';
}
int main() {
    while(getline(cin,s) && s!="0") {
        if(s.empty()) continue;
        cnt=pos=0; ids.clear(); int root=parse();
        output(root,true); cout << '\n';
    }
    return 0;
}
