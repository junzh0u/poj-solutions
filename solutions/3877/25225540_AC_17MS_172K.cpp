// POJ 3877 - Genetics
// Model: gpt-6.1-sol
// Regard the circular word as the boundary of a polygon with paired edges.
// Equal faces identify corresponding endpoints; opposite faces cross them.
// With E paired edges, V vertex classes and one face, 2-chi = 1+E-V.
// Any equal-face pair makes the surface nonorientable, yielding that many
// arms; otherwise half that quantity is the number of legs.
#include <iostream>
#include <string>
#include <cctype>
using namespace std;
int p[52];
int root(int x) { return p[x]==x?x:p[x]=root(p[x]); }
void join(int a,int b) { p[root(a)]=root(b); }
int main() {
    string s;
    while(cin>>s && s!="END") {
        int n=s.size(), first[26]; bool arms=false;
        for(int i=0;i<26;++i) first[i]=-1;
        for(int i=0;i<n;++i) p[i]=i;
        for(int i=0;i<n;++i) {
            int c=tolower(s[i])-'a';
            if(first[c]<0) first[c]=i;
            else {
                int j=first[c];
                if(s[i]==s[j]) {
                    arms=true; join(i,j); join((i+1)%n,(j+1)%n);
                } else { join(i,(j+1)%n); join((i+1)%n,j); }
            }
        }
        int vertices=0;
        for(int i=0;i<n;++i) if(root(i)==i) ++vertices;
        int count=1+n/2-vertices;
        if(!arms) count/=2;
        if(count==0) cout<<"none\n";
        else cout<<count<<' '<<(arms?"arm":"leg")<<(count==1?"":"s")<<'\n';
    }
}
