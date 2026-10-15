// POJ 2873 - Apply a Cold Compress
// Model: gpt-6.1-sol
// Compute each subtree's smallest dimensions using least common multiples,
// then propagate integral scale factors to paint the leaves. Tag 10 joins
// left to right; tag 01 joins top to bottom. Scaling preserves square leaves.
#include <iostream>
#include <string>
#include <vector>
using namespace std;
struct Node { string tag; int a,b,w,h; };
vector<Node> nodes;
string expr;
int pos;
int gcd(int a,int b) { return b ? gcd(b,a%b) : a; }
int parse() {
    int u=nodes.size();
    Node n; n.tag=expr.substr(pos,2); pos+=2;
    n.a=n.b=-1; n.w=n.h=1; nodes.push_back(n);
    if(n.tag=="01" || n.tag=="10") {
        int a=parse(), b=parse();
        nodes[u].a=a; nodes[u].b=b;
        if(n.tag=="10") {
            int h=nodes[a].h/gcd(nodes[a].h,nodes[b].h)*nodes[b].h;
            nodes[u].h=h;
            nodes[u].w=nodes[a].w*(h/nodes[a].h)+nodes[b].w*(h/nodes[b].h);
        } else {
            int w=nodes[a].w/gcd(nodes[a].w,nodes[b].w)*nodes[b].w;
            nodes[u].w=w;
            nodes[u].h=nodes[a].h*(w/nodes[a].w)+nodes[b].h*(w/nodes[b].w);
        }
    }
    return u;
}
vector<string> picture;
void paint(int u,int x,int y,int scale) {
    const Node &n=nodes[u];
    if(n.a<0) {
        for(int r=y;r<y+n.h*scale;++r)
            for(int c=x;c<x+n.w*scale;++c)
                picture[r][c]=n.tag=="00" ? 'X' : ' ';
    } else if(n.tag=="10") {
        int sa=n.h*scale/nodes[n.a].h;
        int sb=n.h*scale/nodes[n.b].h;
        paint(n.a,x,y,sa);
        paint(n.b,x+nodes[n.a].w*sa,y,sb);
    } else {
        int sa=n.w*scale/nodes[n.a].w;
        int sb=n.w*scale/nodes[n.b].w;
        paint(n.a,x,y,sa);
        paint(n.b,x,y+nodes[n.a].h*sa,sb);
    }
}
int main() {
    while(cin>>expr) {
        nodes.clear(); pos=0; int root=parse();
        picture.assign(nodes[root].h,string(nodes[root].w,' '));
        paint(root,0,0,1);
        string border(nodes[root].w+2,'-');
        cout<<border<<'\n';
        for(int r=0;r<nodes[root].h;++r) cout<<'|'<<picture[r]<<"|\n";
        cout<<border<<'\n';
    }
    return 0;
}
